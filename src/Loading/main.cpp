#include "Loading.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{//DXG传入main指令参数否者不给予启动
    QApplication a(argc, argv);
    Loading w;
    w.StartLoad();
    return a.exec();
}