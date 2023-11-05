#include "Test.h"
#include <QtWidgets/QApplication>
#include "../MainWnd_DLL/ThemeWnd.h"
//#include "../MainWnd_DLL/SetConfig.h"
#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString op = SIGNAL("kkkkkkl");
   /* SetConfig::SetRunPath("F:/源代码/C++/Qt-MyRedCar/make/x64/Debug");
    SetConfig::SetScaling(1);
    ThemeWnd wnd;
    wnd.show();
    wnd.LoadingRc();*/
    return a.exec();
}
