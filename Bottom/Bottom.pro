#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T09:27:16
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = $$qtLibraryTarget(Bottom)
TEMPLATE = lib
DEFINES += BOTTOM_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        bottom.cpp

HEADERS += \
        bottom.h \
        bottom_global.h
FORMS += \
        bottom.ui

DESTDIR = ../bin

include(../CANAPI/CANAPI.pri)

-std=c++11
