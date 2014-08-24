#-------------------------------------------------
#
# Project created by QtCreator 2014-08-11T15:32:04
#
#-------------------------------------------------

QT += core gui
QT += network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeoPosition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sample.cpp \
    logger.cpp \
    databasedriver.cpp \
    configurator.cpp

HEADERS  += mainwindow.h \
    sample.h \
    logger.h \
    databasedriver.h \
    configurator.h

FORMS    += mainwindow.ui

CONFIG += c++11
