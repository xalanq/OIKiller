#ifndef WIDGETANSWER_H
#define WIDGETANSWER_H

#include <QWidget>
#include "headers.h"

class WidgetAnswer : public QWidget
{
	Q_OBJECT
public:
	explicit WidgetAnswer(QWidget *parent = 0);
	void toDanXuan(const QList<QString> &list);
	void toDuoXuan(const QList<QString> &list);
	void toQiuJie(const QList<QString> &list);
	void toYueDu(const QList<QString> &list);
	void toWanShan(const QList<QString> &list);
	QList<QString> getAnswer();

signals:

public slots:

private:
	ProblemType type;
	void clearLayout();
	void setupUI();
	void setupConnection();
	void setupMain();
};

#endif // WIDGETANSWER_H
