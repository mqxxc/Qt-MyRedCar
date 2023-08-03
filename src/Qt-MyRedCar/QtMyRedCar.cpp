#include<QLibrary>              
#include<QMessageBox>          
#include<QDesktopServices>      
#include<QUrl>                  
#include<QFileDialog>           
#include<QTimer>               
#include<QLineEdit>             
#include<QMouseEvent>           
#include<QListView>
#define QTMYREDCAR_H
#include"macro.h"
#include "ui_qtmyredcar.h"      
#include "QtMyRedCar.h"
#include"Elastic.h"
#include"set.h"
#include "Config.h"
#include"../playVideo/playVideo.h"     
#pragma execution_character_set("utf-8")

                   
QtMyRedCar::QtMyRedCar(QWidget* parent) : QWidget(parent)
{
	m_pUi = new Ui::QtMyRedCar;
	m_pUi->setupUi(this);
	InitMember();
	InitUi();
	Connects();
}

QtMyRedCar::~QtMyRedCar() 
{
	delete m_pEvenRoot;
	delete m_pPlayer;
	if (m_pDll->isLoaded())
	{
		m_pDll->unload();
	}
	delete m_pDll;
	delete m_pUi;
	this->disconnect();
}

void QtMyRedCar::InitMember() 
{
	m_pDll = new QLibrary("playVideo");
	m_pPlayer = nullptr;
	m_pEvenRoot = new Elastic(this);
	m_nState = 2;
}

void QtMyRedCar::InitUi() 
{
// 窗口基本设置
	setWindowFlags(Qt::FramelessWindowHint |
		Qt::WindowMinimizeButtonHint);          // 无边框
	setWindowIcon(QIcon(CONFIG->m_strAppPath + icoPath));

	//标题栏
	int title = titleh * CONFIG->m_fScale;
	m_pUi->title->setMinimumSize(QSize(MainWnd_W * CONFIG->m_fScale, title));     //初始化标题栏

//初始窗口大小
	this->resize(QSize(MainWnd_W * CONFIG->m_fScale, MainWnd_H * CONFIG->m_fScale + title));
	//移动窗口至屏幕中间
	QPoint workSize = CONFIG->GetWorkDesktopPoint_RB();
	move((workSize.x() - width()) / 2,
		(workSize.y() - height()) / 2);
	//其他默认操作
	m_pEvenRoot->setTitleH(title);

	//title
		//无滑块
	m_pUi->titletext->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_pUi->titletext->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//取消右键菜单
	m_pUi->titletext->setContextMenuPolicy(Qt::NoContextMenu);
	//鼠标事件穿透
	m_pUi->titletext->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	//分类设置字体格式
	m_pUi->label->setFont(QFont("Timers", 10, QFont::Bold));
	//过滤按钮
	m_pUi->filter->setLayoutDirection(Qt::RightToLeft);
	//Rcwid
	m_pUi->RcWid->SetScaling(CONFIG->m_fScale);
	//comboBox
	m_pUi->comboBox->addItems(QStringList() << comboBox1 << comboBox2
		<< comboBox3 << comboBox4);
	m_pUi->comboBox->setView(new QListView());
	//order
	on_order_clicked();
	//延时初始化
	QTimer::singleShot(10, this, [=]() {
		InitSearch();
		InitVidewid();
		});
}

void QtMyRedCar::Connects() 
{
	connect(m_pUi->RcWid, &RcWidget::SelectUnit, this, &QtMyRedCar::previewVideo);
	connect(m_pUi->RcWid, &RcWidget::IniFinish, this, &QtMyRedCar::LoadFinish);
	connect(this,      &QtMyRedCar::search, m_pUi->RcWid, &RcWidget::SearchUnits);
	connect(this,      &QtMyRedCar::ChangeUnitOrder, m_pUi->RcWid, &RcWidget::ChangeOrder);
	connect(m_pUi->comboBox, SIGNAL(activated(QString)), m_pUi->RcWid, SLOT(ChangeCondition(QString)));
}

void QtMyRedCar::InitSearch() 
{
	int w = m_pUi->widget->height();
	m_pInput = new QLineEdit;
	m_pInput->setMinimumHeight(w);
	m_pInput->setFont(QFont("Timers", 16, QFont::Bold));
	m_pInput->setPlaceholderText(inputTxt);

	m_pButton = new QPushButton;
	QPixmap pixmap(CONFIG->m_strAppPath+inputIco);
	QPixmap fitpixmap = pixmap.scaled(w, w, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	m_pButton->setIcon(QIcon(fitpixmap));
	m_pButton->setIconSize(QSize(w, w));
	m_pButton->setFlat(true);
	m_pButton->setStyleSheet("border: 0px"); //消除边框
	m_pButton->setCursor(Qt::PointingHandCursor);
	m_pButton->installEventFilter(this);

	QVBoxLayout* pEditTmpLayout = new QVBoxLayout(m_pInput);
	pEditTmpLayout->setSpacing(0);
	pEditTmpLayout->setMargin(0);
	pEditTmpLayout->addWidget(m_pButton);
	pEditTmpLayout->setAlignment(m_pButton, Qt::AlignHCenter | Qt::AlignRight);

	QGridLayout* pMainLayout = new QGridLayout();
	pMainLayout->setMargin(0);
	pMainLayout->setSpacing(0);
	pMainLayout->addWidget(m_pInput);

	m_pUi->widget->setLayout(pMainLayout);
}

void QtMyRedCar::InitVidewid() 
{
	m_pUi->videwid->setMaximumHeight(m_pUi->videwid->height());
}

inline void QtMyRedCar::SetMaxButtonState(bool bIsOnMax)
{
	if (bIsOnMax)
	{
		m_nState |= Max_8bit & 1;
	}
	else
	{
		m_nState &= Max_8bit ^ 1;
	}
}

inline bool QtMyRedCar::IsOnMaxButton()
{
	return m_nState & 1;
}

inline void QtMyRedCar::SetOrderState(bool bNormal)
{
	if (bNormal)
	{
		m_nState |= Max_8bit & 2;
	}
	else
	{
		m_nState &= Max_8bit & 2;
	}
}

inline bool QtMyRedCar::IsNormalOrder()
{
	return m_nState & 2;
}

void QtMyRedCar::on_maxsize_clicked() 
{
	if (IsOnMaxButton())
	{
		//dxg后期可加上ico来代替这里用文字‘□’表示
		this->setGeometry(m_leftTop);
		SetMaxButtonState(false);
	}
	else 
	{
		m_leftTop = this->geometry();
		this->hide();
		this->showMaximized();
		SetMaxButtonState(true);
	}
}

void QtMyRedCar::on_originality_clicked() 
{
	QMessageBox::about(this, MsgTitle, Msg_originality);
}

void QtMyRedCar::on_about_clicked() 
{
	QDesktopServices::openUrl(QUrl(QString(Msg_about_link)));
}

void QtMyRedCar::on_lod_clicked() 
{
	//定义文件对话框类
	QFileDialog fileDialog(this);
	//定义文件对话框标题
	fileDialog.setWindowTitle(dialogTitle);
	//设置默认文件路径
	fileDialog.setDirectory(".");
	//设置文件过滤器
	fileDialog.setNameFilter(dialogFilter);
	//设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
	fileDialog.setFileMode(QFileDialog::ExistingFiles);
	//设置视图模式
	fileDialog.setViewMode(QFileDialog::Detail);
	//判断是否点击确定键
	if (fileDialog.exec() != QFileDialog::Rejected) 
	{

		QStringList fileNames = fileDialog.selectedFiles();
		QString path = fileNames.join("");
		if (m_pUi->RcWid->MoveFile(path)) 
		{
			QMessageBox::about(this, MsgTitle, loadFinish);
		}
		else 
		{
			QMessageBox::about(this, MsgTitle, loadFail);
		}
	}
	fileDialog.setHistory(QStringList());
	fileDialog.close();
}

void QtMyRedCar::on_Refresh_clicked() 
{
	m_pUi->RcWid->Refresh();
	m_pInput->setText("");
}

void QtMyRedCar::on_deletebo_clicked() 
{
	emit upDesk();
}

void QtMyRedCar::on_OK_clicked() 
{
	emit upDesk();
}

void QtMyRedCar::on_endbo_clicked() 
{
	emit MainAppExit();
}

void QtMyRedCar::previewVideo(QString path) 
{
	if (!path.isEmpty()) 
	{
		if (m_pPlayer == nullptr) 
		{
			m_pDll->load();
			m_pPlayer = ((VideoPalyer * (*)(QWidget*))
				(m_pDll->resolve("getpvobj")))(m_pUi->videwid);
		}
		CONFIG->m_strVideo = path;
		m_pPlayer->SetVideo(path);
		m_pPlayer->SetVolume(0);
		m_pPlayer->PlayVideo();
	}
	else {
		CONFIG->m_strVideo.clear();
		delete m_pPlayer;
		m_pPlayer = nullptr;
		m_pDll->unload();
	}
}

void QtMyRedCar::on_off_clicked() 
{
	emit WndToClear();
}

void QtMyRedCar::on_filter_clicked() 
{
	QMessageBox::about(this, MsgTitle, Msg_filter);
}

void QtMyRedCar::on_order_clicked() 
{
	if (IsNormalOrder())
	{
		QPixmap pixmap(CONFIG->m_strAppPath+sort_p);
		int w = m_pUi->order->geometry().width();
		QPixmap fitpixmap = pixmap.scaled(w, w, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		m_pUi->order->setIcon(QIcon(fitpixmap));
		SetOrderState(false);
		emit ChangeUnitOrder(IsNormalOrder());
	}
	else 
	{
		QPixmap pixmap(CONFIG->m_strAppPath+sort_r);
		int w = m_pUi->order->geometry().width();
		QPixmap fitpixmap = pixmap.scaled(w, w, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		m_pUi->order->setIcon(QIcon(fitpixmap));
		SetOrderState(true);
		emit ChangeUnitOrder(IsNormalOrder());
	}
}

void QtMyRedCar::on_setbox_clicked() 
{
	Set* set=new Set;
	delete set;
}

void QtMyRedCar::LoadFinish() 
{
	emit LoadFinishSig();
}

bool QtMyRedCar::eventFilter(QObject* obj, QEvent* even) 
{
	if (obj == m_pButton) 
	{
		if (even->type() == QEvent::MouseButtonPress) 
		{
			QMouseEvent* e = static_cast <QMouseEvent*> (even);
			if (e->button() == Qt::LeftButton) 
			{
				QString name = m_pInput->text();
				if (!name.isEmpty()) 
				{
					emit search(name);
				}
				return true;
			}
		}
	}
	return QObject::eventFilter(obj, even);
}