#pragma once
//设置主窗口为橡皮筋式窗口
#include<QObject>

class Elastic :public QObject 
{
	Q_OBJECT

public:
    Elastic(QWidget* parent);
    ~Elastic();
    void setTitleH(int nHight);                          //设置顶栏高度高


protected:
    QWidget* m_pSubstituteWnd;                          //替身窗口
    QWidget* m_pManagedWnd;                             //要处理事件的窗口
    /* state说明:
    *xxxx 1代表在此位置，
    * 1.右 2.左 3.下 4.上
    * 5.鼠标左键按下
    * 6.鼠标能带动窗口移动
    * 7.替身窗口显示
    */
    uchar m_nState;                                   //鼠标对于窗口的位置
    QPoint m_relativePoint;                           //鼠标对于窗口位置
    QPoint m_absolutePoint;                           //鼠标对于屏幕的位置
    QPoint m_LTWndPoint;                              //窗口左上角对于桌面的坐标
    int m_nTitleHeight;                               //标题栏高度

    void InitSubstituteWnd();                       //初始化替身
    inline bool IsInTop();
    inline bool IsInLeft();
    inline bool IsInBottom();
    inline bool IsInRight();
    inline bool IsNullPosition();
    inline bool IsMouseLDown();
    inline bool IsMouseMove();
    inline bool IsSubWndShow();
    inline void SetTop(bool bInTop);
    inline void SetLeft(bool bInLeft);
    inline void SetBottom(bool bInBottom);
    inline void SetRight(bool bINRight);
    inline void SetMouseLState(bool bDown);
    inline void SetMouseMoveState(bool bMove);
    inline void SetSubWndState(bool bShow);
    void UpMouseIco();                              //更新鼠标图标
    void IsEdge(QPoint mousePoint, QRect WndRect);  //判断鼠标是否靠窗口边上停靠
    void UpWndSize(QPoint point);                   //根据拖拽值更新窗口大小      
    void MouseHoverEventHandler(QHoverEvent* e);    //鼠标停靠窗口
    void MouseMoveEventHandler(QMouseEvent* e);     //鼠标移动
    void MousePressEventHandler(QMouseEvent* e);    //鼠标被按下
    void MouseReleaseEventHandler(QMouseEvent* e);  //鼠标被松开
    bool eventFilter(QObject* obj, QEvent* e);      //事件过滤器
    
};