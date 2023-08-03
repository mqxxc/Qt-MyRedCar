#include <qcoreapplication.h>
#include <QScreen>
#include <QGuiApplication>
#include "macro.h"
#include "Config.h"

Config :: Config()
{
	IniAppParameter();
}

inline void Config::IniAppParameter()
{
	m_strAppPath = QCoreApplication::applicationDirPath();
	m_fScale = QGuiApplication::primaryScreen()->geometry().width() / DEVWIDTH 
		/ (QGuiApplication::primaryScreen()->logicalDotsPerInchX() / PointDistance);

}

Config::~Config()
{
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