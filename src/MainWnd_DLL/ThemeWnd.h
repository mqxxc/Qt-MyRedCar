#pragma once
#include <QFrame>
#include <QMap>
#include "mainwnd_global.h"
class QHBoxLayout;
class QVBoxLayout;
class QLibrary;
class VideoPalyer;
class RcWidget;
class QLineEdit;
class QLabel;
class QPushButton;

class ThemeWnd : public QFrame
{
    Q_OBJECT
public:
	ThemeWnd(QWidget* parent = nullptr);
    virtual ~ThemeWnd();
    virtual void LoadingRc();       //加载资源
    virtual void ReleaseInstance(); //释放实例
    virtual void ConnectSig(const char* strSig, QObject* obj, const char* strSlot);


private:
    RcWidget* m_pRcWnd;        //资源窗口
    QLineEdit* m_pInput;       //搜索输入栏
    QPushButton* m_pButOrder;  //正序or倒叙
    QLabel* m_pLabVideoPreview;//视频预览窗口
    QRect m_leftTop;           //左上角坐标
    VideoPalyer* m_pPlayer;    //右上角视频播放器
    QLibrary* m_pDllPlayer;    //库实例
     /*
    * 1：最大化最小化按钮的状态
    * 2：true--顺序，false--倒序
    */
    uchar m_nState;

	inline void IniFace();			//初始化窗体
    inline QFrame* IniTitle();		//初始化标题栏
    inline QFrame* IniScreen();		//初始化筛选
    inline QFrame* IniMainBody();	//初始化主体
    inline QFrame* IniMainBody_LT();		//初始化主体窗口的左边上部分
    inline QFrame* IniMainBody_LB();		//初始化主体窗口的左边下部分
    inline QFrame* IniMainBody_R();			//初始化主体窗口的右边部分
    inline void IniData();                  //初始化数据
    inline void SetMaxButtonState(bool bIsOnMax);
    inline bool IsOnMaxButton();
    inline void SetOrderState(bool bNormal);
    inline bool IsNormalOrder();

signals:
    void WndClose();                            //窗口关闭
    void UnitsOrderChange(bool isDue);          //单元排列顺序发生改变
    void LoadFinishSig();                       //加载资源完成
    void UpdateDesVidoe(QString strVideoPath);  //更新桌面视频
    void MainAppExit();                         //退出程序
    void SearchUnit(QString name);              //搜索信号


private slots:
    //标题栏按钮
    void on_maxSize_clicked();                          //最大窗口化和复原
    //top
    void on_search_inputFinish();                       //搜索内容输入完成
    void on_filter_clicked();                           //筛选按钮
    void on_order_clicked();                            //正序or倒叙
    void on_setbox_clicked();                           //设置按钮
    //middle
    void ChangeCondition(QString strCondition);         //改变排序条件
    void LoadFinish();
    //bottom
    void on_originality_clicked();                      //浏览创意工坊
    void on_about_clicked();                            //嘤起up
    void on_lod_clicked();                              //加载本地文件
    void on_Refresh_clicked();                          //刷新
    //right
    void on_deletebo_clicked();                         //移除壁纸
    void on_OK_clicked();                               //更换壁纸
    void on_endbo_clicked();                            //退出程序
    void PreviewVideo();                                //预览窗口播放视频
};

extern "C" MAINWND_EXPORT ThemeWnd* GetThemeWnd(QWidget* parent);
extern "C" MAINWND_EXPORT void SetRunPath(QString strPath);
extern "C" MAINWND_EXPORT void SetScaling(float fScaling);