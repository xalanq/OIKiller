#include "widgetPage.h"

WidgetPage::WidgetPage(QWidget *parent) : QWidget(parent)
{
	widget_problem = new WidgetProblem(this);
	widget_answer = new WidgetAnswer(this);
	widget_tool = new WidgetTool(this);
	btn_submit = new QPushButton(tr("确认"), this);
	cbox_ignore = new QCheckBox(tr("我已掌握"), this);
	view = new QLabel(this);
	papers = false;

	setupUI();
	setupConnection();
	setupMain();
}

bool WidgetPage::setProblem(const QString &path, ProblemType type, bool display_score)
{
	QFile *file = new QFile(path + "config");
	if (file->open(QIODevice::ReadOnly))
	{
		QTextStream stream(file);
		current_answer.clear();
		QList<QString> display;
		if (type == DanXuan || type == DuoXuan)
		{
			int cnt = stream.readLine().toInt();
			for (int i = 0; i < cnt; ++i)
			{
				display.append(QString("%1").arg(char('A' + i)));
			}
		}
		int cnt = stream.readLine().toInt();
		for (int i = 0; i < cnt; ++i)
		{

			QString str;
			double score = stream.readLine().toDouble();
			int tot = stream.readLine().toInt();

			if (type == QiuJie || type == YueDu || type == WanShan)
			{
				str = tr("第%1个空").arg(i + 1);
				if (display_score)
				{
					str += tr("(%1分)").arg(score);
				}
				display.append(str + ":");
			}
			QList<QString> list;
			for (int j = 0; j < tot; ++j)
			{
				str = stream.readLine();
				list.append(str);
			}
			current_answer.append(pair<QList<QString>, double> (list, score));
		}
		widget_problem->clearProblem();
		for (; !stream.atEnd(); )
		{
			QString str;
			str = stream.readLine();
			QImage image;
			if (image.load(path + str))
			{
				widget_problem->addProblem(image);
			}
		}
		widget_problem->updateProblem();
		if (type == DanXuan)
		{
			widget_answer->toDanXuan(display);
		}
		else if (type == DuoXuan)
		{
			widget_answer->toDuoXuan(display);
		}
		else if (type == QiuJie)
		{
			widget_answer->toQiuJie(display);
		}
		else if (type == YueDu)
		{
			widget_answer->toYueDu(display);
		}
		else if (type == WanShan)
		{
			widget_answer->toWanShan(display);
		}
		delete file;
		current_type = type;
		widget_tool->setProblemID(QFileInfo(path).dir().dirName());
		widget_tool->setDisplay(display_score);
		papers = display_score;
		if (papers)
		{
			btn_submit->hide();
		}
		return true;
	}
	delete file;
	return false;
}

void WidgetPage::setResult(const QList<QString> &wrong, const QList<QString> &correct, const QString &explaination)
{
	QString text = "";
	if (wrong.length() == 1)
	{
		text += tr("正确答案:%1\n").arg(correct[0]);
		text += tr("\n你的答案:%1\n").arg(wrong[0]);
	}
	else if (wrong.length() > 1)
	{
		text += tr("正确答案:\n");
		for (int i = 0; i < correct.length(); ++i)
		{
			text += tr("第%1个空:%2\n").arg(i + 1).arg(correct[i]);
		}
		text += tr("\n你的答案:\n");
		for (int i = 0; i < wrong.length(); ++i)
		{
			text += tr("第%1个空:%2\n").arg(i + 1).arg(wrong[i]);
		}
	}
	if (explaination != "")
	{
		text += tr("\n解释:\n%1\n").arg(explaination);
	}
	text.replace("\n", "<br>");
	view->setText(text);
	view->show();
}

WidgetTool *WidgetPage::tool()
{
	return widget_tool;
}

void WidgetPage::evaluate()
{
	if (!btn_submit->isEnabled())
	{
		return;
	}
	QList<QString> wrong, correct;
	double tot_score = 0;
	bool is_correct = true;
	bool is_ignore = true;
	QList<QString> answer = widget_answer->getAnswer();
	for (int i = 0; i < current_answer.length(); ++i)
	{
		QList<QString> &list = current_answer[i].first;
		double score = current_answer[i].second;
		bool sub_correct = false;
		QString text_wrong = "", text_correct = "";
		if (current_type == DanXuan || current_type == DuoXuan)
		{
			string y = answer[i].toStdString();
			for (int j = 0; j < int(y.size()); ++j)
			{
				y[j] = y[j] - '0' + 'A';
			}
			sort(y.begin(), y.end());
			if (list.length())
			{
				string x = list[0].toUpper().toStdString();
				sort(x.begin(), x.end());
				if (x == y)
				{
					sub_correct = true;
				}
			}
			text_wrong += QString::fromStdString(y).toHtmlEscaped();
			if (y.size())
			{
				is_ignore = false;
			}
		}
		else if (current_type == QiuJie || current_type == YueDu || current_type == WanShan)
		{
			QString y = answer[i].simplified();
			y.replace(" ", "");
			for (int j = 0; j < list.length(); ++j)
			{
				QString x = list[j].simplified();
				x.replace(" ", "");
				if (x == y)
				{
					sub_correct = true;
				}
			}
			text_wrong += answer[i].toHtmlEscaped();
			if (y.length())
			{
				is_ignore = false;
			}
		}
		if (current_type == DanXuan || current_type == DuoXuan)
		{
			text_correct += list[0].toUpper().toHtmlEscaped();
		}
		else if (current_type == QiuJie || current_type == YueDu || current_type == WanShan)
		{
			for (int j = 0; j < list.length(); ++j)
			{
				text_correct += list[j].toHtmlEscaped();
				if (j < list.length() - 1)
				{
					text_correct += tr(" <strong>或者</strong> ");
				}
			}
		}
		if (sub_correct)
		{
			tot_score += score;
			wrong.append(text_wrong);
			correct.append(text_correct);
		}
		else
		{
			wrong.append(QString("<font color='red'>") + text_wrong + "</font>");
			correct.append(QString("<font color='red'>") + text_correct + "</font>");
			is_correct = false;
		}
	}
	if (!papers)
	{
		setResult(wrong, correct);
	}
	emit addScore(tot_score);
	if (is_ignore)
	{
		emit addIgnore(widget_tool->getProblemID(), current_type);
	}
	else if (is_correct)
	{
		if (!papers)
		{
			cbox_ignore->show();
		}
		emit addCorrect(widget_tool->getProblemID(), current_type);
	}
	else
	{
		emit addWrong(widget_tool->getProblemID(), current_type);
	}
	btn_submit->setEnabled(false);
	widget_answer->setEnabled(false);
}

void WidgetPage::slotPreviousProblem()
{
	emit previousProblem();
}

void WidgetPage::slotNextProblem()
{
	emit nextProblem();
}

void WidgetPage::slotUpdateRight(bool ins)
{
	emit updateRight(widget_tool->getProblemID(), current_type, ins);
}

void WidgetPage::setupUI()
{
	view->setWordWrap(true);
	view->setMaximumWidth(300);
	btn_submit->setFixedHeight(btn_submit->size().height() * 1.4);
	btn_submit->setFixedWidth(300);
	cbox_ignore->hide();

	QHBoxLayout *layout = new QHBoxLayout;

	QVBoxLayout *left = new QVBoxLayout;
	left->addWidget(widget_tool);
	left->addWidget(widget_answer);
	left->addWidget(btn_submit);
	left->addWidget(view);
	left->addWidget(cbox_ignore);
	left->addStretch();
	left->setAlignment(btn_submit, Qt::AlignCenter);
	left->setContentsMargins(10, 0, 5, 0);

	layout->addLayout(left);

	QScrollArea *area = new QScrollArea;
	area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	area->setWidget(widget_problem);
	area->setWidgetResizable(true);

	QPalette Pal(palette());
	Pal.setColor(QPalette::Background, Qt::white);
	area->setAutoFillBackground(true);
	area->setPalette(Pal);

	// area->setStyleSheet("background-color: white;" + area->styleSheet());
	area->setFrameShape(QFrame::NoFrame);
	layout->setMargin(0);
	// layout->setContentsMargins(5, 0, 0, 0);
	// layout->setSpacing(0);
	layout->addWidget(area);


	setLayout(layout);
	setFixedWidth(1140);
	resize(1140, 800);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
}

void WidgetPage::setupConnection()
{
	connect(btn_submit, SIGNAL(clicked(bool)),
			this, SLOT(evaluate()));
	connect(widget_tool, SIGNAL(previousProblem()),
			this, SLOT(slotPreviousProblem()));
	connect(widget_tool, SIGNAL(nextProblem()),
			this, SLOT(slotNextProblem()));
	connect(cbox_ignore, SIGNAL(toggled(bool)),
			this, SLOT(slotUpdateRight(bool)));
}

void WidgetPage::setupMain()
{

}

void WidgetPage::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
	{
		if (!papers)
		{
			evaluate();
		}
	}
}
