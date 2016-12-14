#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T00:51:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BaseStation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../DWM1000/DWM1000.cpp

HEADERS  += mainwindow.h \
    ../DWM1000/DWM1000.h

FORMS    += mainwindow.ui

CONFIG += c++11
