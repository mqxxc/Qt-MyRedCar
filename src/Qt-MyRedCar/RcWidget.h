#pragma once
//资源预览窗口
#include <QWidget>
#include <QVector>
class Unit;
class UnitMsgs;

class RcWidget :public QWidget 
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
	void SetScaling(float fScaling);				//设置缩放比例
	bool MoveFile(QString path);					//DXG改为拷贝视频文件而非移动
	void Refresh();									//刷新资源单元为全显示


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
	*从右边,第一位管理是否刷新，第二为判断正序或者倒序，第三位--第六位管理排序依据
	*1.	为1则刷新单元
	*2.	为1则正序，为0则倒序
	*/

	//尝试初始化资源
	void InitRc();				
	//子控件相关操作
	void InitMember();					//初始化成员变量
	void InitUnitSize();				//初始化图像单元大小
	void InitUnit(int i);				//从i开始初始化单元
	void RefreshUnit();					//刷新所有单元
	//其他辅助函数
	void ShowUnit(int sum);				//辅助放置单元
	void AddUnit();						//添加unit单元
	void ReckonNext();					//计算下一个单元的位置
	inline void SetRefreshbit(bool bRefresh);	//设置刷新位
	inline bool IsRefresh();					//是否刷新
	inline bool IsNormal();						//是正序
	inline void SetOrder(bool bNormal);			//设置正逆序
	

signals:
	void SelectUnit(QString path);				//选中单元信息
	void IniFinish();							//初始化完成


public slots:
	void Release(int id);						//选中单元取消选择其他单元
	void fileRename(int id,QString name);		//文件重命名
	void SearchUnits(QString name);				//筛选单元
	void ChangeCondition(SortCriteria condition);//修改单元排序的条件
	void ChangeOrder(bool state);				//改变单元的顺序


private slots:
	void resizeEvent(QResizeEvent* e);			//大小改变事件
	void mousePressEvent(QMouseEvent* event);	//鼠标点击事件
};