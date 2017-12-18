QT -= core
TEMPLATE = app
CONFIG += console
CONFIG += c++11
#CONFIG -= app_bundle

HEADERS += \
    testcanapi.h

SOURCES += main.cpp \
    testcanapi.cpp

#INCLUDEPATH += $$PWD/../CANAPI
#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin -lCANAPI
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin -lCANAPId
#DEPENDPATH += $$OUT_PWD/../bin

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../CANAPI/PCANBasic/x64/ -lPCANBasic
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../CANAPI/PCANBasic/x64/ -lPCANBasic
#DEPENDPATH += $$PWD/../CANAPI/PCANBasic/x64

include(../CANAPI/CANAPI.pri)
