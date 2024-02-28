#include <QWidget>               //基本窗口类
#include <QHoverEvent>           //qt事件类
#include <QMouseEvent>           //鼠标事件
#define Elastic_H
#include"macro.h"
#include"Elastic.h"
#pragma execution_character_set("utf-8")

Elastic::Elastic(QWidget* parent) : QObject(parent)
{
    //初始化变量
    IniData();
    m_pManagedWnd = parent;
    //对要管理的窗口进行基本设置
    parent->installEventFilter(this);                        //安装事件过滤器
    parent->setAttribute(Qt::WA_Hover, true);                //开启悬停事件
}

Elastic::~Elastic()
{
    m_pManagedWnd->removeEventFilter(this);  //关闭事件过滤
    delete m_pSubstituteWnd;
}

void Elastic::setTitleH(int nHight)
{
    m_nTitleHeight = nHight;
}

void Elastic::UpMouseIco()
{
    switch (m_nState)
    {
    case 0:
    {
        m_pManagedWnd->setCursor(Qt::ArrowCursor);
    }break;
    case 8:
    case 4:
    {
        m_pManagedWnd->setCursor(Qt::SizeVerCursor);
    }break;
    case 2:
    case 1:
    {
        m_pManagedWnd->setCursor(Qt::SizeHorCursor);
    }break;
    case 10:
    case 5:
    {
        m_pManagedWnd->setCursor(Qt::SizeFDiagCursor);
    }break;
    case 9:
    case 6:
    {
        m_pManagedWnd->setCursor(Qt::SizeBDiagCursor);
    }break;
    }
}

void Elastic::IsEdge(QPoint mousePoint, QRect WndRect)
{
    SetTop(mousePoint.y() - WndRect.y() < mouse_sens);
    SetLeft(mousePoint.x() - WndRect.x() < mouse_sens);
    SetBottom(WndRect.bottom() - mousePoint.y() < mouse_sens);
    SetRight(WndRect.right() - mousePoint.x() < mouse_sens);
}

void Elastic::UpWndSize(QPoint p)
{
    int minw = m_pManagedWnd->minimumWidth();
    int minh = m_pManagedWnd->minimumHeight();
    QRect SubstituteWndRect = m_pSubstituteWnd->geometry();

    if (m_nState & 15)
    {
        if (IsInTop())
        {//鼠标位于上边缘
            SubstituteWndRect.setTop(qMin(SubstituteWndRect.bottom() - minh, p.y()));
        }
        else if (IsInBottom())
        {//鼠标位于下边缘
            SubstituteWndRect.setBottom(qMax(SubstituteWndRect.top() + minh, p.y()));
        }
        if (IsInLeft())
        {//鼠标位于左边缘
            SubstituteWndRect.setLeft(qMax(SubstituteWndRect.right() - minw, p.x()));
        }
        else if (IsInRight())
        {//鼠标位于右边缘
            SubstituteWndRect.setRight(qMax(SubstituteWndRect.left() + minw, p.x()));
        }
    }
    m_pSubstituteWnd->setGeometry(SubstituteWndRect);
}

void Elastic::MouseHoverEventHandler(QHoverEvent* e)
{
    if (!IsMouseLDown())
    {
        m_relativePoint = m_pManagedWnd->mapToGlobal(e->pos());
        IsEdge(m_relativePoint, m_pManagedWnd->frameGeometry());
        UpMouseIco();
    }
}

void Elastic::MouseMoveEventHandler(QMouseEvent* e)
{
    Q_UNUSED(e);

    if (IsMouseLDown())
    {
        m_absolutePoint = e->globalPos();
        if (IsMouseMove())
        { //判断鼠标是否位于标题栏
            QPoint tmp = m_absolutePoint - m_LTWndPoint;
            m_pSubstituteWnd->move(tmp);
        }
        else if (m_nState != 0)
        {
            UpWndSize(m_absolutePoint);
        }
    }
}

void Elastic::MousePressEventHandler(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_LTWndPoint = e->pos();
        if (!IsNullPosition())
        {//含有任意拉伸的向量
            SetSubWndState(true);
        }
        else if (m_LTWndPoint.y() < m_nTitleHeight)
        {//鼠标在标题栏处按下
            SetSubWndState(true);
            SetMouseMoveState(true);
        }
        if (IsSubWndShow())
        {//显示替身窗口
            m_pSubstituteWnd->setGeometry(m_pManagedWnd->geometry());
            m_pSubstituteWnd->show();
        }
        SetMouseLState(true);
    }
}

void Elastic::MouseReleaseEventHandler(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        SetMouseLState(false);
        SetMouseMoveState(false);
        if (IsSubWndShow())
        {       //替身窗口显示
            m_pManagedWnd->setGeometry(m_pSubstituteWnd->geometry());
            m_pSubstituteWnd->hide();
            SetSubWndState(false);
        }
    }
}

bool Elastic::eventFilter(QObject* obj, QEvent* e)
{
    switch (e->type())
    {
    case QEvent::HoverMove:
        MouseHoverEventHandler(static_cast<QHoverEvent*>(e));
        break;
    case QEvent::MouseMove:
        MouseMoveEventHandler(static_cast<QMouseEvent*>(e));
        break;
    case QEvent::MouseButtonPress:
        MousePressEventHandler(static_cast<QMouseEvent*>(e));
        break;
    case QEvent::MouseButtonRelease:
        MouseReleaseEventHandler(static_cast<QMouseEvent*>(e));
        break;
    default:
        return QObject::eventFilter(obj, e);
    }
    return true;
}

void Elastic::IniData()
{
    m_nState = 0;

    m_pSubstituteWnd = new QWidget;
    //设置为无边框窗口
    m_pSubstituteWnd->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    //设置背景颜色
    m_pSubstituteWnd->setStyleSheet("background-color:rgb(85, 255, 255);");
    //设置透明度
    m_pSubstituteWnd->setWindowOpacity(0.7);
}

inline bool Elastic::IsInTop()
{
    return m_nState & 8;
}

inline bool Elastic::IsInLeft()
{
    return m_nState & 2;
}

inline bool Elastic::IsInBottom()
{
    return m_nState & 4;
}

inline bool Elastic::IsInRight()
{
    return m_nState & 1;
}

inline bool Elastic::IsNullPosition()
{
    return !(m_nState & 15);
}

inline bool Elastic::IsMouseLDown()
{
    return m_nState & 16;
}

inline bool Elastic::IsMouseMove()
{
    return m_nState & 32;
}

inline bool Elastic::IsSubWndShow()
{
    return m_nState & 64;
}

inline void Elastic::SetTop(bool bInTop)
{
    if (bInTop)
    {
        m_nState |= 8;
    }
    else
    {
        m_nState &= StateMax ^ 8;
    }
}

inline void Elastic::SetLeft(bool bInLeft)
{
    if (bInLeft)
    {
        m_nState |= 2;
    }
    else
    {
        m_nState &= StateMax ^ 2;
    }
}

inline void Elastic::SetBottom(bool bInBottom)
{
    if (bInBottom)
    {
        m_nState |= 4;
    }
    else
    {
        m_nState &= StateMax ^ 4;
    }
}

inline void Elastic::SetRight(bool bINRight)
{
    if (bINRight)
    {
        m_nState |= 1;
    }
    else
    {
        m_nState &= StateMax ^ 1;
    }
}

inline void Elastic::SetMouseLState(bool bDown)
{
    if (bDown)
    {
        m_nState |= 16;
    }
    else
    {
        m_nState &= StateMax ^ 16;
    }
}

inline void Elastic::SetMouseMoveState(bool bMove)
{
    if (bMove)
    {
        m_nState |= 32;
    }
    else
    {
        m_nState &= StateMax ^ 32;
    }
}

inline void Elastic::SetSubWndState(bool bShow)
{
    if (bShow)
    {
        m_nState |= 64;
    }
    else
    {
        m_nState &= StateMax ^ 64;
    }
}