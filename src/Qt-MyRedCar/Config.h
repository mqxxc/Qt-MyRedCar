#pragma once
//配置参数
#include <QString>
#include <QPoint>
#define CONFIG Config::GetInstance()
class QDomElement;

class Config
{
public:
	QString m_strAppPath;			//程序运行路径结尾无“/”或者“\”
	double m_fScale;				//程序窗口自适应大小缩放比例
	uchar m_nLVolume;				//视频音量
	QString m_strVideo;				//播放的视频路径
	~Config();
	static Config* GetInstance();		
	QPoint GetWorkDesktopPoint_RB();	//获取工作桌面右下角的坐标
	QString GetAppName();				//获取应用程序名字


private:
	Config();
	inline void IniAppParameter();			//初始化运行参数
	inline void IniConfigFile();			//初始化config.xml文件内容
	inline void ReadPlay(QDomElement* element);		//读取配置文件中play节点信息
	inline void SaveConifgAll();			//保存config.xml文件内容
	inline void WritePlay(QDomElement* element);	//保存配置文件中play节点信息
};