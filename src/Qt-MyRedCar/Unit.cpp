#include <QMouseEvent>
#include <QMenu>
#define Unit_H
#include "macro.h"	
#include "Config.h"
#include "ui_unit.h"
#include "Unit.h"
#include "UnitMsgs.h"
#pragma execution_character_set("utf-8")	

Unit::Unit(int id, QWidget* parent) : QWidget(parent)
{
    //初始化基本变量
    m_pUi = new Ui::Unit;
    m_pUi->setupUi(this);
    this->m_nID = id;
    
    //其他初始化
    InitMember();                       //初始化其他变量
    SetWndW();                          //初始化窗口大小
    InitUi();                           //初始化界面属性
    InitRMenu();                        //初始化右键菜单
}

void Unit::SetWndW()
{
    resize(m_nWndW, m_nWndW);
    m_pUi->ico->resize(m_nWndW, m_nWndW);
    int texth = m_nWndW / 4 * 3;
    m_pUi->text->setGeometry(0, texth, m_nWndW, m_nWndW-texth);
    m_pUi->input->setGeometry(0, texth, m_nWndW, m_nWndW - texth);
}

void Unit::InitRMenu()
{
    //菜单列表
    m_arrMenu[0]=new QAction(menuRN, this);
    if (m_pDataHandle->GetData(m_nID)->IsLove()) 
    {
        m_arrMenu[1] = new QAction(menuNL, this);
        m_arrMenu[1]->setData(1);
    }
    else
    {
        m_arrMenu[1] = new QAction(menuIL, this);
        m_arrMenu[1]->setData(0);
    }
    m_arrMenu[2]=new QAction(menuDe, this);
    for (auto it : m_arrMenu)
    {
        m_pMenu_R->addAction(it);
    }
    connect(m_arrMenu[0], SIGNAL(triggered(bool)), this, SLOT(MenuRename(bool)));
    connect(m_arrMenu[1], SIGNAL(triggered(bool)), this, SLOT(MenuSetLove(bool)));
    connect(m_arrMenu[2], SIGNAL(triggered(bool)), this, SLOT(menuDelete(bool)));
}

void Unit::InitUi() 
{
//初始化窗体
    this->setWindowFlags(Qt::FramelessWindowHint);      //设置为无边框窗口

//初始化文本框界面
    m_pUi->text->setTextColor(QColor(255, 255, 255));                   //白色背景
    m_pUi->text->setContextMenuPolicy(Qt::NoContextMenu);               //取消右键菜单
    m_pUi->text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //无滑块

//设置重命名界面
    m_pUi->input->hide();                                               //初始状态为隐藏
    m_pUi->input->setContextMenuPolicy(Qt::NoContextMenu);              //输入框无右键菜单
    m_pUi->input->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);  //输入框窗口标记

//设置文本图标
    m_pUi->text->setText(m_pDataHandle->GetData(m_nID)->GetName());
    QPixmap pixmapPic(m_pDataHandle->GetData(m_nID)->GetPhotoPath());
    QPixmap pixmapPicFit = pixmapPic.scaled(this->width(), this->width(),
        Qt::IgnoreAspectRatio);
    m_pUi->ico->setPixmap(pixmapPicFit);

//其他默认操作
    OnRelease();                                //默认不选中
}

void Unit::InitMember()
{
    m_pMenu_R = new QMenu(this);
    m_arrMenu = QVector<QAction*>(3);
    input_loop = new QEventLoop(m_pUi->input);
}

Unit::~Unit()
{
    for (int i = 0; i < m_arrMenu.size(); ++i)
    {
        delete m_arrMenu[i];
    }
    delete m_pMenu_R;

    if (input_loop != nullptr)
    {
        delete input_loop;
    }
    this->disconnect();
    delete m_pUi;
}

void Unit::setScaling(float appScaling)
{
    m_nWndW = initw / appScaling;
}

int Unit::GetWnd_W()
{
    return m_nWndW;
}

void Unit::OnSelect()
{
    m_pUi->ico->setStyleSheet("border:4px solid rgb(0, 255, 255);"
        " border-radius:25px;");
}

void Unit::OnRelease()
{
    m_pUi->ico->setStyleSheet("border:2px solid rgb(240, 242, 245);"
        " border-radius:25px; ");
}

ushort Unit::CurrentID()
{
    return m_nSelectID;
}

void Unit::SetDatas(UnitMsgs* handle)
{
    m_pDataHandle = handle;
}

void Unit::ReleaseAll()
{
}

void Unit::RenameFinish()
{
    input_loop->exit();
    QString name = m_pUi->input->toPlainText();
    if (!name.isEmpty()) {
        if (name.compare(m_pDataHandle->GetData(m_nID)->GetName())!=0)
        {
            m_pUi->text->setText(name);
            emit RenameSig(m_nID, name);
            UpdateUnitPath(name);
        }
    }
    m_pUi->input->hide();
    m_pUi->input->removeEventFilter(this);
}

void Unit::UpdateUnitPath(QString name)
{
    m_pDataHandle->GetData(m_nID)->SetName(name);
}

void Unit::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::RightButton) 
    {
        m_pMenu_R->move(ev->globalPos());
        m_pMenu_R->show();
        return;
    }
    if (ev->button() == Qt::LeftButton) 
    {
        int oldId = m_nSelectID;
        m_nSelectID = m_nID;
        emit UpdateUnits(oldId);
        OnSelect();
        return;
    }
}

bool Unit::eventFilter(QObject* object, QEvent* event) 
{
    if (object == m_pUi->input) 
    {
        if (event->type() == QEvent::KeyPress) 
        {            //回车键完成重命名
            QKeyEvent* e = static_cast <QKeyEvent*> (event);
            if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) 
            {
                RenameFinish();
                return true;
            }
        }
    }
    if (event->type() == QEvent::MouseButtonPress) 
    {        //点击任意处完成重命名
        RenameFinish();
        return true;
    }
    return QObject::eventFilter(object, event);
}

void Unit::MenuRename(bool b)
{
    Q_UNUSED(b)
    m_pUi->input->installEventFilter(this);                             //安装事件过滤器
    //重命名
    m_pUi->input->setText(m_pUi->text->toPlainText());
    m_pUi->input->move(m_pUi->text->mapToGlobal(QPoint(0, 0)));
    m_pUi->input->show();
    m_pUi->input->activateWindow();                         //设置为顶层窗口
    input_loop->exec();
}

void Unit::MenuSetLove(bool b)
{
    Q_UNUSED(b)
    //是否喜欢
    if (m_arrMenu[1]->data() == 0) 
    {
        m_pDataHandle->GetData(m_nID)->SetLove(true);
        m_arrMenu[1]->setText(menuNL);
        m_arrMenu[1]->setData(1);
    }
    else 
    {
        m_pDataHandle->GetData(m_nID)->SetLove(false);
        m_arrMenu[1]->setText(menuIL);
        m_arrMenu[1]->setData(0);
    }
}

void Unit::MenuDelete(bool b)
{
    Q_UNUSED(b)
    close();
    emit DeleteSig(m_nID);
}

ushort Unit::m_nWndW = 120;
ushort Unit::m_nSelectID = 0;
UnitMsgs* Unit::m_pDataHandle = nullptr;