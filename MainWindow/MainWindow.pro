#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T09:29:30
#
#-------------------------------------------------
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = MRJDebugger
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

DESTDIR = ../bin

include(../CANAPI/CANAPI.pri)

INCLUDEPATH += $$PWD/../Bottom
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin -lBottom
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin -lBottomd

INCLUDEPATH += $$PWD/../Move
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin -lMove
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin -lMoved

INCLUDEPATH += $$PWD/../PID
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin -lPID
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin -lPIDd

INCLUDEPATH += $$PWD/../SetAndHealth
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin -lSetAndHealth
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin -lSetAndHealthd

INCLUDEPATH += $$PWD/../OscilloScope
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin -lOscilloScope
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin -lOscilloScoped

INCLUDEPATH += $$PWD/../OscilloScope/qwt/include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../OscilloScope/qwt/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../OscilloScope/qwt/lib/ -lqwtd
DEPENDPATH += $$PWD/../OscilloScope/qwt/lib

RESOURCES += \
    $$PWD/../images/image.qrc
win32{
  RC_FILE = MRJDebugger.rc
  DISTFILES += \
      MRJDebugger.rc
}
