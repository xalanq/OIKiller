#ifndef WIDGETMAIN_H
#define WIDGETMAIN_H

#include <QWidget>
#include "headers.h"
#include "widgetProblem.h"
#include "widgetAnswer.h"
#include "widgetPage.h"
#include "widgetTool.h"

class WidgetMain : public QWidget
{
	Q_OBJECT
public:
	explicit WidgetMain(QWidget *parent = 0);
	void work(const QList<pair<QString, ProblemType> > &problems);
	bool addProblem(const QString &path, ProblemType type);
	void setDisplayScore(bool flag);
	void finish(unsigned int delta_time);
	bool maybeFinish(unsigned int delta_time);
	void loadSetting(const QString &APP_PATH);
	void saveSetting(const QString &APP_PATH);

signals:
	void wantFinish();
	void stateUpdated(double, int, int, int, int, int);

public slots:
	void previousPage();
	void nextPage();
	void addScore(const double &score);
	void addCorrect(const QString &ID, ProblemType type);
	void addWrong(const QString &ID, ProblemType type);
	void addIgnore(const QString &ID, ProblemType type);
	void updateRight(const QString &id, ProblemType type, bool ins);

private:
	bool display_score;
	double count_score;
	int count_correct;
	int count_wrong;
	int count_ignore;
	int count_all;
	set<QString> right[5];
	set<QString> correct[5];
	set<QString> wrong[5];
	set<QString> ignore[5];
	QStackedWidget *pages;

	void setupUI();
	void setupConnection();
	void setupMain();
};

#endif // WIDGETMAIN_H
