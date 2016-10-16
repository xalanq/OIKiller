#ifndef WIDGETPROBLEM_H
#define WIDGETPROBLEM_H

#include <QWidget>
#include "headers.h"

class WidgetProblem : public QWidget
{
	Q_OBJECT
public:
	explicit WidgetProblem(QWidget *parent = 0);
	void updateProblem();
	void clearProblem();
	void addProblem(const QImage &image);

signals:

public slots:

private:
	QList<QImage> list;
	void setupUI();
	void setupConnection();
	void setupMain();
};

#endif // WIDGETPROBLEM_H
