#include<QFile>
#include<QLibrary>
#include<QSettings>
#include<QMessageBox>
#define SET_H
#include"macro.h"
#include"Set.h"
#include "Config.h"


Set::Set(QObject* parent):ui(new Ui::Set()){
	ui->setupUi(this);
	loop = new QEventLoop(this);
	oldValue= value = 0;
	initOption();
	
	this->setWindowIcon(QIcon(icoPath));
	this->show();
	loop->exec();
}

Set::~Set(){
	saveData();
	delete ui;
	delete loop;
}

void Set::saveData()
{
}

void Set::readData()
{
}

void Set::initOption(){
	readData();
	if (value == 0) {
		return;
	}
	if (value & 1) {	//开机自启项
		ui->self_start->setChecked(true);
	}
}

void Set::svaeSetMsg(){
	if (ui->self_start->checkState() != Qt::Unchecked) {	//开机自启
		if (!(value & 2)) {
			value |= (2 & SeItemNum);
		}
	}
	else {
		if (value & 2) {
			value &= (2 ^ SeItemNum);
		}
	}

	
	reviseSet(value^oldValue);
	oldValue = value;
}

void Set::reviseSet(unsigned int value){
	//dxg思来想去觉得set应该放在dll中但是有信号以后在修改
	//设置开机自启，flag表示是否开机自启
	if (value & 2) {
		setSelf_start(this->value & 2);
	}
}

void Set::on_close_clicked(){
	this->close();
}

void Set::closeEvent(QCloseEvent* e){
	loop->quit();
}

void Set::setSelf_start(bool state)
{
	QString strAppName = CONFIG->GetAppName();
	

	QSettings* reg = new QSettings(
		HEKY_AUTORUN,//宏定义，自启动的注册表位置
		QSettings::NativeFormat);
	QString val = reg->value(strAppName).toString();//根据appName获取键值

	if (state) 
	{//打开开机自启
		QString appPath = CONFIG->m_strAppPath + "/" + strAppName;
		if (val != appPath) {//键值不是本程序地址或者不存在，则写入键值
			reg->setValue(strAppName, appPath);
		}
		QString v = reg->value(strAppName).toString();//读取刚刚写入的键值
		if (v != appPath) {//写入失败
			ui->self_start->setChecked(false);
			QMessageBox::warning(this, error_addKeyTitle, error_addKeyText);
		}
	}
	else 
	{//关闭开机自启
		reg->remove(strAppName);//移除键值
		QString v = reg->value(strAppName).toString();
		if (v != "") {//键值仍然存在，移除失败
			ui->self_start->setChecked(true);
			QMessageBox::warning(this, error_addKeyTitle, error_addKeyText);
		}
	}
	reg->deleteLater();//延迟删除
}

void Set::on_ok_clicked() 
{
	svaeSetMsg();
	this->close();
}