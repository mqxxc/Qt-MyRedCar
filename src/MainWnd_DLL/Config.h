#pragma once
#include <QString>
#include <QPoint>
#define CONFIG Config::GetInstance()

class Config
{
public:
	QString m_strVideoPath;

	Config();
	~Config();
	static Config* GetInstance();
	QPoint GetWorkDesktopPoint_RB();//获取工作桌面右下角的坐标
};