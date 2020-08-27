#-------------------------------------------------
#
# Project created by QtCreator 2020-08-24T18:14:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    components.cpp \
    utils.cpp \
    setform.cpp \
    gamewindow.cpp \
    gamewidget.cpp \
    loadform.cpp

HEADERS  += mainwidget.h \
    utils.h \
    setform.h \
    components.h \
    gamewindow.h \
    gamewidget.h \
    loadform.h

FORMS    += mainwidget.ui \
    setform.ui \
    gamewindow.ui \
    gamewidget.ui \
    loadform.ui

QMAKE_CXXFLAGS += -std=c++14

RESOURCES += \
    src.qrc

DISTFILES +=
