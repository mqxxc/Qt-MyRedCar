#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QLibrary>
#include <QTimer>
#include <QWheelEvent>
#include <QCollator>
#define RcWidget_H
#include "macro.h"
#include "RcWidget.h"
#include "Unit.h"
#include "UnitMsgs.h"
#include "../Screenshot/Screenshot.h"
#pragma execution_character_set("utf-8")


RcWidget::RcWidget(QWidget* parent) 
{
//其他初始化
	InitMember();
	InitUnitSize();

//延迟初始化
	QTimer::singleShot(10, this, [=]() {
		InitUnit(1);
		RefreshUnit();
		InitRc();
		SetRefreshbit(true);
		   }); 
	
}

RcWidget::~RcWidget() 
{
//将本次运行的各种数据进行保存到本地缓存文件
	delete m_pRcs;
//回收units
	for (QVector<Unit*>::iterator ti = m_arrUnits.begin(); ti != m_arrUnits.end(); ++ti)
	{
		delete (*ti);
	}
}

void RcWidget::SetScaling(float fScaling)
{
	Unit::setScaling(fScaling);
}

void RcWidget::InitRc()
{
	if (m_pRcs->Count()==0)
	{
	}
}

void RcWidget::InitMember()
{
	m_pRcs = new UnitMsgs();
	SetRefreshbit(false);
	SetOrder(true);
	m_sortCriteria = SortCriteria::onoe;
}

void RcWidget::InitUnit(int i)
{
	Unit::SetDatas(m_pRcs);
	for (; i < m_pRcs->Count();++i) 
	{
		m_arrUnits[i] = new Unit(i, this);
		connect(m_arrUnits[i], &Unit::UpdateUnits,  this, &RcWidget::Release);
		connect(m_arrUnits[i], &Unit::RenameSig, this, &RcWidget::fileRename);
		m_listIDList.append(i);
	}
}

void RcWidget::InitUnitSize() 
{
//计算图片单元的大小
	m_UnitMsg.m_nUnitW = Unit::GetWnd_W();					//单元的宽
	m_UnitMsg.m_nSpaceW = Unit::GetWnd_W() / ratio;			//间隙宽
	int divisor = m_UnitMsg.m_nUnitW + m_UnitMsg.m_nSpaceW;		//二者宽的和
	m_UnitMsg.m_nRowN = (m_nWndW - m_UnitMsg.m_nSpaceW) / divisor;	//一行能存放的个数
	m_UnitMsg.m_nOffset= m_nWndW % divisor / 2;					//两边的偏移量
}

void RcWidget::RefreshUnit()
{
	m_nLastUnitX = m_UnitMsg.m_nOffset;
	m_nLastUnitY = 0;
	if (IsNormal())
	{
		for (QList<ushort>::const_iterator it = m_listIDList.constBegin(); it != m_listIDList.constEnd(); ++it)
		{
			ShowUnit(*it);
		}
	}else {
		for (QList<ushort>::const_reverse_iterator it = m_listIDList.rbegin(); it != m_listIDList.rend(); ++it) 
		{
			ShowUnit(*it);
		}
	}

	if (!IsRefresh())
	{//仅在第一次初始化是发射信号
		emit IniFinish();
	}
}

void RcWidget::ShowUnit(int sum) 
{
	if (m_pRcs->GetData(sum)->IsShow())
	{
		m_arrUnits[sum]->move(m_nLastUnitX, m_nLastUnitY);
		m_arrUnits[sum]->show();
		ReckonNext();
	}
}

void RcWidget::Refresh()
{
	m_pRcs->ReFresh(true);
	RefreshUnit();
}

void RcWidget::SearchUnits(QString name)
{
	m_pRcs->ReFresh(true);
	for (int i = 0; i < m_pRcs->Count(); ++i) 
	{
		QString rcName =m_pRcs->GetData(m_listIDList[i])->GetName();
		m_arrUnits[m_listIDList[i]]->hide();
		if (rcName.contains(name)) 
		{
			m_pRcs->GetData(m_listIDList[i])->SetVisual(true);
		}
	}
	RefreshUnit();

}

void RcWidget::ChangeCondition(SortCriteria condition)
{
	if (m_sortCriteria == condition)
	{
		return;
	}
	m_sortCriteria = condition;
	switch (m_sortCriteria)
	{
		case RcWidget::SortCriteria::onoe:
			break;
		case RcWidget::SortCriteria::name:
		{
			QCollator collator(QLocale::Chinese);
			QStringList list;
			QMap<QString, int>map;
			for (int i = 0; i < m_pRcs->Count(); ++i) 
			{
				QString temp = m_pRcs->GetData(i)->GetName();
				list << temp;
				map[temp] = i;
			}
			std::sort(list.begin(), list.end(), collator);
			m_listIDList.clear();
			for (int i = 0; i < m_pRcs->Count(); ++i) 
			{
				m_listIDList.append(map[list.at(i)]);
			}
		}break;
		case RcWidget::SortCriteria::ImportTime:
			break;
		case RcWidget::SortCriteria::lastTime:
		{
			
		}break;
		case RcWidget::SortCriteria::love:
		{
			QMap<QString, int>map;
			m_listIDList.clear();
			for (int i = 0; i < m_pRcs->Count(); ++i) 
			{
				if (m_pRcs->GetData(i)->IsLove()) 
				{
					m_listIDList.prepend(i);
				}
				else 
				{
					m_listIDList.append(i);
				}
			}
		}break;
	}
}

void RcWidget::ChangeOrder(bool state)
{
	SetOrder(state);
	RefreshUnit();
}

void RcWidget::AddUnit()
{
	int i = m_arrUnits.size() + 1;
	InitUnit(i);
	m_arrUnits[i]->move(m_nLastUnitX, m_nLastUnitY);
	m_arrUnits[i]->show();
	ReckonNext();
}

void RcWidget::ReckonNext(){
	m_nLastUnitX += m_UnitMsg.m_nUnitW + m_UnitMsg.m_nSpaceW;
	if (m_nLastUnitX+ m_UnitMsg.m_nUnitW > m_nWndW) {
		m_nLastUnitY += m_UnitMsg.m_nUnitW + m_UnitMsg.m_nSpaceW;
		m_nLastUnitX = m_UnitMsg.m_nOffset;
	}
}

inline void RcWidget::SetRefreshbit(bool bRefresh)
{
}

inline bool RcWidget::IsRefresh()
{
	return false;
}

inline bool RcWidget::IsNormal()
{
	return false;
}

inline void RcWidget::SetOrder(bool bNormal)
{
}

bool RcWidget::MoveFile(QString path)
{
	//QString filePath(this->path);
	//QString video = (path.section("/", -1));
	//QString name=video.section(".",-3,-2);			
	//filePath.append(dataPath).append(name);			//以视频名字作为文件夹名字
	//QDir dir;
	//QFile file(path);

	//if (dir.mkdir(filePath)) {			//创建文件夹
	//	if (file.rename(filePath.append("/").append(video))) 
	//	{

	//		QLibrary dll("Screenshot");
	//		Screenshot* getPotho;
	//		if(dll.load()) {
	//			getPotho = ((Screenshot * (*)())(dll.resolve("getScrObj")))();
	//		}
	//		QEventLoop loop;
	//		getPotho->SetLoop(&loop);
	//		getPotho->GetJpg(filePath);
	//		//dxg提示框正在导入
	//		loop.exec();	
	//		m_pRcs->addData(filePath, getPotho->IsSuccess());

	//		AddUnit();
	//		
	//		delete getPotho;
	//		dll.unload();
	//		return true;
	//	}
	//}
	//return false;
	return false;
}

void RcWidget::resizeEvent(QResizeEvent* e)
{
	m_nWndW = this->size().width();
	m_nWndH = this->size().height();
	InitUnitSize();
	if (m_nControlSig&1) 
	{

		RefreshUnit();
	}
}

void RcWidget::mousePressEvent(QMouseEvent* event)
{
	Unit::ReleaseAll();
	emit SelectUnit("");
}

void RcWidget::Release(int id)
{
	if (id != 0)
	{
		emit SelectUnit(m_pRcs->GetData(id)->GetVideoPath());
	}
}

void  RcWidget::fileRename(int id,QString name)
{
	//仅仅更新文件夹的名字名字
}