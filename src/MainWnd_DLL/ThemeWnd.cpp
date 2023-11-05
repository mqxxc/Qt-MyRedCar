#include <QIcon>
#include <QLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QScrollArea>
#include <QMessageBox>
#include <QLineEdit>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QLibrary>
#include "ThemeWnd.h"
#include "Config.h"
#include "Elastic.h"
#include "RcWidget.h"
#include "set.h"
#include "../three/YQTools_Qt/YQTools_Qt.h"
#include "../playVideo_DLL/playVideo.h"
#define ThemeWnd_H
#include "macro.h"
#pragma execution_character_set("utf-8")

ThemeWnd::ThemeWnd(QWidget* parent) : QFrame(parent)
{
	IniFace();
	IniData();
	connect(this, &ThemeWnd::SearchUnit, m_pRcWnd, &RcWidget::SearchUnits);
	connect(this, &ThemeWnd::UnitsOrderChange, m_pRcWnd, &RcWidget::ChangeOrder);
}

ThemeWnd::~ThemeWnd()
{
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->ReleasePlayVideo();
		m_pPlayer = nullptr;
	}
	if (m_pDllPlayer->isLoaded())
	{
		m_pDllPlayer->unload();
		delete m_pDllPlayer;
		m_pDllPlayer = nullptr;
	}
}

void ThemeWnd::LoadingRc()
{
	m_pRcWnd->IniUnits();
	on_order_clicked();
}

void ThemeWnd::ReleaseInstance()
{
	disconnect();
	delete this;
}

void ThemeWnd::ConnectSig(const char* strSig, QObject* obj, const char* strSlot)
{
	connect(this, strSig, obj, strSlot);
}

inline void ThemeWnd::IniFace()
{
	//初始化窗体
	setWindowFlags(Qt::FramelessWindowHint |
		Qt::WindowMinimizeButtonHint);          // 无边框
	setWindowIcon(QIcon(YQTools_Qt::ToAbsolutePath(appIcoPath)));
	resize(YQTools_Qt::RelativeSize(MainWnd_W), YQTools_Qt::RelativeSize(MainWnd_H));
	Elastic* pElastic = new Elastic(this);
	pElastic->setTitleH(YQTools_Qt::RelativeSize(titleh));
	QPoint workSize = CONFIG->GetWorkDesktopPoint_RB();
	move((workSize.x() - width()) / 2,
		(workSize.y() - height()) / 2);

	QGridLayout* pMainLayout = new QGridLayout(this);
	pMainLayout->addWidget(IniTitle(), 0, 0, 1, 2);
	pMainLayout->addWidget(IniScreen(), 1, 0);
	pMainLayout->addWidget(IniMainBody(), 1, 1);
	pMainLayout->setMargin(0);
}

inline QFrame* ThemeWnd::IniTitle()
{
	QFrame* pWnd = new QFrame(this);
	pWnd->setStyleSheet("background-color: rgba(255, 255, 127,0.1);");
	pWnd->setFixedHeight(YQTools_Qt::RelativeSize(titleh));
	QHBoxLayout* pMainLayout = new QHBoxLayout(pWnd);
	pMainLayout->setContentsMargins(0, 0, 7, 0);

	QFrame* pLeftWnd = new QFrame(pWnd);
	pLeftWnd->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	QHBoxLayout* pLeftLayout = new QHBoxLayout(pLeftWnd);
	pLeftLayout->setMargin(0);
	QLabel* pIco = new QLabel(pLeftWnd);
	pIco->setPixmap(QPixmap(YQTools_Qt::ToAbsolutePath(titleIcoPath)));
	pLeftLayout->addWidget(pIco);
	pMainLayout->addWidget(pLeftWnd);
	pMainLayout->addSpacerItem(new QSpacerItem(2, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

	QPushButton* pBtnMinSize = new QPushButton(pWnd);
	pBtnMinSize->setText("—");
	pBtnMinSize->setStyleSheet("border-style:none;\nbackground-color: rgba(170, 255, 255,0.4);");
	pBtnMinSize->setFixedSize(YQTools_Qt::RelativeSize(43), YQTools_Qt::RelativeSize(29));
	connect(pBtnMinSize, &QPushButton::clicked, this, &ThemeWnd::showMinimized);
	pMainLayout->addWidget(pBtnMinSize);

	QPushButton* pBtnMaxSize = new QPushButton(pWnd);
	pBtnMaxSize->setText("□");
	pBtnMaxSize->setStyleSheet("border-style:none;\nbackground-color: rgba(0, 85, 255,0.4);");
	pBtnMaxSize->setFixedSize(YQTools_Qt::RelativeSize(43), YQTools_Qt::RelativeSize(29));
	connect(pBtnMaxSize, &QPushButton::clicked, this, &ThemeWnd::on_maxSize_clicked);
	pMainLayout->addWidget(pBtnMaxSize);

	QPushButton* pBtnOff = new QPushButton(pWnd);
	pBtnOff->setText("×");
	pBtnOff->setStyleSheet("border-style:none;\nbackground-color: rgba(255, 85, 0,0.4);");
	pBtnOff->setFixedSize(YQTools_Qt::RelativeSize(43), YQTools_Qt::RelativeSize(29));
	connect(pBtnOff, &QPushButton::clicked, this, [&]() {
		emit WndClose();
		});
	pMainLayout->addWidget(pBtnOff);

	return pWnd;
}

inline QFrame* ThemeWnd::IniScreen()
{
	return new QFrame(this);
}

inline QFrame* ThemeWnd::IniMainBody()
{
	QFrame* pMainWnd = new QFrame(this);
	QHBoxLayout* pMainLayout = new QHBoxLayout(pMainWnd);
	pMainLayout->setMargin(0);

	QVBoxLayout* pLeftLayout = new QVBoxLayout;
	pLeftLayout->addWidget(IniMainBody_LT());
	QScrollArea* pRcArea = new QScrollArea(pMainWnd);
	pRcArea->setStyleSheet("border:none");
	m_pRcWnd = new RcWidget(pMainWnd);
	connect(m_pRcWnd, &RcWidget::SelectUnitSig, this, &ThemeWnd::PreviewVideo);
	connect(m_pRcWnd, &RcWidget::IniFinish, this, [&]() {
		emit LoadFinish();
		});
	pRcArea->setWidgetResizable(true);
	pRcArea->setWidget(m_pRcWnd);
	pLeftLayout->addWidget(pRcArea);
	pLeftLayout->addWidget(IniMainBody_LB());
	pLeftLayout->setStretch(0, 5);
	pLeftLayout->setStretch(1, 32);
	pLeftLayout->setStretch(2, 5);

	pMainLayout->addLayout(pLeftLayout);
	pMainLayout->addWidget(IniMainBody_R());

	return pMainWnd;
}

inline QFrame* ThemeWnd::IniMainBody_LT()
{
	QFrame* pMainWnd = new QFrame(this);
	QHBoxLayout* pMainLayout = new QHBoxLayout(pMainWnd);

	m_pInput = new QLineEdit(pMainWnd);
	m_pInput->setMaximumHeight(YQTools_Qt::RelativeSize(49));
	m_pInput->setFont(QFont("Timers", YQTools_Qt::RelativeSize(12), QFont::Bold));
	m_pInput->setPlaceholderText(inputTxt);
	connect(m_pInput, &QLineEdit::editingFinished, this, &ThemeWnd::on_search_inputFinish);
	QVBoxLayout* pEditTmpLayout = new QVBoxLayout(m_pInput);
	pEditTmpLayout->setSpacing(0);
	pEditTmpLayout->setMargin(0);

	QPushButton* pButSearch = new QPushButton(m_pInput);
	pButSearch->setIcon(QIcon(QPixmap(YQTools_Qt::ToAbsolutePath(inputIco)).
		scaled(YQTools_Qt::RelativeSize(49), YQTools_Qt::RelativeSize(49), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	pButSearch->setFlat(true);
	pButSearch->setStyleSheet("border: 0px"); //消除边框
	pButSearch->setCursor(Qt::PointingHandCursor);
	pButSearch->resize(YQTools_Qt::RelativeSize(49), YQTools_Qt::RelativeSize(49));
	pButSearch->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	pButSearch->setIconSize(QSize(YQTools_Qt::RelativeSize(40), YQTools_Qt::RelativeSize(40)));
	connect(pButSearch, &QPushButton::clicked, this, &ThemeWnd::on_search_inputFinish);
	pEditTmpLayout->addWidget(pButSearch);
	pEditTmpLayout->setAlignment(pButSearch, Qt::AlignHCenter | Qt::AlignRight);
	pMainLayout->addWidget(m_pInput);

	QPushButton* pScreenWnd = new QPushButton(pMainWnd);
	pScreenWnd->setText("筛选");
	pScreenWnd->setStyleSheet("background:transparent;");
	connect(pScreenWnd, &QPushButton::clicked, this, &ThemeWnd::on_filter_clicked);
	pMainLayout->addWidget(pScreenWnd);
	pMainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

	QLabel* pLabClassify = new QLabel(pMainWnd);
	pLabClassify->setText("分类");
	pMainLayout->addWidget(pLabClassify);

	m_pButOrder = new QPushButton(pMainWnd);
	m_pButOrder->setIconSize(QSize(YQTools_Qt::RelativeSize(40), YQTools_Qt::RelativeSize(40)));
	m_pButOrder->setStyleSheet("background:transparent;");
	connect(m_pButOrder, &QPushButton::clicked, this, &ThemeWnd::on_order_clicked);
	pMainLayout->addWidget(m_pButOrder);
	QComboBox* pComBoxtypes = new QComboBox(pMainWnd);
	pComBoxtypes->setMaximumSize(YQTools_Qt::RelativeSize(200), YQTools_Qt::RelativeSize(40));
	pComBoxtypes->addItems(QStringList() << comboBox1 << comboBox2
		<< comboBox3 << comboBox4);
	connect(pComBoxtypes, SIGNAL(activated(QString)), this, SLOT(ChangeCondition(QString)));
	pMainLayout->addWidget(pComBoxtypes);

	QPushButton* pButSetbox = new QPushButton(pMainWnd);
	pButSetbox->setIconSize(QSize(YQTools_Qt::RelativeSize(40), YQTools_Qt::RelativeSize(40)));
	pButSetbox->setIcon(QIcon(QPixmap(YQTools_Qt::ToAbsolutePath(settingIco)).
		scaled(YQTools_Qt::RelativeSize(40), YQTools_Qt::RelativeSize(40), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	pButSetbox->setStyleSheet("background:transparent;");
	connect(pButSetbox, &QPushButton::clicked, this, &ThemeWnd::on_setbox_clicked);
	pMainLayout->addWidget(pButSetbox);

	pMainLayout->setStretch(0, 5);
	pMainLayout->setStretch(1, 2);
	pMainLayout->setStretch(2, 0);
	pMainLayout->setStretch(3, 2);
	pMainLayout->setStretch(4, 0);
	pMainLayout->setStretch(5, 3);
	pMainLayout->setStretch(6, 1);
	return pMainWnd;
}

inline QFrame* ThemeWnd::IniMainBody_LB()
{
	QFrame* pMainWnd = new QFrame(this);
	QHBoxLayout* pMainLayout = new QHBoxLayout(pMainWnd);

	QPushButton* pButOriginality = new QPushButton(pMainWnd);
	pButOriginality->setText("浏览创意工坊");
	pButOriginality->setMinimumSize(YQTools_Qt::RelativeSize(200), YQTools_Qt::RelativeSize(50));
	connect(pButOriginality, &QPushButton::clicked, this, &ThemeWnd::on_originality_clicked);
	pMainLayout->addWidget(pButOriginality);
	pMainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

	QPushButton* pButLoad = new QPushButton(pMainWnd);
	pButLoad->setText("加载本地文件");
	pButLoad->setMinimumSize(YQTools_Qt::RelativeSize(200), YQTools_Qt::RelativeSize(50));
	connect(pButLoad, &QPushButton::clicked, this, &ThemeWnd::on_lod_clicked);
	pMainLayout->addWidget(pButLoad);
	pMainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

	QPushButton* pButRefresh = new QPushButton(pMainWnd);
	pButRefresh->setText("刷新");
	pButRefresh->setMinimumSize(YQTools_Qt::RelativeSize(200), YQTools_Qt::RelativeSize(50));
	connect(pButRefresh, &QPushButton::clicked, this, &ThemeWnd::on_Refresh_clicked);
	pMainLayout->addWidget(pButRefresh);
	pMainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

	QPushButton* pButAbout = new QPushButton(pMainWnd);
	pButAbout->setText("嘤起np!!!");
	pButAbout->setStyleSheet("background-color: rgba(255, 255, 255, 0);\n\nborder-color: rgba(255, 255, 255, 0);");
	connect(pButAbout, &QPushButton::clicked, this, &ThemeWnd::on_about_clicked);
	pMainLayout->addWidget(pButAbout);

	return pMainWnd;
}

inline QFrame* ThemeWnd::IniMainBody_R()
{
	QFrame* pMainWnd = new QFrame(this);
	pMainWnd->setFixedWidth(YQTools_Qt::RelativeSize(320));
	QVBoxLayout* pMainLayout = new QVBoxLayout(pMainWnd);

	QFrame* pTopWnd = new QFrame(pMainWnd);
	pTopWnd->setStyleSheet("background-color: rgb(170, 255, 127);");
	QVBoxLayout* pTopLayout = new QVBoxLayout(pTopWnd);
	pTopLayout->setMargin(0);

	m_pLabVideoPreview = new QLabel(pTopWnd);
	m_pLabVideoPreview->setFixedHeight(YQTools_Qt::RelativeSize(300));
	m_pLabVideoPreview->setText("请选择一个壁纸！！！");
	m_pLabVideoPreview->setAlignment(Qt::AlignCenter);
	m_pLabVideoPreview->setStyleSheet("background-color: rgba(235, 255, 255,0.7);");
	pTopLayout->addWidget(m_pLabVideoPreview);
	pTopLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding));

	QLabel* pBottomWnd = new QLabel(pTopWnd);
	pBottomWnd->setStyleSheet("background-color: rgb(0, 0, 0);");
	pTopLayout->addWidget(pBottomWnd);
	pTopLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding));

	pMainLayout->addWidget(pTopWnd);
	pMainLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding));

	QGridLayout* pGridLayout = new QGridLayout;
	QPushButton* pButDelete = new QPushButton(pMainWnd);
	pButDelete->setText("移除壁纸");
	pButDelete->setMinimumHeight(YQTools_Qt::RelativeSize(40));
	connect(pButDelete, &QPushButton::clicked, this, &ThemeWnd::on_deletebo_clicked);
	pGridLayout->addWidget(pButDelete, 0, 0, 1, 2);

	QPushButton* pButOK = new QPushButton(pMainWnd);
	pButOK->setText("确定");
	pButOK->setMinimumHeight(YQTools_Qt::RelativeSize(40));
	connect(pButOK, &QPushButton::clicked, this, &ThemeWnd::on_OK_clicked);
	pGridLayout->addWidget(pButOK, 1, 0, 1, 1);

	QPushButton* pButExit = new QPushButton(pMainWnd);
	pButExit->setText("退出");
	pButExit->setMinimumHeight(YQTools_Qt::RelativeSize(40));
	connect(pButExit, &QPushButton::clicked, this, &ThemeWnd::on_endbo_clicked);
	pGridLayout->addWidget(pButExit, 1, 1, 1, 1);

	pMainLayout->addLayout(pGridLayout);
	pMainLayout->setStretch(0, 3);
	pMainLayout->setStretch(1, 0);
	pMainLayout->setStretch(2, 1);

	return pMainWnd;
}

inline void ThemeWnd::IniData()
{
	m_leftTop.adjust(0, 0, 1, 1);
	m_pPlayer = nullptr;
	m_pDllPlayer = nullptr;
	m_nState = 2;
}

void ThemeWnd::on_maxSize_clicked()
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

inline void ThemeWnd::SetMaxButtonState(bool bIsOnMax)
{
	if (bIsOnMax)
	{
		m_nState |= 1;
	}
	else
	{
		m_nState &= Max_8bit ^ 1;
	}
}

inline bool ThemeWnd::IsOnMaxButton()
{
	return m_nState & 1;
}

inline void ThemeWnd::SetOrderState(bool bNormal)
{
	if (bNormal)
	{
		m_nState |= 2;
	}
	else
	{
		m_nState &= Max_8bit ^ 2;
	}
}

inline bool ThemeWnd::IsNormalOrder()
{
	return m_nState & 2;
}

void ThemeWnd::on_search_inputFinish()
{
}

void ThemeWnd::on_filter_clicked()
{
	QMessageBox::about(this, MsgTitle, Msg_filter);
}

void ThemeWnd::on_order_clicked()
{
	QPixmap pixmap;
	if (IsNormalOrder())
	{
		pixmap.load(YQTools_Qt::ToAbsolutePath(sort_p));
		SetOrderState(false);
	}
	else
	{
		pixmap.load(YQTools_Qt ::ToAbsolutePath(sort_r));
		SetOrderState(true);
	}

	int w = m_pButOrder->geometry().width();
	m_pButOrder->setIcon(QIcon(pixmap.scaled(w, w, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	emit UnitsOrderChange(IsNormalOrder());
}

void ThemeWnd::on_setbox_clicked()
{
	/*Set* set = new Set;
	delete set;*/
}

void ThemeWnd::ChangeCondition(QString strCondition)
{
	if (strCondition == comboBox1)
	{
		m_pRcWnd->ChangeCondition(RcWidget::SortCriteria::name);
	}
	else if (strCondition == comboBox2)
	{
		m_pRcWnd->ChangeCondition(RcWidget::SortCriteria::ImportTime);
	}
	else if (strCondition == comboBox3)
	{
		m_pRcWnd->ChangeCondition(RcWidget::SortCriteria::lastTime);
	}
	else if (strCondition == comboBox4)
	{
		m_pRcWnd->ChangeCondition(RcWidget::SortCriteria::love);
	}
	else
	{
		m_pRcWnd->ChangeCondition(RcWidget::SortCriteria::onoe);
	}
}

void ThemeWnd::on_originality_clicked()
{
	QMessageBox::about(this, MsgTitle, Msg_originality);
}

void ThemeWnd::on_about_clicked()
{
	QDesktopServices::openUrl(QUrl(QString(Msg_about_link)));
}

void ThemeWnd::on_lod_clicked()
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
		if (m_pRcWnd->AddUnit(path))
		{
			QMessageBox::about(this, MsgTitle, loadFinish_Text);
		}
		else
		{
			QMessageBox::about(this, MsgTitle, loadFail);
		}
	}
	fileDialog.setHistory(QStringList());
	fileDialog.close();
}

void ThemeWnd::on_Refresh_clicked()
{
	m_pRcWnd->Refresh();
	m_pInput->setText("");
}

void ThemeWnd::on_deletebo_clicked()
{
	CONFIG->m_strVideoPath.clear();
	emit UpdateDesVidoe("");
}

void ThemeWnd::on_OK_clicked()
{
	emit UpdateDesVidoe(CONFIG->m_strVideoPath);
}

void ThemeWnd::on_endbo_clicked()
{
	emit MainAppExit();
}

void ThemeWnd::PreviewVideo()
{
	if (!CONFIG->m_strVideoPath.isEmpty())
	{
		if (m_pPlayer == nullptr)
		{
			if (m_pDllPlayer == nullptr)
			{
				m_pDllPlayer = new QLibrary(YQTools_Qt::ToAbsolutePath("/dll/playVideo_DLL"));
			}
			if (!m_pDllPlayer->isLoaded())
			{
				if (!m_pDllPlayer->load())
				{
					return;
				}
			}
			m_pPlayer = ((VideoPalyer * (*)(QWidget*))
				(m_pDllPlayer->resolve("GetpPlayVideo")))(m_pLabVideoPreview);
		}
		m_pPlayer->SetVideo(CONFIG->m_strVideoPath);
		m_pPlayer->SetVolume(0);
		m_pPlayer->PlayVideo();
	}
	else
	{
		if (m_pDllPlayer != nullptr)
		{
			if (!m_pDllPlayer->isLoaded())
			{
				if (m_pPlayer != nullptr)
				{
					m_pPlayer->ReleasePlayVideo();
					m_pPlayer = nullptr;
				}
				m_pDllPlayer->unload();
			}
		}
	}
}

void ThemeWnd::LoadFinish()
{
	emit LoadFinishSig();
}

MAINWND_EXPORT ThemeWnd* GetThemeWnd(QWidget* parent)
{
	return new ThemeWnd(parent);
}

MAINWND_EXPORT void SetRunPath(QString strPath)
{
	return YQTools_Qt::SetRunPath(strPath);
}

MAINWND_EXPORT void SetScaling(float fScaling)
{
	return YQTools_Qt::SetScaling(fScaling);
}
