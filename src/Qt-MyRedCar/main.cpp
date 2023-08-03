#include <QtWidgets/QApplication>
#include<QMessageBox>
#include<QFile>
#include<QLibrary>
#include<QSharedMemory>
#include<QProcess>

#define MAIN_H
#include "macro.h"
#include"StartUp.h"
#include "Config.h"
#include"../playVideo/playVideo.h"
#include"../Screenshot/Screenshot.h"

bool testFile();
bool testApp();

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	app.setWindowIcon(QIcon(IcoPath));

	if (testApp()) 
	{			//检查是否重复启动
		return 0;
	}

	if (!testFile()) 
	{			//检测依赖的dll是否别误删
		return 0;
	}

	StartUp exe;
	app.setQuitOnLastWindowClosed(false);
	exe.setApp(&app);
	return app.exec();
}

bool testFile() {
	QWidget wid;
	wid.setWindowIcon(QIcon(IcoPath));
	wid.hide();
	QLibrary dll;
	dll.setFileName("playVideo");
	if (!dll.load()) {
		QMessageBox::warning(&wid, error_loseTitle, error_losePlayVideo);
		return false;
	}
	dll.unload();
	dll.setFileName("Screenshot");
	if (!dll.load()) {
		QMessageBox::warning(&wid, error_loseTitle, error_loseSCreenshot);
		return false;
	}
	dll.unload();
	dll.setFileName("SaveOrRead");
	if (!dll.load()) {
		QMessageBox::warning(&wid, error_loseTitle, error_loseSaveOrRead);
		return false;
	}
	dll.unload();
	QFile file(CONFIG->m_strAppPath + "/awaken_client.exe");
	if (!file.exists()) {
		QMessageBox::warning(&wid, error_loseTitle, error_loseAwaken);
		return false;
	}
	return true;
}

bool testApp() {
	QSharedMemory* sharedMemory = new QSharedMemory;
	sharedMemory->setKey("MyRedCar");
	if (sharedMemory->attach()) {//应用已经启动调用显示主窗口，结束本进程
		QProcess *process=new QProcess;
		process->start("awaken_client.exe");
		delete sharedMemory;
		return true;
	}
	QString error= sharedMemory->errorString();
	if (!sharedMemory->create(1)) 
	{//开辟共享空间失败失败，弹出提示框
		QMessageBox::warning(nullptr, "运行错误 ", "我也不晓得原因 ");
		return true;
	}
	return false;
}