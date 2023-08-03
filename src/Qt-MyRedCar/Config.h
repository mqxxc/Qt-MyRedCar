#pragma once
//配置参数
#include <QString>
#include <QPoint>
#define CONFIG Config::GetInstance()


class Config
{
public:
	QString m_strAppPath;			//程序运行路径结尾无“/”或者“\”
	double m_fScale;				//程序窗口自适应大小缩放比例
	QString m_strVideo;				//播放的视频路径
	uchar m_nLVolume;				//视频音量

	~Config();
	static Config* GetInstance();		
	QPoint GetWorkDesktopPoint_RB();	//获取工作桌面右下角的坐标
	QString GetAppName();				//获取应用程序名字


private:
	Config();
	inline void IniAppParameter();
};