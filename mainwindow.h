#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "headers.h"
#include "widgetMain.h"
#include "dialogChoose.h"
#include "dialogDonate.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	bool Paper;
	MainWindow(const QString &path, QWidget *parent = 0);
	~MainWindow();
	static bool isValidProblem(const QString &path);
	void startTimer(QTimer *timer);

public slots:
	void updateTime();

private:
	unsigned int delta_time;
	QString APP_PATH;
	WidgetMain *main_widget;
	QTimer *Timer;
	void setupUI();
	void setupConnection();
	void setupMain();
	void chooseWay();
	void workZhi(ProblemType type);
	void workRest(ProblemType type);
	void workQuanBu(ProblemType type);
	void workMoNi();
	void workAbout();
	void workDonate();

protected slots:
	void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
