#pragma once
#include <QObject>
class QtMyRedCar;
class Tray;
class DeskVideo;
class QProcess;
class Server;
class QLibrary;
class ThemeWnd;


class StartUp : public QObject 
{
	Q_OBJECT

public:	
	StartUp();
	~StartUp();
	void setApp(QApplication* app);			//设置事件循环实例
	

private:
	ThemeWnd* m_pMainWnd;					//应用主窗口
	Tray* m_pTray;							//托盘
	DeskVideo* m_pDesk;						//桌面视频类
	QApplication* m_pApp;					//事件循环实例
	QProcess* m_pLoadProc;					//加载界面进程
	Server* m_pSrever;						//进程服务端
	QLibrary* m_pDllMainWnd;				//主窗口dll实例

	void init();
	void initMember();
	void Loading();
	void loadFinish();
	inline void connectForOther();			//除主窗口以外的信号连接


private slots:
	void mainWidDelete();					//主窗口销毁
	void mainWidNew();						//主窗口创建
	void mainWidShow();						//主窗口展示
	void mainAwaken();						//主窗口唤醒
	void awakenExe();						//重复启动唤醒主窗口
	void exitToApp();						//结束事件循环退出应用	
	void UpdateDesVidoe(QString strVideoPath);//更新桌面视频
};