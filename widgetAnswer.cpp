#include "widgetAnswer.h"

WidgetAnswer::WidgetAnswer(QWidget *parent) : QWidget(parent)
{
	setupUI();
	setupConnection();
	setupMain();
}

void WidgetAnswer::toDanXuan(const QList<QString> &list)
{
	clearLayout();
	type = DanXuan;
	QButtonGroup btn_group;
	QGridLayout *layout	= new QGridLayout;
	for (int i = 0; i < list.length(); ++i)
	{
		QRadioButton *button = new QRadioButton(list[i]);
		layout->addWidget(button, i / 5, i % 5, Qt::AlignCenter);
		btn_group.addButton(button, i);
	}
	layout->setContentsMargins(0, 5, 0, 5);
	setLayout(layout);
}

void WidgetAnswer::toDuoXuan(const QList<QString> &list)
{
	clearLayout();
	type = DuoXuan;
	QGridLayout *layout	= new QGridLayout;
	for (int i = 0; i < list.length(); ++i)
	{
		QCheckBox *cbox = new QCheckBox(list[i]);
		layout->addWidget(cbox, i / 5, i % 5, Qt::AlignCenter);
	}
	layout->setContentsMargins(0, 5, 0, 5);
	setLayout(layout);
}

void WidgetAnswer::toQiuJie(const QList<QString> &list)
{
	clearLayout();
	type = QiuJie;
	QVBoxLayout *layout = new QVBoxLayout;
	for (int i = 0; i < list.length(); ++i)
	{
		QLabel *label = new QLabel(list[i]);
		QLineEdit *edit= new QLineEdit;
		QHBoxLayout *tmp = new QHBoxLayout;
		tmp->addWidget(label);
		tmp->addWidget(edit);
		layout->addLayout(tmp);
	}
	layout->setContentsMargins(0, 5, 0, 5);
	setLayout(layout);
}

void WidgetAnswer::toYueDu(const QList<QString> &list)
{
	clearLayout();
	type = YueDu;
	QVBoxLayout *layout = new QVBoxLayout;
	for (int i = 0; i < list.length(); ++i)
	{
		QLabel *label = new QLabel(list[i]);
		QLineEdit *edit= new QLineEdit;
		QHBoxLayout *tmp = new QHBoxLayout;
		tmp->addWidget(label);
		tmp->addWidget(edit);
		layout->addLayout(tmp);
	}
	layout->setContentsMargins(0, 5, 0, 5);
	setLayout(layout);
}

void WidgetAnswer::toWanShan(const QList<QString> &list)
{
	clearLayout();
	type = WanShan;
	QVBoxLayout *layout = new QVBoxLayout;
	for (int i = 0; i < list.length(); ++i)
	{
		QLabel *label = new QLabel(list[i]);
		QLineEdit *edit= new QLineEdit;
		QHBoxLayout *tmp = new QHBoxLayout;
		tmp->addWidget(label);
		tmp->addWidget(edit);
		layout->addLayout(tmp);
	}
	layout->setContentsMargins(0, 5, 0, 5);
	setLayout(layout);
}

QList<QString> WidgetAnswer::getAnswer()
{
	QList<QString> ret;
	if (type == DanXuan)
	{
		for (int i = 0; i < layout()->count(); ++i)
		{
			QLayoutItem *item = layout()->itemAt(i);
			QRadioButton *button = static_cast<QRadioButton *> (item->widget());
			if (button->isChecked())
			{
				ret.append(QString::number(i));
			}
		}
		if (ret.length() == 0)
		{
			ret.append("");
		}
	}
	else if (type == DuoXuan)
	{
		QString str;
		for (int i = 0; i < layout()->count(); ++i)
		{
			QLayoutItem *item = layout()->itemAt(i);
			QCheckBox *button = static_cast<QCheckBox *> (item->widget());
			if (button->isChecked())
			{
				str += QString::number(i);
			}
		}
		ret.append(str);
		if (ret.length() == 0)
		{
			ret.append("");
		}
	}
	else if (type == QiuJie || type == YueDu || type == WanShan)
	{
		for (int i = 0; i < layout()->count(); ++i)
		{
			QLayoutItem *item = layout()->itemAt(i);
			QHBoxLayout *tmp = static_cast<QHBoxLayout *> (item->layout());
			QLineEdit *edit = static_cast<QLineEdit *> (tmp->itemAt(1)->widget());
			ret.append(edit->text());
		}
	}
	return ret;
}

void WidgetAnswer::clearLayout()
{
	for (QLayoutItem *item; layout() && (item = layout()->takeAt(0)) != 0; delete item);
	if (layout())
		delete layout();
}

void WidgetAnswer::setupUI()
{
	setContentsMargins(0, 0, 0, 0);
	setFixedWidth(300);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

void WidgetAnswer::setupConnection()
{

}

void WidgetAnswer::setupMain()
{

}
