#pragma once
//资源预览窗口
#include <QFrame>
#include <QVector>
class Unit;
class UnitMsgs;

class RcWidget :public QFrame
{
	Q_OBJECT

public:
	enum class SortCriteria
	{
		onoe = 0,			//默认
		name = 1,			//名字
		ImportTime = 2,		//导入时间
		lastTime = 3,		//最后使用时间
		love = 4			//喜欢
	};

	RcWidget(QWidget* parent = Q_NULLPTR);
	~RcWidget();
	bool AddUnit(QString path);					//DXG改为拷贝视频文件而非移动
	void Refresh();								//刷新资源单元为全显示
	void ChangeCondition(SortCriteria condition);//修改单元排序的条件
	void IniUnits();							//初始化所有单元
	void SearchUnits(QString name);				//筛选单元
	void ChangeOrder(bool state);				//改变单元的顺序


private:
	//窗口信息集合
	struct WndMsg
	{
		int m_nSpaceW;			//间隙宽
		int m_nUnitW;			//单元宽
		int m_nRowN;			//一行存放单元个数
		int m_nOffset;			//两边偏移量
	};
	UnitMsgs* m_pRcs;					//资源文件集
	QVector<Unit*> m_arrUnits;			//图像单元集
	WndMsg m_UnitMsg;					//单元相关信息
	int m_nLastUnitX;					//最后的单元X位置
	int m_nLastUnitY;					//最后的单元Y位置
	QList<ushort> m_listIDList;			//单元显示顺序
	SortCriteria m_sortCriteria;		//排序条件
	int m_nWndW;			//窗口宽
	int m_nWndH;			//窗口高
	uchar m_nControlSig;				//控制信号
	/*    
	*从右边,第一位管理是否刷新，第二为判断正序或者倒序
	*1.	
	*2.	为1则正序，为0则倒序
	*/

	//子控件相关操作
	inline void InitMember();			//初始化成员变量
	void ReUnitSize();					//重新设置图像单元大小
	void CreateUnit();					//根据最后创建UnitWnd
	void RefreshUnit();					//刷新所有单元
	//其他辅助函数
	void ShowUnit(int sum);				//辅助放置单元
	void ReckonNext();					//计算下一个单元的位置
	inline bool IsNormal();				//是正序
	inline void SetOrder(bool bNormal);	//设置排列顺序
	void resizeEvent(QResizeEvent* e);			//大小改变事件
	void mousePressEvent(QMouseEvent* event);	//鼠标点击事件
	

signals:
	void SelectUnitSig();				//选中单元信息
	void IniFinish();					//初始化完成


private slots:
	void SelectUnit(int nCurrentID);	//选中单元取消选择其他单元
};