#include <Qsize>
#include <QGuiApplication>
#include <QScreen>
#include "Config.h"

Config::Config()
{
	m_strVideoPath.clear();
}

Config::~Config()
{
}

Config* Config::GetInstance()
{
	static Config obj;
	return &obj;
}

QPoint Config::GetWorkDesktopPoint_RB()
{
	QSize workSize = QGuiApplication::primaryScreen()->availableSize();  //工作区大小
	return QPoint(workSize.width(), workSize.height());
}