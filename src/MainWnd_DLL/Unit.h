#pragma once
#include <QWidget>
#include <QVector>
class QMenu;
class QEventLoop;
class UnitMsgs;
class QTextEdit;
class QLabel;
class QTextBrowser;

class Unit:public QWidget
{
	Q_OBJECT

public:
	static int GetWnd_Width();					//获取窗口大小
	static Unit* SelectUnit();					//获取当前选中的单元
	static void SetDatas(UnitMsgs* handle);		//设置数据源
	static void DeselectAll();					//取消选择全部
	static void CreateStaticRes();				//创建动态资源
	static void DesStaticRes();					//销毁静态资源
	Unit(int id, QWidget* parent = nullptr);
	~Unit();
	inline int GetID()
	{
		return m_nID;
	};
	

private:
	static UnitMsgs* sm_pDataHandle;			//数据源
	static Unit* sm_pRightClickUnit;			//右键单元
	static QMenu* sm_RMenu;						//右键菜单
	static QVector<QAction*> sm_arrRMenuItems;	//菜单项
	static ushort sm_nWndW;						//窗口宽
	static Unit* sm_pSelectUnit;				//被选中的单元
	static QTextEdit* sm_editor;				//重命名编辑器
	static QEventLoop* sm_editorLoop;			//重命名文本输入框的事件循环
	QLabel* m_pLabelIco;						//缩略预览图
	QTextBrowser* m_pTBroName;					//资源名字
	int m_nID;									//资源信息ID
	
	inline void InitUi();						//初始化ui界面
	void InitRMenu();							//初始化右键菜单
	void OnSelect();							//被选中
	void OnRelease();							//取消选中
	void RenameFinish();						//重命名菜单操作完成
	void mouseReleaseEvent(QMouseEvent* ev);	//鼠标释放事件
	bool eventFilter(QObject* object, QEvent* event);   //子控件事件监听器


signals:
	void SelectUnits(int nCurrentID);			//选中单元事件
	void DeleteSig(int nCurrentID);				//删除信号


private slots:
	/*dxg以后用windows钩子解决重命名输入框问题*/
	void MenuRename();					//重命名
	void MenuSetLove();					//是否喜欢
	void MenuDelete();					//删除
	void MenuHide();					//右键菜单隐藏事件
	void MenuEven(QAction* pAction);	//菜单项触发事件
};