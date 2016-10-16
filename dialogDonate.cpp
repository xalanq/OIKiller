#include "dialogDonate.h"

DialogDonate::DialogDonate(QWidget *parent) : QDialog(parent)
{
	lbl = new QLabel;
	text_alipay = new QLabel;
	image_alipay = new QLabel;

	setupUI();
	setupConnection();
	setupMain();
}

void DialogDonate::setupUI()
{
	setWindowTitle(tr("捐赠"));
	lbl->setText(tr("如果你喜欢这个软件的话可以扫一扫捐赠作者>_<"));
	text_alipay->setText(tr("支付宝账号:whiteeaglealan@gmail.com"));
	image_alipay->setPixmap(QPixmap(":/donate/donate-alipay.png").scaled(500, 500));

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(lbl);
	layout->addWidget(text_alipay);
	layout->addWidget(image_alipay);

	layout->setAlignment(lbl, Qt::AlignCenter);
	layout->setAlignment(text_alipay, Qt::AlignCenter);
	layout->setAlignment(image_alipay, Qt::AlignCenter);

	setLayout(layout);
}

void DialogDonate::setupConnection()
{

}

void DialogDonate::setupMain()
{

}
