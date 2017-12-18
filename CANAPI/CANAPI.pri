# This file is created for other subdirs use CANAPI library
# look out: the subdir who call CANAPI should be in the same dir which CANAPI in

INCLUDEPATH += $$PWD/../CANAPI
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../bin -lCANAPI
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../bin -lCANAPId
DEPENDPATH += $$OUT_PWD/../bin

INCLUDEPATH += $$PWD/../CANAPI/PCANBasic/Include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../CANAPI/PCANBasic/x64/ -lPCANBasic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../CANAPI/PCANBasic/x64/ -lPCANBasic
DEPENDPATH += $$PWD/../CANAPI/PCANBasic/x64
