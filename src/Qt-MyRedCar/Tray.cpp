#include<QSystemTrayIcon>
#include<QMenu>
#include<QSlider>
#include<QTimer>
#include<QLabel>
#include<QEventLoop>
#include<QEvent>
#define TRAY_H
#include"macro.h"
#include"About.h"
#include"Tray.h"
#include "Config.h"
#include "../three/YQTools_Qt/YQTools_Qt.h"
#pragma execution_character_set("utf-8")

Tray::Tray(QObject* parent) : QObject(parent)
{
	//初始化变量
    m_pTrayIco  = new QSystemTrayIcon();
    m_pMenu = new QMenu();
    volumScr = new QWidget;
    volumeico = new QLabel(volumScr);
    scr = new QSlider(volumScr);
    m_bVIcoState = true;
    loop = new QEventLoop(volumScr);
    m_nOldVolume = 100;
    init();
    connects();
}

Tray::~Tray()
{
    delete loop;
    delete volumScr;
    for (QList<QAction*>::iterator it = m_listActions.begin(); it < m_listActions.end(); ++it)
    {
        delete (*it);
    }
    m_listActions.clear();
    delete m_pMenu;
    hide();
    delete m_pTrayIco;
}

void Tray::init()
{
    InitMenus();
    InitTray();
    initScr();
}

void Tray::InitTray()
{
    m_pTrayIco->setIcon(QIcon(YQTools_Qt::ToAbsolutePath(icoPath)));  //设定托盘图标
    m_pTrayIco->setToolTip(TrayTitle);                         //提示文字
    m_pTrayIco->setContextMenu(m_pMenu);
    m_pTrayIco->hide();
}

void Tray::InitMenus()
{
    QAction* action = new QAction(menu1);
    action->setObjectName("open");
    m_mapMenuAction["open"] = &Tray::MenuOpen;
    m_listActions.append(action);
    action = new QAction(menu2);
    action->setObjectName("volume");
    m_mapMenuAction["volume"] = &Tray::MenuVolumeChange;
    action->setData(1);
    m_listActions.append(action);
    action = new QAction(menu3_1);
    action->setObjectName("video");
    m_mapMenuAction["video"] = &Tray::MenuVideoStateChange;
    action->setData(1);
    m_listActions.append(action);
    action = new QAction(menu4);
    action->setObjectName("about");
    m_mapMenuAction["about"] = &Tray::MenuAbout;
    m_listActions.append(action);
    action = new QAction(menu5);
    action->setObjectName("exit");
    m_mapMenuAction["exit"] = &Tray::MenuExit;
    m_listActions.append(action);
    for (QList<QAction*>::iterator it = m_listActions.begin(); it != m_listActions.end(); ++it)
    {
        m_pMenu->addAction((*it));
        connect((*it), SIGNAL(triggered(bool)), this, SLOT(MenuAction(bool)));
    }
}

void Tray::initScr()
{
    volumScr->setFixedSize(40, 150);
    QPoint workSize = CONFIG->GetWorkDesktopPoint_RB();
    volumScr->move((workSize.x() - volumScr->width()),
        (workSize.y() - volumScr->height()));
    volumScr->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    volumScr->setStyleSheet("background-color: rgb(0, 0, 0);");

    scr->setGeometry(10, 10, 20, 100);
    scr->setMaximum(100);
    scr->setMinimum(0);
    scr->setPageStep(10);
    scr->setValue(100);
    scr->setTracking(false);

    volumeico->setGeometry(10, 120, 20, 20);
    volumScr->hide();
}

void Tray::connects()
{
    connect(scr, &QSlider::valueChanged,this,&Tray::setVolumFish);
    connect(m_pTrayIco, &QSystemTrayIcon::activated, this, &Tray::eventHandler);
}

void Tray::setVolumFish()
{
    CONFIG->m_nLVolume = scr->value();
    m_bVIcoState = CONFIG->m_nLVolume != 0;
    ChangeScrIco();
    emit DeskVolumeChange();
}

void Tray::ChangeScrIco()
{
    QPixmap pixmap;
    if (m_bVIcoState) 
    {
        pixmap.load(YQTools_Qt::ToAbsolutePath(volumeIco));
        m_bVIcoState = true;
    }
    else 
    {
        pixmap.load(YQTools_Qt::ToAbsolutePath(muteIco));
        m_bVIcoState = false;
    }
    int w = volumeico->geometry().width();
    volumeico->setPixmap(pixmap.scaled(w, w, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void Tray::show()
{
    m_pTrayIco->show();
}

void Tray::hide()
{
    m_pTrayIco->hide();
}

void Tray::MenuVolumeChange(bool bCheckState)
{
    Q_UNUSED(bCheckState)
    ChangeScrIco();
    volumeico->installEventFilter(this);
    QTimer::singleShot(10, this, [=]() {
        //延时弹出否则弹出后直接关闭
        volumScr->show();
        });
    loop->exec();
}

void Tray::MenuVideoStateChange(bool bCheckState)
{
    Q_UNUSED(bCheckState)
    QAction* temp = m_listActions.at(2);
    if (temp->data() == 1)
    {
        emit DeskStateChange(0);//暂停播放
        temp->setText(menu3_0);
        temp->setData(0);
    }  
    else 
    {
        emit DeskStateChange(1);//继续
        temp->setText(menu3_1);
        temp->setData(1);
    }
}

void Tray::MenuAbout(bool bCheckState)
{
    Q_UNUSED(bCheckState)
    About about;
    about.exec();
}

void Tray::MenuExit(bool bCheckState)
{
    Q_UNUSED(bCheckState)
    hide();
    emit MainAppExit();
}

void Tray::SetVolume()
{
    show();
    m_bVIcoState = CONFIG->m_nLVolume != 0;
    scr->setValue(CONFIG->m_nLVolume);
}

void Tray::SetState()
{
    QAction* temp = m_listActions.at(2);
    temp->setText(menu3_1);
    temp->setData(1);
}

void Tray::eventHandler(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) 
    {
        MenuOpen(true);
    }
}

bool Tray::eventFilter(QObject* object, QEvent* event)
{
    if (object == volumeico) 
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            m_bVIcoState = !m_bVIcoState;
            if (m_bVIcoState) 
            {
                scr->setValue(m_nOldVolume);
                CONFIG->m_nLVolume = m_nOldVolume;
                emit DeskVolumeChange();
            }
            else 
            {
                m_nOldVolume = scr->value();
                scr->setValue(0);
                CONFIG->m_nLVolume = 0;
                emit DeskVolumeChange();
            }
            ChangeScrIco();
        return true;
        }
    }
    if (event->type() == QEvent::MouseButtonPress) 
    {        //点击任意处关闭音量调节
        QObject* op = object;
        volumScr->hide();
        volumeico->removeEventFilter(this);             //关闭事件过滤
        loop->exit();
        return true;
    }
    return QObject::eventFilter(object, event);
}

void Tray::MenuAction(bool bCheckState)
{
    QMap<QString, void (Tray::*)(bool)>::iterator it = m_mapMenuAction.find(sender()->objectName());
    if (it == m_mapMenuAction.end())
    {
        return;
    }
    (this->**it)(bCheckState);
}

void Tray::MenuOpen(bool bCheckState)
{
    Q_UNUSED(bCheckState)
    emit ShowMainWnd();
    hide();
}