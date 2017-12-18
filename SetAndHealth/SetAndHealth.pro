#-------------------------------------------------
#
# Project created by QtCreator 2017-09-15T16:55:30
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = $$qtLibraryTarget(SetAndHealth)
TEMPLATE = lib
DEFINES += SETANDHEALTH_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS


SOURCES += \
        setandhealth.cpp

HEADERS += \
        setandhealth.h \
        setandhealth_global.h

FORMS += \
  setandhealth.ui

DESTDIR = ../bin

include(../CANAPI/CANAPI.pri)
