#ifndef DIALOGDONATE_H
#define DIALOGDONATE_H

#include <QDialog>
#include "headers.h"

class DialogDonate : public QDialog
{
	Q_OBJECT
public:
	explicit DialogDonate(QWidget *parent = 0);

signals:

public slots:

private:

	QLabel *lbl;
	QLabel *text_alipay;
	QLabel *image_alipay;

	void setupUI();
	void setupConnection();
	void setupMain();

};

#endif // DIALOGDONATE_H
