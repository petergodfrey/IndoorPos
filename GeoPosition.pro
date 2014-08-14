#-------------------------------------------------
#
# Project created by QtCreator 2014-08-11T15:32:04
#
#-------------------------------------------------

QT += core gui

QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeoPosition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    reader.cpp \
    sample.cpp \
    logger.cpp \
    socket.cpp

HEADERS  += mainwindow.h \
    reader.h \
    sample.h \
    logger.h \
    socket.h

FORMS    += mainwindow.ui

CONFIG += c++11
