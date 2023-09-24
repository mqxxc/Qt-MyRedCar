#include<Windows.h>
#include<Qlibrary>
#include<QWindow>
#include<QTimer>
#include<QWidget>
#include<QVariant>
#include<QFile>
#include<QMessageBox>
#define DeskVideo_H
#include "macro.h"
#include"DeskVideo.h"
#include "Config.h"
#include"../playVideo/playVideo.h"

//查找windows壁纸层窗口并隐藏
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM Lparam) 
{
	HWND hDefView = FindWindowEx(hwnd, nullptr, L"SHELLDLL_DefView", 0);
	if (hDefView != 0) 
	{
		// 找它的下一个窗口，类名为WorkerW
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);
		return false;
	}
	return true;
}

DeskVideo::DeskVideo() 
{
	//初始化变量
	m_pDeskWnd = new QWidget();
	m_pPlayer = nullptr;
	m_pPlayDll = new QLibrary(CONFIG->m_strAppPath + "/dll/playVideo");
	
	HWND hProgman = FindWindow(L"Progman", 0);				//找到PM窗口
	SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);	//给它发特殊消息
	QWindow* winv = QWindow::fromWinId(WId(hProgman));		//找到背后的壁纸层
	//设置视频窗口为背后壁纸层还子窗口并设置视频窗口大小为背后壁纸层层大小
	m_pDeskWnd->setProperty("_q_embedded_native_parent_handle",
		QVariant(WId(hProgman)));				
	m_pDeskWnd->winId();							
	m_pDeskWnd->windowHandle()->setParent(winv);
	m_pDeskWnd->resize(winv->frameGeometry().size());
	m_pDeskWnd->move(0, 0);
	//找到winddows壁纸层并隐藏
	EnumWindows(EnumWindowsProc, 0);

	//其他初始化操作
	if (m_pPlayDll->load())
	{
		m_pPlayer = ((VideoPalyer * (*)(QWidget*))(m_pPlayDll->resolve("GetpPlayVideo")))(m_pDeskWnd);
	}

	//读取上次保存的设置信息
	QTimer::singleShot(10, this, [=]() {
		if (CONFIG->m_strVideo.isEmpty()) 
		{
			emit NoVideo();			//上次无视频播放显示主窗口
		}
		else 
		{
			SetVolume();
			ReplaceDesk();
			emit TrayShow();		//直接播放上次视频并直接显示托盘
		}
	});
}

DeskVideo::~DeskVideo() 
{
	delete m_pPlayer;
	m_pPlayDll->unload();
	delete m_pPlayDll;
	delete m_pDeskWnd;
}

bool DeskVideo::ReplaceDesk() 
{
	if (CONFIG->m_strVideo.isEmpty()) 
	{
		m_pPlayer->StopPlay();
		m_pDeskWnd->hide();
		return false;
	}
	else 
	{
		QFile video(CONFIG->m_strVideo);
		if (!video.exists()) 
		{
			QWidget wid;
			wid.setWindowIcon(QIcon(icoPath));
			wid.hide();
			QMessageBox::warning(&wid, msgBoxTitle, fileNoFind);
			return false;
		}
		m_pPlayer->SetVideo(CONFIG->m_strVideo);
		m_pPlayer->PlayVideo();
		m_pDeskWnd->show();
		emit LoadFinish();
		return true;
	}
}

void DeskVideo::SetVolume() 
{
	m_pPlayer->SetVolume(CONFIG->m_nLVolume);
}

void DeskVideo::StopOrStart(int state) 
{
	if (state) 
	{
		m_pPlayer->PlayVideo();
	}
	else
	{//暂停
		m_pPlayer->PauseVideo();
	}
}