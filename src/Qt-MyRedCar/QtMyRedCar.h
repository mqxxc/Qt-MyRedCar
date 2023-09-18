#pragma once
//主窗口
#include<QWidget>
class VideoPalyer;
class Elastic;
class QLibrary;
class QLineEdit;
class QPushButton;
namespace Ui { class QtMyRedCar; }

class QtMyRedCar : public QWidget
{
    Q_OBJECT 

public:
    QtMyRedCar(QWidget* parent = nullptr);
    ~QtMyRedCar();
    void IniUnits();


private:
    Ui::QtMyRedCar* m_pUi;
    Elastic* m_pEvenRoot;      //窗口移动缩放管理者
    QRect m_leftTop;           //左上角坐标
    VideoPalyer* m_pPlayer;    //右上角视频播放器
    QLibrary* m_pDll;          //库实例
    QLineEdit* m_pInput;       //搜索输入框
    QPushButton* m_pButton;    //搜索按钮
    /*
    * 1：最大化最小化按钮的状态
    * 2：true--顺序，false--倒序
    */
    uchar m_nState;

    inline void Connects();              //初始化信号信号槽
    inline void InitMember();            //初始化变量
    inline void InitUi();                //初始化窗口Ui
    inline void InitSearch();            //初始化搜索栏            
    inline void InitVidewid();           
    inline void SetMaxButtonState(bool bIsOnMax);
    inline bool IsOnMaxButton();
    inline void SetOrderState(bool bNormal);
    inline bool IsNormalOrder();


signals:
    //title
    void WndToClear();                                  //窗口最小化到托盘消息
    //top
    void ChangeUnitOrder(bool state);                   //修改正序or倒叙
    //middle
    void search(QString name);                          //搜索信号
    //bottom
    void MainAppExit();                                 //主程序退出
    void upDesk();                                      //更换壁纸
    //other
    void LoadFinishSig();                               //主窗口加载完成信号

private slots:
    //title
    void on_maxsize_clicked();                          //最大化最小化按钮
    void on_off_clicked();                              //关闭按钮
    //top
    void on_filter_clicked();                           //筛选按钮
    void on_order_clicked();                            //正序or倒叙
    void on_setbox_clicked();                           //设置按钮
    //middle
    void LoadFinish();                                  //资源窗口加载完成
    void ChangeCondition(QString strCondition);         //改变排序条件
    //bottom
    void on_originality_clicked();                      //浏览创意工坊
    void on_about_clicked();                            //嘤起up
    void on_lod_clicked();                              //加载本地文件
    void on_Refresh_clicked();                          //刷新
    //right
    void on_deletebo_clicked();                         //移除壁纸
    void on_OK_clicked();                               //更换壁纸
    void on_endbo_clicked();                            //退出程序
    void previewVideo(QString path);                    //预览窗口播放视频
    bool eventFilter(QObject* obj, QEvent* even);       //事件过滤器
};