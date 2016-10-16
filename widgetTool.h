#ifndef WIDGETTOOL_H
#define WIDGETTOOL_H

#include <QWidget>
#include "headers.h"

class WidgetTool : public QWidget
{
	Q_OBJECT
public:
	explicit WidgetTool(QWidget *parent = 0);
	void setProblemID(const QString &name);
	void setProblemCount(const int &now, const int &tot, const int &rest);
	void setScore(const int &correct, const int &wrong, const int &ignore, const double &score = 0);
	void setDisplay(bool flag);
	QString getProblemID();

signals:
	void previousProblem();
	void nextProblem();

public slots:
	void stateUpdated(const double &score, const int &correct, const int &wrong, const int &ignore, const int &all, const int &index);

private:
	QLabel *lbl_problem_id;
	QLabel *lbl_problem_count;
	QLabel *lbl_score;
	QPushButton *btn_left;
	QPushButton *btn_right;
	QString ID;
	bool display;

	void setupUI();
	void setupConnection();
	void setupMain();
};

#endif // WIDGETTOOL_H
