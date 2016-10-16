#ifndef HEADERS_H
#define HEADERS_H

#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <set>
#include <random>
#include <chrono>
// #include <QtGui>
// #include <QtWidgets>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QDialog>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QIcon>
#include <QInputDialog>
#include <QIODevice>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QLocale>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScrollArea>
#include <QSettings>
#include <QString>
#include <QStackedWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QRect>
#include <QTableWidget>
#include <QTextEdit>
#include <QTextStream>
#include <QTranslator>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include "author.h"
using std::vector;
using std::string;
using std::pair;
using std::set;

enum ProblemType {DanXuan = 0, DuoXuan, QiuJie, YueDu, WanShan};

const QString typeToName[5] =
{
	QObject::tr("单选题"),
	QObject::tr("多选题"),
	QObject::tr("问题求解"),
	QObject::tr("阅读程序"),
	QObject::tr("完善程序")
};

const QString typeToPinyin[5] =
{
	"DanXuan",
	"DuoXuan",
	"QiuJie",
	"YueDu",
	"WanShan"
};

#endif // HEADERS_H
