#-------------------------------------------------
#
# Project created by QtCreator 2017-09-15T11:14:08
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = $$qtLibraryTarget(Move)
TEMPLATE = lib
DEFINES += MOVE_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
  move.cpp \
    Algorithm/motioncontrol.cpp

HEADERS += \
  move.h \
  move_global.h \
    Algorithm/motioncontrol.h

FORMS += \
  move.ui

DESTDIR = ../bin

include(../CANAPI/CANAPI.pri)

INCLUDEPATH += $$PWD/Algorithm
