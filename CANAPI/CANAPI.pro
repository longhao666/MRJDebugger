#-------------------------------------------------
#
# Project created by QtCreator 2016-12-11T18:31:03
#
#-------------------------------------------------

QT       -= gui

TARGET = $$qtLibraryTarget(CANAPI)
TEMPLATE = lib
DESTDIR = ../bin

DEFINES += CANAPI_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
  $$PWD/usercontroloncan.cpp \
  $$PWD/joint.cpp \
  $$PWD/gripper.cpp \
  $$PWD/driveroncan.cpp \
  $$PWD/driver.cpp \
  $$PWD/module.cpp

HEADERS += \
  $$PWD/CANAPI_global.h \
  $$PWD/usercontroloncan.h \
  $$PWD/joint.h \
  $$PWD/gripper.h \
  $$PWD/driveroncan.h \
  $$PWD/driver.h \
  $$PWD/module.h

INCLUDEPATH += $$PWD/PCANBasic/Include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/PCANBasic/x64/ -lPCANBasic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/PCANBasic/x64/ -lPCANBasic
DEPENDPATH += $$PWD/PCANBasic/x64


#INCLUDEPATH += $$PWD/PCANBasic/Include
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/PCANBasic/win32/ -lPCANBasic
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/PCANBasic/xwin32/ -lPCANBasic
#DEPENDPATH += $$PWD/PCANBasic/win32
