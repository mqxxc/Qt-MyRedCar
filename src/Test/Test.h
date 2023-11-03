#pragma once

#include <QtWidgets/QWidget>
#include "ui_Test.h"

class Test : public QWidget
{
    Q_OBJECT

public:
    Test(QWidget *parent = nullptr);
    ~Test();

private:
    Ui::TestClass ui;
};
