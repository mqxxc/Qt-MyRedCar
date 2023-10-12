﻿#include <QDir>
#include <QFile>
#include <QMap>
#include <QLibrary>
#include <QWheelEvent>
#include <QCollator>
#define RcWidget_H
#include "macro.h"
#include "Config.h"
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
}

RcWidget::~RcWidget() 
{
	m_pRcs->SaveUnitMsgs();
//将本次运行的各种数据进行保存到本地缓存文件
	delete m_pRcs;
//回收units
	for (int i = 0; i < m_arrUnits.size(); ++i)
	{
		delete m_arrUnits[i];
	}
}

void RcWidget::SetScaling(float fScaling)
{
	Unit::SetScaling(fScaling);
}

void RcWidget::InitMember()
{
	m_pRcs = new UnitMsgs;
	m_nControlSig = 0;
	SetOrder(true);
	m_sortCriteria = SortCriteria::onoe;
}

void RcWidget::CreateUnit(int i)
{
	Unit::SetDatas(m_pRcs);
	for (; i < m_pRcs->Count();++i) 
	{
		m_arrUnits.insert(i, new Unit(i, this));
		connect(m_arrUnits[i], &Unit::UpdateUnits,  this, &RcWidget::SelectUnit);
		connect(m_arrUnits[i], &Unit::RenameSig, this, &RcWidget::fileRename);
		m_listIDList.append(i);
	}
}

void RcWidget::InitUnitSize() 
{
//计算图片单元的大小
	m_UnitMsg.m_nUnitW = Unit::GetWnd_Width();					//单元的宽
	m_UnitMsg.m_nSpaceW = Unit::GetWnd_Width() / ratio;			//间隙宽
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
	UnitMsg* op = m_pRcs->GetData(sum);
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
	CreateUnit(0);
	RefreshUnit();
	emit IniFinish();
}

void RcWidget::ChangeOrder(bool state)
{
	SetOrder(state);
	RefreshUnit();
}

void RcWidget::AddUnit()
{
	int i = m_arrUnits.size() + 1;
	CreateUnit(i);
	m_arrUnits[i]->move(m_nLastUnitX, m_nLastUnitY);
	m_arrUnits[i]->show();
	ReckonNext();
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
}

void RcWidget::mousePressEvent(QMouseEvent* event)
{
	int nCurrentID = Unit::CurrentID();
	Unit::ReleaseAll();
	SelectUnit(nCurrentID);
	emit SelectUnitSig("");
}

void RcWidget::SelectUnit(int nOldID)
{
	if (nOldID != -1)
	{
		m_arrUnits.at(nOldID)->OnRelease();
	}

	int nCurrentID = Unit::CurrentID();
	if (nCurrentID != -1)
	{
		emit SelectUnitSig(m_pRcs->GetData(nCurrentID)->GetVideoPath());
	}
}

void  RcWidget::fileRename(int id,QString name)
{
	//仅仅更新文件夹的名字名字
}