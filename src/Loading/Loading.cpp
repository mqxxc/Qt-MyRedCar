#include "Loading.h"
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsRotation>
#include <QTimer>
#include <QSplashScreen>
#include <QCoreApplication>
#pragma execution_character_set("utf-8")

//Item
Item::Item(QRect rect) 
{
    m_nCurrentIndex = 0;
    m_bReversal = false;
    m_bFirst = true;
    m_Rect = rect;
    //设置该item的变换
    QGraphicsRotation* m_itemRotation = new QGraphicsRotation(this);
    m_itemRotation->setAxis(Qt::YAxis); //沿Y轴旋转
    m_itemRotation->setOrigin(QVector3D((rect.x()+rect.width()/2), 0, 0)); //旋转基准位置为item的中心线
    this->setTransformations(QList<QGraphicsTransform*>() << m_itemRotation);

    //动画效果
    m_pFrontAnimation = new QPropertyAnimation(m_itemRotation, "angle");
    m_pFrontAnimation->setDuration(750);
    m_pFrontAnimation->setStartValue(0);
    m_pFrontAnimation->setEndValue(90);

    m_pAfterAnimation = new QPropertyAnimation(m_itemRotation, "angle");
    m_pAfterAnimation->setDuration(750);
    m_pAfterAnimation->setStartValue(90);
    m_pAfterAnimation->setEndValue(180);

    connect(m_pFrontAnimation, SIGNAL(finished()), this, SLOT(FrontAnimationFinished()));
    connect(m_pAfterAnimation, SIGNAL(finished()), this, SLOT(AfterAnimationFinished()));
}

Item::~Item()
{
    delete m_pFrontAnimation;
    delete m_pAfterAnimation;

    for (auto ti : m_arrPixmaps)
    {
        delete ti;
    }
    for (auto ti : m_arrRePixmaps)
    {
        delete ti;
    }
}

void Item::AppendPoxmap(QString strPath)
{
    QPixmap temp(strPath);
    QPixmap* pixmap = new QPixmap;
    *pixmap = temp.scaled(m_Rect.width(), m_Rect.height(), Qt::KeepAspectRatioByExpanding);
    m_arrPixmaps.append(pixmap);
    QImage img(strPath);
    img = img.mirrored(true, false);
    pixmap = new QPixmap;
    *pixmap = QPixmap::fromImage(img);
    m_arrRePixmaps.append(pixmap);
}

void Item::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QVector<QPixmap*>* pTarget;
    if (m_bReversal)
    {
        pTarget = &m_arrRePixmaps;

    }
    else
    {
        pTarget = &m_arrPixmaps;
    }
    painter->drawPixmap(m_Rect,*(*pTarget)[m_nCurrentIndex]);
}

QRectF Item::boundingRect() const
{
    return QRectF(0, 0, 900, 520);
}

void Item::AfterAnimationFinished()
{
    emit Next();
}

void Item::FrontAnimationFinished()
{
    if (++m_nCurrentIndex >= m_arrPixmaps.size())
    {
        m_nCurrentIndex = 0;
    }
    m_pAfterAnimation->start();
}

void Item::AnimationStart()
{
    if (m_bFirst)
    {
        m_bFirst = false;
        m_pFrontAnimation->start();
        return;
    }
    m_bReversal = !m_bReversal;
    m_pFrontAnimation->start();
}

//View
View::View(QString strPath, QWidget* parent) : QGraphicsView(parent)
{//初始化
    m_strImgPath = strPath;
	Init();
    Connects();
}

View::~View()
{
    for (auto& i : m_listItems) 
    {
        delete i;
    }
    delete m_pScene;
}

void View::Strat()
{
    m_listItems[0]->AnimationStart();
}

void View::Init()
{
	InitMember();
	InitUi();
}

void View::InitMember()
{
    m_pScene = new QGraphicsScene(this);

    Item* temp1 = new Item(QRect(20, 10, 164, 500));
    Item* temp2 = new Item(QRect(194, 10, 164, 500));
    Item* temp3 = new Item(QRect(542, 10, 164, 500));
    Item* temp4 = new Item(QRect(716, 10, 164, 500));
    m_listItems.append(temp1);
    m_listItems.append(temp2);
    m_listItems.append(temp3);
    m_listItems.append(temp4);
    m_listItems[0]->AppendPoxmap(m_strImgPath + "q22_10.jpg");
    m_listItems[0]->AppendPoxmap(m_strImgPath + "22_10.jpg");
    m_listItems[1]->AppendPoxmap(m_strImgPath + "q33_20.jpg");
    m_listItems[1]->AppendPoxmap(m_strImgPath + "33_20.jpg");
    m_listItems[2]->AppendPoxmap(m_strImgPath + "q33_40.jpg");
    m_listItems[2]->AppendPoxmap(m_strImgPath + "33_40.jpg");
    m_listItems[3]->AppendPoxmap(m_strImgPath + "q22_50.jpg");
    m_listItems[3]->AppendPoxmap(m_strImgPath + "22_50.jpg");
    for (int i = 0; i < 4; ++i) 
    {
        m_pScene->addItem(m_listItems[i]);
    }
}

void View::InitUi()
{
    this->setScene(m_pScene);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setViewportUpdateMode(FullViewportUpdate);   //刷新方式
    setCacheMode(CacheBackground);
    setRenderHints(QPainter::Antialiasing
        | QPainter::SmoothPixmapTransform
        | QPainter::TextAntialiasing);
}

void View::Connects()
{
    connect(m_listItems[0], &Item::Next, m_listItems[1], &Item::AnimationStart);
    connect(m_listItems[1], &Item::Next, m_listItems[2], &Item::AnimationStart);
    connect(m_listItems[2], &Item::Next, m_listItems[3], &Item::AnimationStart);
    connect(m_listItems[3], &Item::Next, m_listItems[0], &Item::AnimationStart);
}

void View::wheelEvent(QWheelEvent* e)
{
}

void View::drawBackground(QPainter* painter, const QRectF& rect) 
{
    painter->drawPixmap(rect, QPixmap(m_strImgPath+"backdrop.jpg"), QRectF());
}

//Loading
Loading::Loading() 
{
    this->setWindowFlags(Qt::SubWindow | Qt::FramelessWindowHint);
    setFixedSize(900, 520);
    QString strImagePath = GetImgPath();
    m_pView = new View(strImagePath, this);
    //m_labelMiddle = new QWidget(this);
    m_nTransparency = 0;
    m_strPohotoPath = strImagePath + "22330.jpg";
    IniAnimation();
}

Loading::~Loading()
{
    delete m_pView;
    //delete m_labelMiddle;
    delete m_pAnimation;
}

void Loading::StartLoad()
{
    show();
    QTimer::singleShot(30, this, [=]() {
        m_pView->Strat();
        //m_pAnimation->start();
        });
}

inline QString Loading::GetImgPath()
{
    return QCoreApplication::applicationDirPath() + "/res/images/loading/";
}

inline void Loading::IniAnimation()
{
    //m_labelMiddle->setGeometry(368,10,164,500);
    ////QPixmap pixmap(m_strPohotoPath);
    ////m_labelMiddle->setPixmap(pixmap.scaled(164, 500, Qt::IgnoreAspectRatio));
    ////m_labelMiddle->setStyleSheet(QString("background-image: url(%1);").arg(m_strPohotoPath));
    //m_labelMiddle->setStyleSheet("background-color: rgb(0, 0, 0);");
  

    //m_pAnimation = new QPropertyAnimation(this, "windowOpacity");
    //m_pAnimation->setDuration(1500);
    //m_pAnimation->setStartValue(1);
    //m_pAnimation->setEndValue(0);
    //m_pAnimation->setLoopCount(-1); 
}

void Loading::wheelEvent(QWheelEvent* e)
{
    return;
}