#include <Qcoreapplication>
#include <QScreen>
#include <QGuiApplication>
#include <qdom.h>
#include <QFile>
#include <QTextStream>
#define CONFIG_H
#include "macro.h"
#include "Config.h"

Config :: Config()
{
	IniAppParameter();
	IniConfigFile();
}

inline void Config::IniAppParameter()
{
	m_strAppPath = QCoreApplication::applicationDirPath();
	m_fScale = QGuiApplication::primaryScreen()->geometry().width() / DEVWIDTH 
		/ (QGuiApplication::primaryScreen()->logicalDotsPerInchX() / PointDistance);
}

inline void Config::IniConfigFile()
{
	QFile file(m_strAppPath + ConfigPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QDomDocument mainDoc;
	if (!mainDoc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();
	QDomElement root = mainDoc.documentElement();
	ReadPlay(&root.firstChildElement("play"));
}

inline void Config::SaveConifgAll()
{
	QFile file(m_strAppPath + ConfigPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	QDomDocument mainDoc;
	if (!mainDoc.setContent(&file))
	{
		file.close();
		return;
	}
	file.close();
	QDomElement root = mainDoc.documentElement();
	WritePlay(&root.firstChildElement("play"));

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
	{
		return;
	}
	QTextStream textStream(&file);
	mainDoc.save(textStream,4);
	file.close();
}

inline void Config::WritePlay(QDomElement* element)
{
	element->firstChildElement("volume").setAttribute("value", QString::number(m_nLVolume));
	element->firstChildElement("playing").setAttribute("path", m_strVideo);
}

inline void Config::ReadPlay(QDomElement* element)
{
	m_nLVolume = element->firstChildElement("volume").attribute("value", "100").toInt();
	m_strVideo = element->firstChildElement("playing").attribute("path");
}

Config::~Config()
{
	SaveConifgAll();
}

Config* Config::GetInstance()
{
	static Config instance;
	return &instance;
}

QPoint Config::GetWorkDesktopPoint_RB()
{
	QSize workSize = QGuiApplication::primaryScreen()->availableSize();  //工作区大小
	return QPoint(workSize.width(), workSize.height());
}

QString Config::GetAppName()
{
	QString strTemp = QCoreApplication::applicationFilePath();
	strTemp = strTemp.right(strTemp.length() - strTemp.lastIndexOf('/') - 1);
	return strTemp.left(strTemp.indexOf('.'));
}