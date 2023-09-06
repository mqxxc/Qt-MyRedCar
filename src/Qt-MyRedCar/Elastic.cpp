#include<QWidget>               //基本窗口类
#include<QEvent>                //qt事件类
#include<QMouseEvent>           //移动事件类
#define Elastic_H
#include"macro.h"
#include"Elastic.h"

Elastic::Elastic(QWidget* parent) 
{
    InitSubstituteWnd();   //初始化替身窗口样式
    //初始化变量
    m_nState = 0;
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
    QPoint temp_point;

    if (m_nState & 15)
    {
        if (IsInTop())
        {//含有向上拉伸的向量
            temp_point = SubstituteWndRect.topLeft();
            if (SubstituteWndRect.bottomRight().y() - p.y() > minh)
            {
                temp_point.setY(p.y());
            }
            else
            {
                temp_point.setY(SubstituteWndRect.bottomRight().y() - minh);
            }
            SubstituteWndRect.setTopLeft(temp_point);
        }
        if (IsInBottom())
        {//含有向下拉伸的向量
            temp_point = SubstituteWndRect.bottomLeft();
            if (p.y() - SubstituteWndRect.topRight().y() > minh)
            {
                temp_point.setY(p.y());
            }
            else
            {
                temp_point.setY(SubstituteWndRect.bottomRight().y() + minh);
            }
            SubstituteWndRect.setBottomLeft(temp_point);
        }
        if (IsInLeft())
        {//含有向左拉伸的向量
            temp_point = SubstituteWndRect.topLeft();
            if (SubstituteWndRect.bottomRight().x() - p.x() > minw)
            {
                temp_point.setX(p.x());
            }
            else
            {
                temp_point.setX(SubstituteWndRect.bottomRight().x() - minw);
            }
            SubstituteWndRect.setTopLeft(temp_point);
        }
        if (IsInRight())
        {//含有向下拉伸的向量
            temp_point = SubstituteWndRect.topRight();
            if (p.x() - SubstituteWndRect.bottomLeft().x() > minw)
            {
                temp_point.setX(p.x());
            }
            else
            {
                temp_point.setX(SubstituteWndRect.bottomLeft().x() + minw);
            }
            SubstituteWndRect.setTopRight(temp_point);
        }
    }
    m_pSubstituteWnd->setGeometry(SubstituteWndRect);
}

void Elastic::MouseHoverEventHandler(QHoverEvent* e) 
{
    if(!IsMouseLDown())
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
        {          //判断鼠标是否位于标题栏
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
        if (m_LTWndPoint.y() < m_nTitleHeight) 
        {//鼠标在标题栏处按下
            SetSubWndState(true);
            SetMouseMoveState(IsNullPosition());
        }
        if (!IsNullPosition())
        {//含有任意拉伸的向量
            SetSubWndState(true);
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

void Elastic::InitSubstituteWnd()
{
    m_pSubstituteWnd = new QWidget;
    //设置为无边框窗口
    m_pSubstituteWnd->setWindowFlags(Qt::FramelessWindowHint| Qt::Tool);
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
    return !(m_nState ^ 15);
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
        m_nState |=  8;
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
        m_nState |=  2;
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
        m_nState |=  4;
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
        m_nState |=  1;
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