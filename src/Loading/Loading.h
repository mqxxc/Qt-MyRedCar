#pragma once
//启动界面
#include<QGraphicsObject>
#include<QGraphicsView>
class QPropertyAnimation;
class QLabel;

class Item : public QGraphicsObject 
{
	Q_OBJECT

public:
	Item(QRect rect);
	~Item();
//设置私有变量
	void AppendPoxmap(QString path);


private:
	bool m_bFirst;									//第一次开始
	bool m_bReversal;								//是否翻转
	QVector<QPixmap*> m_arrPixmaps;					//图片列表
	QVector<QPixmap*> m_arrRePixmaps;				//反转图片列表
	ushort m_nCurrentIndex;							//当前显示的图片的下标
	QPropertyAnimation* m_pFrontAnimation;			//前半步动画类 
	QPropertyAnimation* m_pAfterAnimation;			//后半步动画类
	QRect m_Rect;									//图元绘制矩形

	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
		QWidget* widget);						//图元重绘函数

	QRectF boundingRect() const;


signals:
	void Next();								//下一个图元开始旋转


public slots:
	void AnimationStart();						//此图元开始旋转


private slots:
	void FrontAnimationFinished();		//前半段动画结束
	void AfterAnimationFinished();		//后半段动画结束
};


class View:public QGraphicsView 
{
	Q_OBJECT

public:
	View(QString strPath, QWidget* parent = nullptr);
	~View();
	void Strat();

	
private:
	QGraphicsScene* m_pScene;
	QList<Item*> m_listItems;
	QString m_strImgPath;

	inline void Init();
	inline void InitMember();
	inline void InitUi();
	inline void Connects();


private slots:
	void drawBackground(QPainter* painter, const QRectF& rect);		//绘制背景事件
	void wheelEvent(QWheelEvent* e);			//鼠标滚轮事件
};

class Loading :public QWidget 
{
	Q_OBJECT

public:
	Loading();
	~Loading();
	void StartLoad();


private:
	QWidget* m_labelMiddle;
	View* m_pView;
	ushort m_nTransparency;
	QString m_strPohotoPath;
	QPropertyAnimation* m_pAnimation;			//前半步动画类 

	inline QString GetImgPath();
	inline void IniAnimation();
	
private slots:
	void wheelEvent(QWheelEvent* e);
};