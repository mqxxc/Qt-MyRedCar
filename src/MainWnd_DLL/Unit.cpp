#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include <QtextBrowser>
#include <QEventLoop>
#define Unit_H
#include "macro.h"	
#include "Config.h"
#include "Unit.h"
#include "UnitMsgs.h"
#include "../three/YQTools_Qt/YQTools_Qt.h"
#pragma execution_character_set("utf-8")	

Unit::Unit(int id, QWidget* parent) : QWidget(parent)
{
    m_nID = id;
    InitUi();                           //初始化界面属性
}

void Unit::InitRMenu()
{
    if (sm_pDataHandle->GetData(m_nID)->IsLove())
    {
        sm_arrRMenuItems[1]->setText(menuNL);
        sm_arrRMenuItems[1]->setData(1);
    }
    else
    {
        sm_arrRMenuItems[1]->setText(menuIL);
        sm_arrRMenuItems[1]->setData(0);
    }
    connect(sm_RMenu, &QMenu::triggered, this, &Unit::MenuEven);
    connect(sm_RMenu, &QMenu::aboutToHide, this, &Unit::MenuHide);
}

inline void Unit::InitUi() 
{
    resize(sm_nWndW, sm_nWndW);
    m_pLabelIco = new QLabel(this);
    m_pLabelIco->resize(sm_nWndW, sm_nWndW);
    m_pTBroName = new QTextBrowser(this);
    int texth = sm_nWndW / 4 * 3;
    m_pTBroName->setGeometry(0, texth, sm_nWndW, sm_nWndW - texth);
    setWindowFlags(Qt::FramelessWindowHint);      //设置为无边框窗口

//初始化文本框界面
    m_pTBroName->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_pTBroName->setContextMenuPolicy(Qt::NoContextMenu);               //取消右键菜单
    m_pTBroName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //无滑块
    m_pTBroName->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTBroName->document()->setMaximumBlockCount(1);
    m_pTBroName->setLineWrapMode(QTextBrowser::NoWrap);
    m_pTBroName->setStyleSheet("background-color:rgba(0, 0, 0, 0.5);font:15pt \"Agency FB\";color:rgb(255, 255, 255);border - style:none;");

//设置文本图标
    if (sm_pDataHandle != nullptr)
    {
        m_pTBroName->setText(sm_pDataHandle->GetData(m_nID)->GetName());
        m_pLabelIco->setPixmap(QPixmap(sm_pDataHandle->GetData(m_nID)->GetPhotoPath()).scaled(width(), width(),
            Qt::IgnoreAspectRatio));
    }
   
//其他默认操作
    OnRelease();        //默认不选中
}

Unit::~Unit()
{
    disconnect();
}

int Unit::GetWnd_Width()
{
    return sm_nWndW;
}

void Unit::OnSelect()
{
    if (sm_pSelectUnit != nullptr)
    {
        sm_pSelectUnit->OnRelease();
    }
    sm_pSelectUnit = this;
    m_pLabelIco->setStyleSheet("border:2px solid rgb(0, 255, 255);");
    emit SelectUnits(m_nID);
}

void Unit::OnRelease()
{
    m_pLabelIco->setStyleSheet("border:2px solid rgb(255, 255, 255);");
}

Unit* Unit::SelectUnit()
{
    return sm_pSelectUnit;
}

void Unit::SetDatas(UnitMsgs* handle)
{
    sm_pDataHandle = handle;
}

void Unit::DeselectAll()
{
    if (sm_pSelectUnit == nullptr)
    {
        return;
    }
    sm_pSelectUnit->OnRelease();
    sm_pSelectUnit = nullptr;
}

void Unit::RenameFinish()
{
    QString name = sm_editor->toPlainText();
    if (!name.isEmpty()) {
        if (name.compare(sm_pDataHandle->GetData(m_nID)->GetName())!=0)
        {//与原来的名字不一样
            if (sm_pDataHandle->GetData(m_nID)->SetName(name))
            {
                m_pTBroName->setText(name);
            }
            else
            {
                QMessageBox::warning(this, "提示", "重命名失败！");
            }
        }
    }
    sm_editorLoop->exit();
    sm_editor->setParent(this);
    sm_editor->hide();
    sm_editor->removeEventFilter(this);
}

void Unit::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::RightButton) 
    {
        sm_RMenu->move(ev->globalPos());
        sm_RMenu->show();
        InitRMenu();
        return;
    }
    if (ev->button() == Qt::LeftButton) 
    {
        OnSelect();
        return;
    }
}

bool Unit::eventFilter(QObject* object, QEvent* event) 
{
    if (object->objectName() == "editor")
    {
        if (event->type() == QEvent::KeyPress) 
        {   //回车键完成重命名
            QKeyEvent* e = static_cast <QKeyEvent*> (event);
            if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) 
            {
                RenameFinish();
                return true;
            }
        }
    }
    if (event->type() == QEvent::MouseButtonPress) 
    {   //点击任意处完成重命名
        RenameFinish();
        return true;
    }
    return false;
}

void Unit::MenuRename()
{
    sm_editor->installEventFilter(this);                 //安装事件过滤器
    //重命名
    sm_editor->setParent(this);
    sm_editor->setGeometry(m_pTBroName->geometry());
    sm_editor->setText(m_pTBroName->toPlainText());
    sm_editor->activateWindow();                         //设置为顶层窗口
    sm_editor->show();
    sm_editorLoop->exec();
}

void Unit::MenuSetLove()
{//是否喜欢
    sm_pDataHandle->GetData(m_nID)->SetLove(sm_arrRMenuItems[1]->data() == 0);
}

void Unit::MenuDelete()
{
    close();
    sm_pDataHandle->DeleteData(m_nID);
    emit DeleteSig(m_nID);
}

void Unit::MenuHide()
{
    QTimer::singleShot(20, this, [=]() {
        sm_RMenu->disconnect();
        });
}

void Unit::DesStaticRes()
{
    for (auto& it : sm_arrRMenuItems)
    {
        delete it;
    }
    sm_arrRMenuItems.empty();

    YQTools_Qt::Delete_Ex(sm_RMenu);
    YQTools_Qt::Delete_Ex(sm_editor);
    YQTools_Qt::Delete_Ex(sm_editorLoop);
}

void Unit::CreateStaticRes()
{
    if (sm_editor == nullptr)
    {
        sm_editor = new QTextEdit;
        sm_editor->setObjectName("editor");
    }
   
    if (sm_editorLoop == nullptr)
    {
        sm_editorLoop = new QEventLoop;
    }
    
    sm_nWndW = YQTools_Qt::RelativeSize(UnitWidth);

    if (sm_RMenu == nullptr)
    {
        sm_RMenu = new QMenu;
    }

    if (sm_arrRMenuItems.isEmpty())
    {
        QAction* temp = new QAction(menuRN, sm_RMenu);
        temp->setObjectName("name");
        sm_arrRMenuItems.append(temp);

        temp = new QAction(menuIL, sm_RMenu);
        temp->setObjectName("lover");
        sm_arrRMenuItems.append(temp);

        temp = new QAction(menuDe, sm_RMenu);
        temp->setObjectName("delete");
        sm_arrRMenuItems.append(temp);

        for (auto& it : sm_arrRMenuItems)
        {
            sm_RMenu->addAction(it);
        }
    }
}

void Unit::MenuEven(QAction* pAction)
{
    QString strObject = pAction->objectName();
    if (strObject == "name")
    {
        MenuRename();
    }
    else if (strObject == "lover")
    {
        MenuSetLove();
    }
    else if(strObject == "delete")
    {
        MenuDelete();
    }
}

UnitMsgs* Unit::sm_pDataHandle = nullptr;
Unit* Unit::sm_pRightClickUnit = nullptr;
QMenu* Unit::sm_RMenu;
QVector<QAction*> Unit::sm_arrRMenuItems;
ushort Unit::sm_nWndW = UnitWidth;
Unit* Unit::sm_pSelectUnit = nullptr;
QTextEdit* Unit::sm_editor;
QEventLoop* Unit::sm_editorLoop = nullptr;