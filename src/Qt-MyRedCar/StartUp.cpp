#include <QProcess>
#include <QLibrary>
#include <QApplication>
#define STARTUP_H
#include "macro.h"
#include "Tray.h"
#include "DeskVideo.h"
#include "awaken_source.h"
#include "StartUp.h"
#include "Config.h"
#include "../MainWnd_DLL/ThemeWnd.h"
#include "../three/YQTools_Qt/YQTools_Qt.h"
#pragma execution_character_set("utf-8")

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
	loadFinish();
	delete m_pDesk;
	delete m_pTray;
	if (m_pDllMainWnd != nullptr)
	{
		if (m_pDllMainWnd->isLoaded())
		{
			if (m_pMainWnd != nullptr)
			{
				m_pMainWnd->ReleaseInstance();
			}
		}
		m_pDllMainWnd->unload();
	}
	delete m_pDllMainWnd;
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
	m_pTray = new Tray(this);
	m_pDesk = new DeskVideo(this);
	m_pSrever = new Server(this);
	m_pDllMainWnd = nullptr;
}

void StartUp::Loading()
{
	if (m_pLoadProc == nullptr) 
	{
		m_pLoadProc = new QProcess;
	}

	m_pLoadProc->start(YQTools_Qt::ToAbsolutePath(load_path));
}

void StartUp::loadFinish()
{
	if (m_pLoadProc != nullptr) 
	{
		m_pLoadProc->kill();
		m_pLoadProc->waitForFinished();
		delete m_pLoadProc;
		m_pLoadProc = nullptr;
	}
}

void StartUp::exitToApp()
{
	m_pApp->exit();
}

void StartUp::UpdateDesVidoe(QString strVideoPath)
{
	CONFIG->m_strVideoPath = strVideoPath;
	m_pDesk->ReplaceDesk();
	m_pTray->SetState();
}

void StartUp::mainWidNew()
{
	Loading();
	if (m_pDllMainWnd == nullptr)
	{
		m_pDllMainWnd = new QLibrary(YQTools_Qt::ToAbsolutePath("dll/MainWnd_DLL"));
	}
	if (!m_pDllMainWnd->isLoaded())
	{
		m_pDllMainWnd->load();
		((void(*)(QString))(m_pDllMainWnd->resolve("SetRunPath")))(YQTools_Qt::ToAbsolutePath(""));
		((void(*)(float))(m_pDllMainWnd->resolve("SetScaling")))(1);
	}
	if (m_pMainWnd == nullptr)
	{
		m_pMainWnd = ((ThemeWnd * (*)(QWidget* ))(m_pDllMainWnd->resolve("GetThemeWnd")))(nullptr);
		m_pMainWnd->ConnectSig(SIGNAL(MainAppExit()), this, SLOT(exitToApp()));
		m_pMainWnd->ConnectSig(SIGNAL(WndClose()), this, SLOT(mainWidDelete()));
		m_pMainWnd->ConnectSig(SIGNAL(UpdateDesVidoe(QString)), this, SLOT(UpdateDesVidoe(QString)));
		m_pMainWnd->ConnectSig(SIGNAL(LoadFinishSig()), this, SLOT(mainWidShow()));
	}
	m_pMainWnd->show();
	m_pMainWnd->hide();
	m_pMainWnd->LoadingRc();
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
		m_pMainWnd->ReleaseInstance();
		m_pMainWnd = nullptr;
		m_pTray->show();
	}
	if (m_pDllMainWnd->isLoaded())
	{
		m_pDllMainWnd->unload();
	}
	if (m_pDllMainWnd != nullptr)
	{
		delete m_pDllMainWnd;
		m_pDllMainWnd = nullptr;
	}
}