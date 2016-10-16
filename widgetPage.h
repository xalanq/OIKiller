#ifndef WIDGETPAGE_H
#define WIDGETPAGE_H

#include <QWidget>
#include "headers.h"
#include "widgetProblem.h"
#include "widgetAnswer.h"
#include "widgetTool.h"

class WidgetPage : public QWidget
{
	Q_OBJECT
public:
	explicit WidgetPage(QWidget *parent = 0);
	bool setProblem(const QString &path, ProblemType type, bool display_score = false);
	void setResult(const QList<QString> &wrong, const QList<QString> &correct, const QString &explaination = "");
	WidgetTool *tool();

signals:
	void previousProblem();
	void nextProblem();
	void addScore(double);
	void addCorrect(QString, ProblemType);
	void addWrong(QString, ProblemType);
	void addIgnore(QString, ProblemType);
	void updateRight(QString, ProblemType, bool);

public slots:
	void evaluate();
	void slotPreviousProblem();
	void slotNextProblem();
	void slotUpdateRight(bool ins);

private:
	WidgetProblem *widget_problem;
	WidgetAnswer *widget_answer;
	WidgetTool *widget_tool;
	QPushButton *btn_submit;
	QCheckBox *cbox_ignore;
	QLabel *view;
	QList<pair<QList<QString>, double> > current_answer;
	ProblemType current_type;
	bool papers;

	void setupUI();
	void setupConnection();
	void setupMain();

protected:
	void keyPressEvent(QKeyEvent *event);

};

#endif // WIDGETPAGE_H
