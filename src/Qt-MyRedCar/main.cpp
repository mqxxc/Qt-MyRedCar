#include <QtWidgets/QApplication>
#include <QMessageBox>
#include <QFile>
#include <QLibrary>
#include <QSharedMemory>
#include <QProcess>
#include <QIcon>

#include"StartUp.h"
#include "Config.h"
#include"../playVideo_DLL/playVideo.h"
#include"../Screenshot_DLL/Screenshot.h"

bool testApp();

int main(int argc, char* argv[]) 
{
	QApplication app(argc, argv);
	QIcon ico(":/QtMyRedCar/taskbar");
	app.setWindowIcon(ico);

	if (testApp()) 
	{			//检查是否重复启动
		return 0;
	}

	StartUp exe;
	app.setQuitOnLastWindowClosed(false);
	exe.setApp(&app);
	return app.exec();
}

bool testApp() 
{
	QSharedMemory* sharedMemory = new QSharedMemory;
	sharedMemory->setKey("MyRedCar");
	if (sharedMemory->attach()) 
	{//应用已经启动调用显示主窗口，结束本进程
		QProcess* process = new QProcess;
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