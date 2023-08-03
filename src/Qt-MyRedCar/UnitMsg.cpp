#include <QXmlStreamAttributes>
#define UnitMsg_H
#include "macro.h"
#include "UnitMsg.h"
#include "Config.h"

UnitMsg::UnitMsg(QXmlStreamAttributes* attributes)
{
	m_strName = attributes->value("name").toString();
	m_strVideo = attributes->value("videoPath").toString();
	m_strPhoto = m_strVideo.left(m_strVideo.indexOf(".")) + dePhotoS;
	m_dateMportTime.fromString(attributes->value("videoPath").toString());
	switch (attributes->value("source").toString().toInt())
	{
	case 0:
	{
		m_source = Source::CRAFT;
	}break;
	case 2:
	{
		m_source = Source::INTERNET;
	}break;
	default:
	{
		m_source = Source::LOCAL;
	}break;
	}

	m_dateUserTime.fromString(attributes->value("lastTime").toString());
	m_cState = 0;
	SetLove(attributes->value("isLove").toString() == "1");
}

UnitMsg::UnitMsg(QString strFileName, bool bHaveThumbnail)
{
	m_strName = strFileName.left(strFileName.indexOf("."));
	m_strVideo = strFileName;
	bHaveThumbnail ? m_strPhoto = strFileName + dePhotoS : m_strPhoto = dePhotoPath;
	m_dateMportTime = QDate::currentDate();
	m_dateUserTime.fromString(deusedTime);
	m_source = Source::LOCAL;
	m_cState = 0;
}


QString UnitMsg::GetName()
{
	return m_strName;
}

QString UnitMsg::GetPhotoPath()
{
	return CONFIG->m_strAppPath + "/" + recordPath + m_strName + "/" + m_strPhoto;
}

QString UnitMsg::GetVideoPath()
{
	return CONFIG->m_strAppPath + "/" + recordPath + m_strName + "/" + m_strVideo;
}

QDate UnitMsg::GetMportTime()
{
	return m_dateMportTime;
}

QDate UnitMsg::GetUserTime()
{
	return m_dateUserTime;
}

UnitMsg::Source UnitMsg::GetSource()
{
	return m_source;
}

void UnitMsg::SetName(QString strName)
{
	m_strName = strName;
}

void UnitMsg::SetPhoto(QString strPath)
{
	m_strPhoto = strPath;
}

void UnitMsg::SetVideo(QString strPath)
{
	m_strVideo = strPath;
}

void UnitMsg::SetMporTime(QDate date)
{
	m_dateMportTime = date;
}

void UnitMsg::SetUserTime(QDate date)
{
	m_dateUserTime = date;
}

void UnitMsg::SetSource(Source source)
{
	m_source = source;
}