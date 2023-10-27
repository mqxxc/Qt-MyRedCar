#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#define Unit_H
#include "macro.h"	
#include "Config.h"
#include "ui_unit.h"
#include "Unit.h"
#include "UnitMsgs.h"
#include "Tools.h"
#pragma execution_character_set("utf-8")	

Unit::Unit(int id, QWidget* parent) : QWidget(parent)
{
    //初始化基本变量
    m_pUi = new Ui::Unit;
    m_pUi->setupUi(this);
    m_nID = id;
    
    //其他初始化
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
    m_pUi->ico->resize(sm_nWndW, sm_nWndW);
    int texth = sm_nWndW / 4 * 3;
    m_pUi->text->setGeometry(0, texth, sm_nWndW, sm_nWndW - texth);
    setWindowFlags(Qt::FramelessWindowHint);      //设置为无边框窗口

//初始化文本框界面
    m_pUi->text->setTextColor(QColor(255, 255, 255));                   //白色背景
    m_pUi->text->setContextMenuPolicy(Qt::NoContextMenu);               //取消右键菜单
    m_pUi->text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);    //无滑块
    m_pUi->text->document()->setMaximumBlockCount(1);
    m_pUi->text->setLineWrapMode(QTextBrowser::NoWrap);

////设置重命名界面
//    m_pUi->input->hide();                                               //初始状态为隐藏
//    m_pUi->input->setContextMenuPolicy(Qt::NoContextMenu);              //输入框无右键菜单
//    m_pUi->input->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);  //输入框窗口标记

//设置文本图标
    if (sm_pDataHandle != nullptr)
    {
        m_pUi->text->setText(sm_pDataHandle->GetData(m_nID)->GetName());
        m_pUi->ico->setPixmap(QPixmap(sm_pDataHandle->GetData(m_nID)->GetPhotoPath()).scaled(width(), width(),
            Qt::IgnoreAspectRatio));
    }
   
//其他默认操作
    OnRelease();                                //默认不选中
}

Unit::~Unit()
{
    disconnect();
    delete m_pUi;

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
    m_pUi->ico->setStyleSheet("border:2px solid rgb(0, 255, 255);");
    emit SelectUnits(m_nID);
}

void Unit::OnRelease()
{
    m_pUi->ico->setStyleSheet("border:2px solid rgb(255, 255, 255);");
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
                m_pUi->text->setText(name);
            }
            else
            {
                QMessageBox::warning(this, "提示", "重命名失败！");
            }
        }
    }
    sm_editorLoop.exit();
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
    sm_editor->setGeometry(m_pUi->text->geometry());
    sm_editor->setText(m_pUi->text->toPlainText());
    sm_editor->activateWindow();                         //设置为顶层窗口
    sm_editor->show();
    sm_editorLoop.exec();
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

    Tools::Delete_Ex(sm_RMenu);
    Tools::Delete_Ex(sm_editor);
}

void Unit::CreateStaticRes()
{
    if (sm_editor == nullptr)
    {
        sm_editor = new QTextEdit;
        sm_editor->setObjectName("editor");
    }
   
    sm_nWndW = UnitWidth / CONFIG->m_fScale;

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
QEventLoop  Unit::sm_editorLoop;