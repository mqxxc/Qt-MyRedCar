#include <QProcess>
#include <QLibrary>
#include <QApplication>
#define STARTUP_H
#include "macro.h"
#include "qtmyredcar.h"
#include "Tray.h"
#include "DeskVideo.h"
#include "awaken_source.h"
#include "StartUp.h"
#include "Config.h"

StartUp::StartUp() 
{
//初始化操作
	init();
//与除主窗口外的部件建立联系
	connectForOther();
}
StartUp::~StartUp() 
{
	disconnect();
	if (m_pMainWnd != nullptr)
	{
		delete m_pMainWnd;
	}
	delete m_pDesk;
	delete m_pTray;
}

void StartUp::setApp(QApplication* app)
{
	m_pApp = app;
}

void StartUp::awakenExe()
{
	if (m_pMainWnd != nullptr)
	{
		if (m_pMainWnd->isMinimized())
		{//最小化
			m_pMainWnd->showNormal();
		}
		else 
		{//在其他窗口后面
			m_pMainWnd->hide();
			m_pMainWnd->show();
		}
	}
	else 
	{//最小化托盘状态
		m_pTray->hide();
		mainWidNew();
	}
}

void StartUp::connectForOther() 
{
	connect(m_pTray,  &Tray::DeskVolumeChange, m_pDesk, &DeskVideo::SetVolume);
	connect(m_pTray,  &Tray::MainAppExit,        this, &StartUp::exitToApp);
	connect(m_pTray,  &Tray::ShowMainWnd,          this, &StartUp::mainAwaken);
	connect(m_pDesk,  &DeskVideo::NoVideo,    this, &StartUp::mainWidNew);
	connect(m_pDesk,  &DeskVideo::TrayShow,   m_pTray, &Tray::SetVolume);
	connect(m_pTray,  &Tray::DeskStateChange,     m_pDesk, &DeskVideo::StopOrStart);
	connect(m_pDesk,  &DeskVideo::LoadFinish, this, &StartUp::loadFinish);
	connect(m_pSrever,&Server::Awaken,        this, &StartUp::awakenExe);
}

void StartUp::init()
{
	initMember();
}

void StartUp::initMember()
{
	m_pLoadProc = nullptr;
	m_pMainWnd = nullptr;
	m_pTray = new Tray;
	m_pDesk = new DeskVideo;
	m_pSrever = new Server;
}

void StartUp::Loading()
{
	if (m_pLoadProc == nullptr) 
	{
		m_pLoadProc = new QProcess;
	}

	m_pLoadProc->start(CONFIG->m_strAppPath + "/" + load_path);
}

void StartUp::loadFinish()
{
	if (m_pLoadProc != nullptr) 
	{
		m_pLoadProc->terminate();
		delete m_pLoadProc;
		m_pLoadProc = nullptr;
	}
}

void StartUp::connectForMW()
{
	connect(m_pMainWnd, &QtMyRedCar::MainAppExit,		this, &StartUp::exitToApp);
	connect(m_pMainWnd, &QtMyRedCar::WndToClear,		this, &StartUp::mainWidDelete);
	connect(m_pMainWnd, &QtMyRedCar::upDesk,			m_pDesk, &DeskVideo::ReplaceDesk);
	connect(m_pMainWnd, &QtMyRedCar::upDesk,			m_pTray, &Tray::SetState);
	connect(m_pMainWnd, &QtMyRedCar::LoadFinishSig,    this, &StartUp::mainWidShow);
}

void StartUp::exitToApp()
{
	m_pApp->exit();
}

void StartUp::mainWidNew()
{
	Loading();
	m_pMainWnd = new QtMyRedCar;
	connectForMW();
	m_pMainWnd->show();
	m_pMainWnd->hide();
	m_pMainWnd->IniUnits();
}

void StartUp::mainWidShow()
{
	loadFinish();
	m_pMainWnd->show();
}

void StartUp::mainAwaken()
{
	Loading();
	mainWidNew();
}

void StartUp::mainWidDelete() 
{
	if (m_pMainWnd != nullptr)
	{
		delete m_pMainWnd;
		m_pMainWnd = nullptr;
		m_pTray->show();
	}
}