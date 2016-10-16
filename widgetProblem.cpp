#include "widgetProblem.h"

WidgetProblem::WidgetProblem(QWidget *parent) : QWidget(parent)
{

	setupUI();
	setupConnection();
	setupMain();
}

void WidgetProblem::clearProblem()
{
	list.clear();
}

void WidgetProblem::addProblem(const QImage &image)
{
	list.append(image);
}

void WidgetProblem::updateProblem()
{
	for (QLayoutItem *item; layout() && (item = layout()->takeAt(0)) != 0; delete item);
	if (layout())
		delete layout();
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 20, 0, 20);
	layout->setSpacing(0);

	for (int i = 0; i < list.length(); ++i)
	{
		QLabel *label = new QLabel;
		layout->addWidget(label);
		label->resize(800, 600);
		label->setPixmap(QPixmap::fromImage(list[i].scaled(800, list[i].height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
		label->setMargin(0);
	}
	layout->addStretch();
	setLayout(layout);
}

void WidgetProblem::setupUI()
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void WidgetProblem::setupConnection()
{

}

void WidgetProblem::setupMain()
{

}
