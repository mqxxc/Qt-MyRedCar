#include <QLibrary>
#include <QDateTime>
#include <QFile>
#include <QXmlStream.h>
#include <QMessageBox>
#include "UnitMsgs.h"
#include "Config.h"
#pragma execution_character_set("utf-8")

UnitMsgs::UnitMsgs()
{
	ReadMsg();
}

UnitMsgs::~UnitMsgs()
{
	while (!m_arrDatas.empty())
	{
		for (QVector<UnitMsg*>::const_iterator ti = m_arrDatas.cbegin(); ti != m_arrDatas.cend(); ++ti)
		{
			delete (*ti);
		}
	}
}

void UnitMsgs::addData(QString fileNmae, bool success)
{
	fileNmae = fileNmae.right(fileNmae.size() - fileNmae.lastIndexOf("/") - 1);
	fileNmae = fileNmae.left(fileNmae.lastIndexOf("."));
	m_arrDatas.insert(m_arrDatas.size(), new UnitMsg(fileNmae, success));
}

void UnitMsgs::DeleteData(char id)
{
	delete m_arrDatas[id];
	m_arrDatas[id] = nullptr;
	m_arrDatas.remove(id);
}

UnitMsg* UnitMsgs::GetData(ushort id)
{
	if (id > m_arrDatas.size())
	{
		return nullptr;
	}
	return m_arrDatas.at(id);
}

int UnitMsgs::Count()
{
	return m_arrDatas.size();
}

void UnitMsgs::ReFresh(bool state)
{
	for (QVector<UnitMsg*>::const_iterator ti = m_arrDatas.cbegin(); ti != m_arrDatas.cend(); ++ti)
	{
		(*ti)->SetVisual(state);
	}
}

void UnitMsgs::ReadMsg()
{//读取xml文件数据缓存
	QString strDtatPath = CONFIG->m_strAppPath + "/conf/dataMsg.xml";
	QFile file(strDtatPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(nullptr, "提示", "数据初始化失败！");
		return;
	}

	UnitMsg* temp = nullptr;
	QXmlStreamReader xmlReader(&file);
	while (!xmlReader.atEnd())
	{
		if (xmlReader.readNext() != QXmlStreamReader::StartElement)
		{
			continue;
		}
		temp = new UnitMsg(&xmlReader.attributes());
	}
}