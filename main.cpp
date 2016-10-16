#include "mainwindow.h"
#include <QApplication>
#include "headers.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setWindowIcon(QIcon(":/images/icon.jpg"));
	QCoreApplication::setOrganizationName(AUTHOR_NAME);
	QCoreApplication::setApplicationName(APP_NAME);

	MainWindow w(QCoreApplication::applicationDirPath());

	w.show();

	if (w.Paper)
	{
		QTimer *timer = new QTimer;
		QObject::connect(timer, SIGNAL(timeout()),
						 &w, SLOT(updateTime()));
		w.startTimer(timer);
		timer->start(1000);
	}

	return a.exec();
}
