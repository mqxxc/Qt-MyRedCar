﻿#include <QXmlStream.h>
#include <QMessageBox>
#include <QFile>
#define UnitMsgs_H
#include "macro.h"
#include "UnitMsgs.h"
#include "Config.h"
#include "../three/YQTools_Qt/YQTools_Qt.h"
#pragma execution_character_set("utf-8")

UnitMsgs::UnitMsgs()
{
}

UnitMsgs::~UnitMsgs()
{
	if (!m_arrDatas.isEmpty())
	{
		for (int i = 0; i < m_arrDatas.size(); ++i)
		{
			if (m_arrDatas[i] != nullptr)
			{
				delete m_arrDatas[i];
			}
		}
	}
	m_arrDatas.clear();
}

void UnitMsgs::AddData(QString fileNmae, bool success)
{
	fileNmae = fileNmae.right(fileNmae.size() - fileNmae.lastIndexOf("/") - 1);
	fileNmae = fileNmae.left(fileNmae.lastIndexOf("."));
	m_arrDatas.insert(m_arrDatas.size(), new UnitMsg(fileNmae, success));
}

void UnitMsgs::AddData(QXmlStreamAttributes* attributes)
{
	m_arrDatas.insert(m_arrDatas.size(), new UnitMsg(attributes));
}

void UnitMsgs::DeleteData(ushort id)
{
	delete m_arrDatas[id];
	m_arrDatas[id] = nullptr;
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
	for (QVector<UnitMsg*>::const_iterator ti = m_arrDatas.begin(); ti != m_arrDatas.end(); ++ti)
	{
		(*ti)->SetVisual(state);
	}
}

void UnitMsgs::ReadUnitMsgs()
{
	//读取文档
	QFile file(YQTools_Qt::ToAbsolutePath(initTextPath));
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	QXmlStreamReader dataReadre(&file);
	while (!dataReadre.atEnd())
	{
		switch (dataReadre.readNext())
		{
			case QXmlStreamReader::StartElement:
			{
				if (dataReadre.name() == "Items")
				{
					ReadDataMsgItems(&dataReadre);
				}
			}break;
		};
	}
}

void UnitMsgs::SaveUnitMsgs()
{
	QFile file(YQTools_Qt::ToAbsolutePath(initTextPath));
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		return;
	}
	QXmlStreamWriter dataWriter(&file);
	dataWriter.setAutoFormatting(true);
	dataWriter.writeStartDocument();
	dataWriter.writeStartElement("IniRc");
	dataWriter.writeStartElement("Items");
	for (QVector<UnitMsg*>::const_iterator it = m_arrDatas.cbegin(); it != m_arrDatas.cend(); ++it)
	{
		if ((*it) == nullptr)
		{
			continue;
		}
		dataWriter.writeEmptyElement("Item");
		dataWriter.writeAttribute("name", (*it)->WriteName());
		dataWriter.writeAttribute("videoPath", (*it)->WriteVideoPath());
		dataWriter.writeAttribute("lastTime", (*it)->WriteUserTime());
		dataWriter.writeAttribute("source", (*it)->WriteSource());
		dataWriter.writeAttribute("downloadTime", (*it)->WriteMportTime());
		dataWriter.writeAttribute("isLove", (*it)->WriteLove());
	}
	dataWriter.writeEndElement();
	dataWriter.writeEndElement();
	dataWriter.writeEndDocument();
}

void UnitMsgs::ReadDataMsgItems(QXmlStreamReader* reader)
{
	while (!reader->atEnd())
	{
		switch (reader->readNext())
		{
			case QXmlStreamReader::StartElement:
			{//读取Items下的所有自闭合标签
				AddData(&reader->attributes());
			}break;
			case QXmlStreamReader::EndElement:
			{//读取到遇到的第一个结束标签
				return;
			}break;
		}
	}
}