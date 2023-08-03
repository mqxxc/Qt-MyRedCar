#pragma once
//关于窗口
#include<QDialog>
namespace Ui { 
	class About; 
}

class About : public QDialog 
{
	Q_OBJECT

private:
	Ui::About* ui;


public:
	About(QWidget* parent = nullptr);
	~About();


private slots:
	void on_copy_clicked();					//复制按钮点击事件
};