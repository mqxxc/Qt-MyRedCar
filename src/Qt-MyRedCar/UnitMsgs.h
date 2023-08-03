#pragma once
#include <QVector>
#include <QDate>
#include "UnitMsg.h"

class UnitMsgs
{
public:
	UnitMsgs();
	~UnitMsgs();
	//数据增删改查
	void addData(QString fileNmae,bool success=true);
	void DeleteData(char id);
	UnitMsg* GetData(ushort id);
	//其他
	int Count();								//统计资源数
	void ReFresh(bool bIsShow);					//刷新所有资源显示状态
	

private:
	QVector<UnitMsg*> m_arrDatas;				//数据集

	void ReadMsg();								//从本地读取数据
};