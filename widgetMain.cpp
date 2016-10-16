#include "widgetMain.h"

WidgetMain::WidgetMain(QWidget *parent) : QWidget(parent)
{
	display_score = false;
	count_score = 0;
	count_all = 0;
	count_correct = 0;
	count_wrong = 0;
	count_ignore = 0;
	pages = new QStackedWidget(this);

	setupUI();
	setupConnection();
	setupMain();
}

bool WidgetMain::addProblem(const QString &path, ProblemType type)
{
	WidgetPage *page = new WidgetPage(this);
	bool ret = page->setProblem(path, type, display_score);
	connect(page, SIGNAL(previousProblem()),
			this, SLOT(previousPage()));
	connect(page, SIGNAL(nextProblem()),
			this, SLOT(nextPage()));
	connect(page, SIGNAL(addScore(double)),
			this, SLOT(addScore(double)));
	connect(page, SIGNAL(addCorrect(QString, ProblemType)),
			this, SLOT(addCorrect(QString, ProblemType)));
	connect(page, SIGNAL(addWrong(QString, ProblemType)),
			this, SLOT(addWrong(QString, ProblemType)));
	connect(page, SIGNAL(addIgnore(QString, ProblemType)),
			this, SLOT(addIgnore(QString, ProblemType)));
	connect(this, SIGNAL(stateUpdated(double,int,int,int,int,int)),
			page->tool(), SLOT(stateUpdated(double,int,int,int,int,int)));
	connect(page, SIGNAL(updateRight(QString,ProblemType,bool)),
			this, SLOT(updateRight(QString,ProblemType,bool)));

	pages->addWidget(page);
	return ret;
}

void WidgetMain::setDisplayScore(bool flag)
{
	display_score = flag;
}

void WidgetMain::finish(unsigned int delta_time)
{
	if (display_score)
	{
		for (int i = 0; i < pages->count(); ++i)
		{
			WidgetPage *x = static_cast<WidgetPage *> (pages->widget(i));
			x->evaluate();
		}
	}
	QString text;
	text += tr("总共:%1题\n正确:%2题\n错误:%3题\n没做:%4题\n%5正确率:%6\%")
			.arg(count_all)
			.arg(count_correct)
			.arg(count_wrong)
			.arg(count_ignore)
			.arg(display_score ? "" : tr("剩余:%1题\n").arg(count_all - count_correct - count_wrong - count_ignore))
			.arg(QString::number(double(count_correct) / double(count_all) * 100, 'f', 0));

	if (display_score)
	{
		unsigned int delta = delta_time;
		int h = delta / 3600;
		int m = (delta - h * 3600) / 60;
		int s = (delta - h * 3600 - m * 60);
		QString tm = QString().sprintf("%02d时%02d分%02d秒", h, m, s);
		text += tr("\n耗时:%1\n分数:%2").arg(tm).arg(count_score);
	}
	QMessageBox::information(this, APP_NAME, text, QMessageBox::Ok, QMessageBox::Ok);

	QMessageBox::StandardButton ret = QMessageBox::warning(this, APP_NAME, tr("想要保存做题情况吗？"),
														   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (ret == QMessageBox::Yes)
	{
		QFileDialog dialog(this, tr("保存做题情况"), QString(), "文本文件 (*.txt)");
		dialog.setAcceptMode(QFileDialog::AcceptSave);
		dialog.setDefaultSuffix("txt");
		if (dialog.exec() == QFileDialog::Accepted)
		{
			QFile *file = new QFile(dialog.selectedFiles()[0]);
			if (file->open(QFile::WriteOnly))
			{
				QTextStream stream(file);
				stream << text + '\n';

				stream << tr("\n【正确的题目ID】\n");
				for (int i = 0; i < 5; ++i)
				{
					if (correct[i].size())
					{
						stream << typeToName[i] + ":\n";
						for (set<QString>::iterator it = correct[i].begin(); it != correct[i].end(); ++it)
						{
							stream << *it << '\n';
						}
						stream << "\n";
					}
				}

				stream << tr("\n【错误的题目ID】\n");
				for (int i = 0; i < 5; ++i)
				{
					if (wrong[i].size())
					{
						stream << typeToName[i] + ":\n";
						for (set<QString>::iterator it = wrong[i].begin(); it != wrong[i].end(); ++it)
						{
							stream << *it << '\n';
						}
						stream << "\n";
					}
				}

				stream << tr("\n【没做的题目ID】\n");
				for (int i = 0; i < 5; ++i)
				{
					if (ignore[i].size())
					{
						stream << typeToName[i] + ":\n";
						for (set<QString>::iterator it = ignore[i].begin(); it != ignore[i].end(); ++it)
						{
							stream << *it << '\n';
						}
						stream << "\n";
					}
				}

				file->close();
				delete file;
			}
		}
	}
}

bool WidgetMain::maybeFinish(unsigned int delta_time)
{
	QMessageBox::StandardButton ret = QMessageBox::warning(this, APP_NAME, tr("想要结束吗？"),
														   QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
	if (ret == QMessageBox::Yes)
	{
		finish(delta_time);
		return true;
	}
	return false;
}

void WidgetMain::loadSetting(const QString &APP_PATH)
{
	for (int i = 0; i < 5; ++i)
	{
		right[i].clear();
		QString path = APP_PATH + typeToPinyin[i] + QDir::separator();
		QFile *file = new QFile(path + "right.txt");
		if (file->open(QFile::ReadOnly))
		{
			QTextStream stream(file);
			int cnt = stream.readLine().toInt();
			for (int j = 0; j < cnt; ++j)
			{
				right[i].insert(stream.readLine());
			}
		}
		file->close();
		delete file;
	}
}

void WidgetMain::saveSetting(const QString &APP_PATH)
{
	for (int i = 0; i < 5; ++i)
	{
		QString path = APP_PATH + typeToPinyin[i] + QDir::separator();
		QFile *file = new QFile(path + "right.txt");
		if (file->open(QFile::WriteOnly))
		{
			QTextStream stream(file);
			stream << int(right[i].size()) << '\n';
			for (set<QString>::iterator it = right[i].begin(); it != right[i].end(); ++it)
			{
				stream << *it << '\n';
			}
		}
		file->close();
		delete file;
	}
}

void WidgetMain::previousPage()
{
	int index = pages->currentIndex();
	if (index > 0)
	{
		pages->setCurrentIndex(index - 1);
		emit stateUpdated(count_score, count_correct, count_wrong, count_ignore, count_all, pages->currentIndex() + 1);
	}
}

void WidgetMain::nextPage()
{
	int index = pages->currentIndex();
	if (index +	1 < pages->count())
	{
		pages->setCurrentIndex(index + 1);
		emit stateUpdated(count_score, count_correct, count_wrong, count_ignore, count_all, pages->currentIndex() + 1);
	}
	else if (index + 1 == pages->count())
	{
		emit wantFinish();
	}
}

void WidgetMain::addScore(const double &score)
{
	count_score += score;
	emit stateUpdated(count_score, count_correct, count_wrong, count_ignore, count_all, pages->currentIndex() + 1);
}

void WidgetMain::addCorrect(const QString &ID, ProblemType type)
{
	count_correct += 1;
	correct[type].insert(ID);
	emit stateUpdated(count_score, count_correct, count_wrong, count_ignore, count_all, pages->currentIndex() + 1);
}

void WidgetMain::addWrong(const QString &ID, ProblemType type)
{
	count_wrong += 1;
	wrong[type].insert(ID);
	emit stateUpdated(count_score, count_correct, count_wrong, count_ignore, count_all, pages->currentIndex() + 1);
}

void WidgetMain::addIgnore(const QString &ID, ProblemType type)
{
	count_ignore += 1;
	ignore[type].insert(ID);
	emit stateUpdated(count_score, count_correct, count_wrong, count_ignore, count_all, pages->currentIndex() + 1);
}

void WidgetMain::updateRight(const QString &id, ProblemType type, bool ins)
{
	if (ins)
	{
		right[type].insert(id);
	}
	else
	{
		set<QString>::iterator it = right[type].find(id);
		if (it != right[type].end())
		{
			right[type].erase(it);
		}
	}
}

void WidgetMain::work(const QList<pair<QString, ProblemType> > &problems)
{
	for (int i = 0; i < problems.length(); ++i)
	{
		addProblem(problems[i].first, problems[i].second);
	}
	if (problems.length())
	{
		pages->setCurrentIndex(0);
		count_all = problems.length();
		emit stateUpdated(count_score, count_correct, count_wrong, count_ignore, count_all, pages->currentIndex() + 1);
	}
}

void WidgetMain::setupUI()
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setSpacing(0);
	layout->addWidget(pages);
	setLayout(layout);
}

void WidgetMain::setupConnection()
{

}

void WidgetMain::setupMain()
{

}
