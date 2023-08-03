#include<QProcess>
#include<QLibrary>
#include<QApplication>
#define STARTUP_H
#include"macro.h"
#include"awaken_source.h"
#include"StartUp.h"

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
	if (mainwid != nullptr) 
	{
		delete mainwid;
	}
	delete desk;
	delete tray;
}

void StartUp::setApp(QApplication* app)
{
	this->app = app;
}

void StartUp::awakenExe()
{
	if (mainwid != nullptr)
	{
		if (mainwid->isMinimized()) 
		{
			mainwid->showNormal();
		}
		else 
		{
			mainwid->hide();
			mainwid->show();
		}
	}
	else 
	{
		tray->hide();
		mainWidNew();
	}
}

void StartUp::connectForOther() 
{
	connect(tray,  &Tray::setVolumeToDesk, desk, &DeskVideo::SetVolume);
	connect(tray,  &Tray::mainExit,        this, &StartUp::exitToApp);
	connect(tray,  &Tray::ShowMW,          this, &StartUp::mainAwaken);
	connect(desk,  &DeskVideo::NoVideo,    this, &StartUp::mainWidNew);
	connect(desk,  &DeskVideo::TrayShow,   tray, &Tray::setVolume);
	connect(tray,  &Tray::stateToDesk,     desk, &DeskVideo::StopOrStart);
	connect(tray,  &Tray::setVolumeToDesk, desk, &DeskVideo::SetVolume);
	connect(desk,  &DeskVideo::LoadFinish, this, &StartUp::loadFinish);
	connect(desk,  &DeskVideo::TrayShow,   tray, &Tray::setVolume);
	connect(srever,&Server::Awaken,        this, &StartUp::awakenExe);
}

void StartUp::init()
{
	initMember();
}

void StartUp::initMember()
{
	load = nullptr;
	loading();
	mainwid = nullptr;
	tray = new Tray();
	desk = new DeskVideo();
	srever = new Server();
}

void StartUp::loading()
{
	if (load == nullptr) 
	{
		load = new QProcess;
	}
	QString processPath = exePath + load_path;
	load->start(processPath);
}

void StartUp::loadFinish()
{
	if (load != nullptr) 
	{
		load->terminate();
		delete load;
		load = nullptr;
	}
}

void StartUp::connectForMW()
{
	connect(mainwid, &QtMyRedCar::MainAppExit,		this, &StartUp::exitToApp);
	connect(mainwid, &QtMyRedCar::WndToClear,		this, &StartUp::mainWidDelete);
	connect(mainwid, &QtMyRedCar::upDesk,			desk, &DeskVideo::ReplaceDesk);
	connect(mainwid, &QtMyRedCar::upDesk,			tray, &Tray::setState);
	connect(mainwid, &QtMyRedCar::LoadFinishSig,    this, &StartUp::mainWidShow);
}

void StartUp::exitToApp()
{
	app->exit();
}

void StartUp::mainWidNew()
{
	mainwid = new QtMyRedCar();
	mainwid->show();
	mainwid->hide();
	connectForMW();
}

void StartUp::mainWidShow()
{
	loadFinish();
	mainwid->show();
}

void StartUp::mainAwaken()
{
	loading();
	mainWidNew();
}

void StartUp::mainWidDelete() 
{
	if (mainwid != nullptr) 
	{
		delete mainwid;
		mainwid = nullptr;
		tray->show();
	}
}