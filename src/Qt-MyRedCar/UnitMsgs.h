﻿#pragma once
#include <QVector>
#include <QDate>
#include "UnitMsg.h"
class QXmlStreamReader;

class UnitMsgs
{
public:
	UnitMsgs();
	~UnitMsgs();
	//数据增删改查
	void AddData(QString fileNmae,bool success=true);
	void AddData(QXmlStreamAttributes* attributes);
	void DeleteData(char id);
	UnitMsg* GetData(ushort id);
	//其他
	int Count();								//统计资源数
	void ReFresh(bool bIsShow);					//刷新所有资源显示状态
	void ReadUnitMsgs();						//从文件信息中保存信息
	void SaveUnitMsgs();						//保存所有单元信息
	

private:
	QVector<UnitMsg*> m_arrDatas;				//数据集

	void ReadDataMsgItems(QXmlStreamReader* reader);	//辅助初始化单元信息
};