#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "usercontroloncan.h"
#include "bottom.h"
#include "move.h"
#include "pid.h"
#include "setandhealth.h"
#include "oscilloscope.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    UserControlOnCan *can1 = NULL;  // 标记所用的CAN总线
    Joint *jointBeingUsed = NULL;   // 标记所控制的关节模块，初始化时还未设置

signals:
    void widgetAllReady();

private:
    Ui::MainWindow* ui;
    Bottom* bottom = NULL;                             // 控制的基础部分
    Move* moveComponent = NULL;
    PID* pid = NULL;
    SetAndHealth* setAndHealth = NULL;
    OscilloScope* oscilloScope = NULL;
};

#endif // MAINWINDOW_H
