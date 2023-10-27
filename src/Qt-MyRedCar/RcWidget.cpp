#include <QMap>
#include <QDir>
#include <QLibrary>
#include <QWheelEvent>
#include <QCollator>
#include <QEventLoop>
#define RcWidget_H
#include "macro.h"
#include "Config.h"
#include "RcWidget.h"
#include "Unit.h"
#include "UnitMsgs.h"
#include "../Screenshot/Screenshot.h"
#pragma execution_character_set("utf-8")


RcWidget::RcWidget(QWidget* parent) :QWidget(parent)
{
//其他初始化
	InitMember();
}

RcWidget::~RcWidget() 
{
	m_pRcs->SaveUnitMsgs();
	Unit::SetDatas(nullptr);
	delete m_pRcs;
	Unit::DesStaticRes();
}

inline void RcWidget::InitMember()
{
	Unit::CreateStaticRes();
	m_arrUnits.empty();
	m_pRcs = new UnitMsgs;
	Unit::SetDatas(m_pRcs);
	m_nControlSig = 0;
	SetOrder(true);
	m_sortCriteria = SortCriteria::onoe;
}

void RcWidget::ReUnitSize() 
{
//计算图片单元的大小
	m_UnitMsg.m_nUnitW = Unit::GetWnd_Width();						//单元的宽
	m_UnitMsg.m_nSpaceW = Unit::GetWnd_Width() / ratio;				//间隙宽
	int divisor = m_UnitMsg.m_nUnitW + m_UnitMsg.m_nSpaceW;			//二者宽的和
	m_UnitMsg.m_nRowN = (m_nWndW - m_UnitMsg.m_nSpaceW) / divisor;	//一行能存放的个数
	m_UnitMsg.m_nOffset= m_nWndW % divisor / 2;						//两边的偏移量
}

void RcWidget::CreateUnit()
{
	int nID = m_arrUnits.count();
	m_arrUnits.insert(nID, new Unit(nID, this));
	connect(m_arrUnits[nID], &Unit::SelectUnits, this, &RcWidget::SelectUnit);
	m_listIDList.append(nID);
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
	}else 
	{
		for (QList<ushort>::const_reverse_iterator it = m_listIDList.rbegin(); it != m_listIDList.rend(); ++it) 
		{
			ShowUnit(*it);
		}
	}
}

void RcWidget::ShowUnit(int sum)
{
	if (!m_pRcs->GetData(sum)->IsShow())
	{
		return;
	}
	m_arrUnits[sum]->move(m_nLastUnitX, m_nLastUnitY);
	m_arrUnits[sum]->show();
	ReckonNext();
}

void RcWidget::Refresh()
{
	m_pRcs->ReFresh(true);
	RefreshUnit();
}

void RcWidget::SearchUnits(QString name)
{
	for (int i = 0; i < m_pRcs->Count(); ++i) 
	{
		QString rcName =m_pRcs->GetData(m_listIDList[i])->GetName();
		m_arrUnits[m_listIDList[i]]->hide();
		if (rcName.indexOf(name) != -1)
		{
			m_pRcs->GetData(m_listIDList[i])->SetVisual(true);
		}
		else
		{
			m_pRcs->GetData(m_listIDList[i])->SetVisual(false);
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

void RcWidget::IniUnits()
{
	if (!m_arrUnits.empty())
	{
		return;
	}
	m_pRcs->ReadUnitMsgs();
	for (int i = 0; i < m_pRcs->Count(); ++i)
	{
		CreateUnit();
	}
	RefreshUnit();
	emit IniFinish();
}

void RcWidget::ChangeOrder(bool state)
{
	SetOrder(state);
	RefreshUnit();
}

void RcWidget::ReckonNext(){
	m_nLastUnitX += m_UnitMsg.m_nUnitW + m_UnitMsg.m_nSpaceW;
	if (m_nLastUnitX+ m_UnitMsg.m_nUnitW > m_nWndW)
	{
		m_nLastUnitY += m_UnitMsg.m_nUnitW + m_UnitMsg.m_nSpaceW;
		m_nLastUnitX = m_UnitMsg.m_nOffset;
	}
}

inline bool RcWidget::IsNormal()
{
	return m_nControlSig & 2;
}

inline void RcWidget::SetOrder(bool bNormal)
{
	if (bNormal)
	{
		m_nControlSig |= 2;
	}
	else
	{
		m_nControlSig &= maxControlSig ^ 2;
	}
}

bool RcWidget::AddUnit(QString path)
{
	QString video = (path.section("/", -1));
	QString name = video.section(".", -3, -2);
	QString filePath = CONFIG->m_strAppPath + dataPath + name;			//以视频名字作为文件夹名字
	QDir dir;
	
	if (!dir.mkdir(filePath))
	{//创建文件夹
		return false;
	}

	QFile file(path);
	if (!file.copy(filePath.append("/").append(video)))
	{//复制视频文件
		return false;
	}

	QLibrary dll(CONFIG->m_strAppPath + "/dll/Screenshot");
	if (!dll.load())
	{//加载制作缩略图dll，DXG改为生成缩略图失败后绑定默认缩略图
		return false;
	}
	Screenshot* getPotho = ((Screenshot * (*)())(dll.resolve("GetScreenshot")))();

	QEventLoop loop;
	getPotho->SetLoop(&loop);
	getPotho->GetJpg(filePath);
	//dxg提示框正在导入
	loop.exec();
	m_pRcs->AddData(filePath, getPotho->IsSuccess());

	CreateUnit();
	Unit* temp = m_arrUnits.last();
	temp->move(m_nLastUnitX, m_nLastUnitY);
	temp->show();
	ReckonNext();

	getPotho->ReleaseScreenshot();
	dll.unload();

	return true;
}

void RcWidget::resizeEvent(QResizeEvent* e)
{
	m_nWndW = size().width();
	m_nWndH = size().height();
	ReUnitSize();
	RefreshUnit();
}

void RcWidget::mousePressEvent(QMouseEvent* event)
{
	Unit::DeselectAll();
	CONFIG->m_strVideoPath.clear();
	emit SelectUnitSig();
}

void RcWidget::SelectUnit(int nCurrentID)
{
	CONFIG->m_strVideoPath = m_pRcs->GetData(nCurrentID)->GetVideoPath();
	emit SelectUnitSig();
}