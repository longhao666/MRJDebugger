#-------------------------------------------------
#
# Project created by QtCreator 2017-09-15T15:38:40
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = $$qtLibraryTarget(PID)
TEMPLATE = lib
DEFINES += PID_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        pid.cpp

HEADERS += \
        pid.h \
        pid_global.h

FORMS += \
  pid.ui

DESTDIR = ../bin

include(../CANAPI/CANAPI.pri)
