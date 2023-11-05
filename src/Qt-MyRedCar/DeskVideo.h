#pragma once
//替换桌面
#include<QObject>
class VideoPalyer;
class QLibrary;
#pragma execution_character_set("utf-8")

class DeskVideo:public QObject
{
	Q_OBJECT

public:
	DeskVideo(QObject* parent = nullptr);
	~DeskVideo();
	bool ReplaceDesk();						//替换桌面


private:
	QWidget* m_pDeskWnd;	//视频层窗口
	QLibrary* m_pPlayDll;	//dll库
	VideoPalyer* m_pPlayer;	//播放器


signals:
	void NoVideo();						//初始化时无视频播放信号
	void TrayShow();					//托盘显示信号
	void LoadFinish();					//初始化完成完成信号


public slots:
	void StopOrStart(int Start);		//暂停或者开始播放
	void SetVolume();					//设置音量
};