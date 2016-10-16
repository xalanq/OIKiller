#include "widgetTool.h"

WidgetTool::WidgetTool(QWidget *parent) : QWidget(parent)
{
	lbl_problem_id = new QLabel(this);
	lbl_problem_count = new QLabel(this);
	lbl_score = new QLabel(this);
	btn_left = new QPushButton(tr("上一题"), this);
	btn_right = new QPushButton(tr("下一题"), this);
	display = false;

	setupUI();
	setupConnection();
	setupMain();
}

void WidgetTool::setProblemID(const QString &name)
{
	ID = name;
	lbl_problem_id->setText(tr("<center><h1>题目ID:%1</h1></center>").arg(name));
}

void WidgetTool::setProblemCount(const int &now, const int &tot, const int &rest)
{
	lbl_problem_count->setText(tr("当前是第%1题，总共%2题%3")
							   .arg(now)
							   .arg(tot)
							   .arg(display ? "" : tr("，剩余%3题").arg(rest)));
}

void WidgetTool::setDisplay(bool flag)
{
	display = flag;
	if (display)
	{
		lbl_score->hide();
	}
	else
	{
		lbl_score->show();
	}
}

QString WidgetTool::getProblemID()
{
	return ID;
}

void WidgetTool::setScore(const int &correct, const int &wrong, const int &ignore, const double &score)
{
	QString text = tr("正确%1题，错误%2题，没做%3题")
				   .arg(display ? "?" : QString::number(correct))
				   .arg(display ? "?" : QString::number(wrong))
				   .arg(display ? "?" : QString::number(ignore));
	lbl_score->setText(text);
}

void WidgetTool::stateUpdated(const double &score, const int &correct, const int &wrong, const int &ignore, const int &all, const int &index)
{
	setProblemCount(index, all, all - correct - wrong - ignore);
	setScore(correct, wrong, ignore, score);
}

void WidgetTool::setupUI()
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(lbl_problem_id);
	layout->addWidget(lbl_problem_count);
	layout->addWidget(lbl_score);

	QHBoxLayout *layout_btn = new QHBoxLayout;
	layout_btn->addWidget(btn_left);
	layout_btn->addWidget(btn_right);
	layout_btn->setContentsMargins(0, 0, 0, 0);

	layout->addLayout(layout_btn);
	layout->setContentsMargins(0, 10, 0, 0);
	setLayout(layout);
}

void WidgetTool::setupConnection()
{
	connect(btn_left, SIGNAL(clicked(bool)),
			this, SIGNAL(previousProblem()));
	connect(btn_right, SIGNAL(clicked(bool)),
			this, SIGNAL(nextProblem()));
}

void WidgetTool::setupMain()
{

}

