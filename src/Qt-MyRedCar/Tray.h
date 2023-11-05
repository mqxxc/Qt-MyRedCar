#pragma once
#include <QObject>
#include <QList>
#include <QMap>
#include <QSystemTrayIcon>
class QMenu;
class About;
class QSlider;
class QLabel;
class QEventLoop;
class QAction;

class Tray :public QObject
{
	Q_OBJECT

public:
	Tray(QObject* parent = nullptr);
	~Tray();

	void show();									//显示托盘
	void hide();									//隐藏托盘


private:
	QMap<QString, void (Tray::*)(bool)> m_mapMenuAction;	//托盘右键菜单动作表
	QSystemTrayIcon* m_pTrayIco;					//托盘类实例
	QMenu* m_pMenu;									//托盘菜单
	QList<QAction*> m_listActions;					//菜单项
	QWidget* volumScr;								//音量条		
	int m_nOldVolume;								//静音前设置的音量
	QEventLoop* loop;								//事件循环
	QSlider* scr;									//音量调节部件
	QLabel* volumeico;								//音量设置ico部件
	//false----静音状态，true-----非静音状态
	bool m_bVIcoState;								//音量设置ico状态

	inline void init();								//总初始化函数
	inline void InitTray();							//初始化托盘
	inline void InitMenus();						//初始化菜单
	inline void initScr();							//初始化音量条
	inline void connects();							//建立联系
	void setVolumFish();							//音量设置完成
	void ChangeScrIco();							//改变音量滑块下图标


signals:
	void DeskVolumeChange();							//设置壁纸音量
	void DeskStateChange(int state);					//暂停或者开始播放视频
	void MainAppExit();								//主程序退出
	void ShowMainWnd();									//主窗口显示


public slots:
	void eventHandler(QSystemTrayIcon::
		ActivationReason reason);						//事件处理
	bool eventFilter(QObject* object, QEvent* event);   //子控件事件监听器
	//菜单对应的动作
	void MenuAction(bool bCheckState);
	void MenuOpen(bool bCheckState);				//打开							
	void MenuVolumeChange(bool bCheckState);		//设置声音
	void MenuVideoStateChange(bool bCheckState);	//设置视频播放状态
	void MenuAbout(bool bCheckState);				//关于
	void MenuExit(bool bCheckState);				//退出
	void SetVolume();								//设置音量
	void SetState();								//设置视频播放状态
};