#pragma once
#include <QWidget>
#include <QVector>
class QMenu;
class QEventLoop;
class UnitMsgs;
namespace Ui { class Unit; }
				

class Unit:public QWidget
{
	Q_OBJECT

public:
	Unit(int id, QWidget* parent = Q_NULLPTR);
	~Unit();
	static void setScaling(float scaling);		//设置窗口缩放比例
	static int GetWnd_W();						//获取窗口大小
	static ushort CurrentID();					//获取当前选中的单元id号
	static void SetDatas(UnitMsgs* handle);		//设置数据源
	static void ReleaseAll();					//取消选择所有控件


private:
	Ui::Unit* m_pUi;							//ui界面
	QMenu* m_pMenu_R;							//右键菜单
	QVector<QAction*> m_arrMenu;				//菜单项
	QEventLoop* input_loop;						//重命名文本输入框的事件循环
	int m_nID;									//资源信息ID
	static ushort m_nWndW;						//窗口宽
	static ushort m_nSelectID;					//被选中的单元的id
	static UnitMsgs* m_pDataHandle;				//数据源

	void InitUi();								//初始化ui界面
	void InitMember();							//初始化变量
	void InitRMenu();							//初始化右键菜单
	void SetWndW();								//设置窗口的宽
	void OnSelect();							//被选中
	void RenameFinish();						//重命名菜单操作完成
	void UpdateUnitPath(QString name);			//修改单元名字以及图片路径
	void OnRelease();							//取消选中


signals:
	void UpdateUnits(int oldId);				//请求更新指定id号的单元状态
	void RenameSig(int id, QString name);		//重命名信号
	void DeleteSig(int id);						//删除信号


private slots:
	void mousePressEvent(QMouseEvent* ev);				//鼠标按键消息
	bool eventFilter(QObject* object, QEvent* event);   //子控件事件监听器
	//右键菜单处理
	/*dxg以后用windows钩子解决重命名输入框问题*/
	void MenuRename(bool b);					//重命名
	void MenuSetLove(bool b);					//是否喜欢
	void MenuDelete(bool b);					//删除
};