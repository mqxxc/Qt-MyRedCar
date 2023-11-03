#include <QClipboard>
#include <QApplication>
#include"About.h"


About::About(QWidget* parent):QDialog(parent)
{
	setWindowFlag(Qt::Tool);

	this->show();
}

About::~About()
{
}

void About::on_copy_clicked()
{
	QClipboard* clip = QApplication::clipboard();
	//clip->setText(ui->mage->toPlainText());
}