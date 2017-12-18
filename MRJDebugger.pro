#-------------------------------------------------
#
# Project created by QtCreator 2016-12-26T17:20:33
#
#-------------------------------------------------
TEMPLATE = subdirs
SUBDIRS += \
    CANAPI \
    Bottom \
    Move \
    PID \
    SetAndHealth \
    OscilloScope \
    MainWindow
#    TestCANAPI \

#MainWindow.depends = Bottom
#MainWindow.depends = SetAndHealth
#MainWindow.depends = Move
#MainWindow.depends = OscilloScope
#MainWindow.depends = PID
#Bottom.depends       = CANAPI
#SetAndHealth.depends = CANAPI
#Move.depends         = CANAPI
#OscilloScope.depends = CANAPI
#PID.depends          = CANAPI
CONFIG += ordered #按照目录列表的顺序处理它们
CONFIG += c++11
