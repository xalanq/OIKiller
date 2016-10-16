#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T21:48:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OIKiller
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    widgetProblem.cpp \
    widgetMain.cpp \
    widgetAnswer.cpp \
    dialogChoose.cpp \
    widgetPage.cpp \
    widgetTool.cpp \
    dialogDonate.cpp

HEADERS  += mainwindow.h \
    headers.h \
    author.h \
    widgetProblem.h \
    widgetMain.h \
    widgetAnswer.h \
    dialogChoose.h \
    widgetPage.h \
    widgetTool.h \
    dialogDonate.h

DISTFILES +=

RESOURCES += \
    oikiller.qrc

RC_ICONS = "icon.ico"
