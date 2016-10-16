#include "mainwindow.h"

MainWindow::MainWindow(const QString &path, QWidget *parent)
	: QMainWindow(parent)
{
	APP_PATH = QDir::toNativeSeparators(path + "/");
	Paper = false;
	main_widget = new WidgetMain(this);
	setupUI();
	setupConnection();
	setupMain();
	chooseWay();
}

MainWindow::~MainWindow()
{

}

bool MainWindow::isValidProblem(const QString &path)
{
	if (QDir(path).exists())
	{
		QFile file(path + "config");
		if (file.open(QIODevice::ReadOnly))
		{
			return true;
		}
	}
	return false;
}

void MainWindow::startTimer(QTimer *timer)
{
	Timer = timer;
	delta_time = -1;
	updateTime();
}

void MainWindow::updateTime()
{
	++delta_time;
	unsigned int delta = delta_time;
	int h = delta / 3600;
	int m = (delta - h * 3600) / 60;
	int s = (delta - h * 3600 - m * 60);
	QString text = QString().sprintf("%02d时%02d分%02d秒", h, m, s);
	setWindowTitle(APP_NAME + " - " + text);
}

void MainWindow::setupUI()
{
	setCentralWidget(main_widget);
	setWindowTitle(APP_NAME);
	setMinimumWidth(1140);
	setMaximumWidth(1140);
	resize(1140, 800);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

	QRect screenGeometry = QApplication::desktop()->screenGeometry();
	int x = (screenGeometry.width() - width()) / 2;
	int y = (screenGeometry.height() - height()) / 2;
	move(x, y);
}

void MainWindow::setupConnection()
{
	connect(main_widget, SIGNAL(wantFinish()),
			this, SLOT(close()));
}

void MainWindow::setupMain()
{
	main_widget->loadSetting(APP_PATH);
}

void MainWindow::chooseWay()
{
	QList<QString> list;
	for (int i = 0; i < 5; ++i)
	{
		list.append(typeToName[i]);
	}
	list.append(tr("模拟考试"));
	list.append(tr("关于"));
	list.append(tr("捐赠"));
	DialogChoose choose1(list);
	if (choose1.exec() == QDialog::Accepted)
	{
		int id1 = choose1.getAnswer();
		list.clear();
		if (id1 < 5)
		{
			list.append(tr("指定题目"));
			list.append(tr("剩下题目"));
			list.append(tr("全部题目"));
			DialogChoose choose2(list);
			if (choose2.exec() == QDialog::Accepted)
			{
				int id2 = choose2.getAnswer();
				if (id2 == 0)
				{
					workZhi(ProblemType(id1));
				}
				else if(id2 == 1)
				{
					workRest(ProblemType(id1));
				}
				else if(id2 == 2)
				{
					workQuanBu(ProblemType(id1));
				}
			}
			else
			{
				chooseWay();
			}
		}
		else if (id1 == 5)
		{
			workMoNi();
		}
		else if (id1 == 6)
		{
			workAbout();
			chooseWay();
		}
		else if (id1 == 7)
		{
			workDonate();
			chooseWay();
		}
	}
	else
	{
		exit(0);
	}
}

void MainWindow::workZhi(ProblemType type)
{
	bool isOK;
	QString name;
	name = QInputDialog::getText(this,
								 APP_NAME,
								 tr("输入题目ID"),
								 QLineEdit::Normal,
								 "",
								 &isOK);
	if (isOK)
	{
		QString path = APP_PATH + typeToPinyin[type] + QDir::separator();
		QList<pair<QString, ProblemType> > problems;
		if (isValidProblem(path + name + QDir::separator()))
		{
			problems.append(pair<QString, ProblemType> (path + name + QDir::separator(), type));
		}
		if (problems.size())
		{
			main_widget->work(problems);
			return;
		}
	}
	chooseWay();
}

void MainWindow::workRest(ProblemType type)
{
	QString path = APP_PATH + typeToPinyin[type] + QDir::separator();
	QFile *file = new QFile(path + "right.txt");
	set<QString> rt;
	QList<pair<QString, ProblemType> > problems;
	if (file->open(QFile::ReadOnly))
	{
		QTextStream stream(file);
		int cnt = stream.readLine().toInt();
		for (int i = 0; i < cnt; ++i)
		{
			rt.insert(stream.readLine());
		}
	}
	file->close();
	delete file;
	file = new QFile(path + "problemset.txt");
	if (file->open(QFile::ReadOnly))
	{
		QTextStream stream(file);
		int cnt = stream.readLine().toInt();
		for (int i = 0; i < cnt; ++i)
		{
			QString name = stream.readLine();
			if (rt.find(name) == rt.end() && isValidProblem(path + name + QDir::separator()))
			{
				problems.append(pair<QString, ProblemType> (path + name + QDir::separator(), type));
			}
		}
	}
	file->close();
	delete file;
	if (problems.size())
	{
		std::shuffle(problems.begin(), problems.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
		main_widget->work(problems);
	}
}

void MainWindow::workQuanBu(ProblemType type)
{
	QString path = APP_PATH + typeToPinyin[type] + QDir::separator();
	QFile *file = new QFile(path + "problemset.txt");
	QList<pair<QString, ProblemType> > problems;
	if (file->open(QFile::ReadOnly))
	{
		QTextStream stream(file);
		int cnt = stream.readLine().toInt();
		for (int i = 0; i < cnt; ++i)
		{
			QString name = stream.readLine();
			if (isValidProblem(path + name + QDir::separator()))
			{
				problems.append(pair<QString, ProblemType> (path + name + QDir::separator(), type));
			}
		}
	}
	file->close();
	delete file;
	if (problems.size())
	{
		main_widget->work(problems);
	}
}

void MainWindow::workMoNi()
{
	QList<QString> list, papers;
	QString path = APP_PATH + "ShiJuan" + QDir::separator();
	QFile *file = new QFile(path + "problemset.txt");
	QList<pair<QString, ProblemType> > problems;
	if (file->open(QFile::ReadOnly))
	{
		QTextStream stream(file);
		int cnt = stream.readLine().toInt();
		for (int i = 0; i < cnt; ++i)
		{
			QString name = stream.readLine();
			QFile *tmp = new QFile(path + name);
			if (tmp->open(QFile::ReadOnly))
			{
				QString text;
				QTextStream file(tmp);
				text = file.readLine();
				papers.append(name);
				list.append(text);
			}
			tmp->close();
			delete tmp;
		}
	}
	file->close();
	delete file;
	DialogChoose dialog(list);
	if (dialog.exec() == QDialog::Accepted)
	{
		int choose = dialog.getAnswer();
		file = new QFile(path + papers[choose]);
		if (file->open(QFile::ReadOnly))
		{
			QTextStream stream(file);
			stream.readLine();
			for (int i = 0; i < 5; ++i)
			{
				int cnt = stream.readLine().toInt();
				QString problem_path;
				problem_path = APP_PATH + typeToPinyin[i] + QDir::separator();
				for (int j = 0; j < cnt; ++j)
				{
					QString name = stream.readLine();
					problems.append(pair<QString, ProblemType> (problem_path + name + QDir::separator(), ProblemType(i)));
				}
			}
		}
		file->close();
		delete file;
		main_widget->setDisplayScore(true);
		main_widget->work(problems);
		Paper = true;
	}
	else
	{
		chooseWay();
	}
}

void MainWindow::workAbout()
{
	QMessageBox::about(this,
					   tr("About %1")
					   .arg(APP_NAME),
					   tr("<center><b> %1 %2 </b></center> \
						   <p>CopyRight &copy; 2016 by %3.</p> \
						   <p>Contact me: %4 </p> \
						   <p>Blog: <a href=\'http://%5\'>%6</a></p> \
						   <p>Project page: <a href=\'https://%7\'>%8</a></p>\
						   <p>Lisence: <a href=\'%9\'>LGPL v3.0</a></p>")
					   .arg(APP_NAME.toHtmlEscaped())
					   .arg(APP_VERSION.toHtmlEscaped())
					   .arg(AUTHOR_NAME.toHtmlEscaped())
					   .arg(AUTHOR_EMAIL.toHtmlEscaped())
					   .arg(AUTHOR_WEBSITE.toHtmlEscaped())
					   .arg(AUTHOR_WEBSITE.toHtmlEscaped())
					   .arg(APP_PAGE.toHtmlEscaped())
					   .arg(APP_PAGE.toHtmlEscaped())
					   .arg(QDir::fromNativeSeparators(APP_PATH.toHtmlEscaped()) + "LICENSE.txt"));
}

void MainWindow::workDonate()
{
	DialogDonate dialog(this);
	dialog.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (Paper)
	{
		Timer->stop();
	}
	if (main_widget->maybeFinish(delta_time))
	{
		main_widget->saveSetting(APP_PATH);
		event->accept();
	}
	else
	{
		if (Paper)
		{
			Timer->start(1000);
		}
		event->ignore();
	}
}
