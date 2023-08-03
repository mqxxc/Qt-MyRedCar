#include"ui_About.h"
#include"About.h"
#include<QClipboard>

About::About(QWidget* parent):QDialog(parent)
{
	ui = new Ui::About;
	ui->setupUi(this);
	setWindowFlag(Qt::Tool);

	this->show();
}

About::~About()
{
	delete ui;
}

void About::on_copy_clicked()
{
	QClipboard* clip = QApplication::clipboard();
	clip->setText(ui->mage->toPlainText());
}