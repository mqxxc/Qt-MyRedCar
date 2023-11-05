#pragma once
//关于窗口
#include<QDialog>

class About : public QDialog 
{
	Q_OBJECT

private:

public:
	About(QWidget* parent = nullptr);
	~About();


private slots:
	void on_copy_clicked();					//复制按钮点击事件
};