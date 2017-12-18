#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    can1(new UserControlOnCan),     // 调用API但还未开启线程
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    moveComponent = new Move(can1, jointBeingUsed, this);
    ui->verticalLayout_Move->addWidget(moveComponent);

    bottom = new Bottom(can1, jointBeingUsed, this);
    ui->verticalLayout_Bottom->addWidget(bottom);

    pid = new PID(can1, jointBeingUsed, this);
    ui->verticalLayout_PID->addWidget(pid);

    setAndHealth = new SetAndHealth(can1, jointBeingUsed, this);
    ui->verticalLayout_SetAndHealth->addWidget(setAndHealth);

    oscilloScope = new OscilloScope(can1, jointBeingUsed, this);
    ui->verticalLayout_OscilloScope->addWidget(oscilloScope);

    QObject::connect( bottom, &Bottom::cmbIDChanged,
                      moveComponent, &Move::moveInitialize );
    QObject::connect( bottom, &Bottom::cmbIDChanged,
                      pid, &PID::PIDInitialize );
    QObject::connect( bottom, &Bottom::cmbIDChanged,
                      setAndHealth, &SetAndHealth::SetAndHealthInitialize );
    QObject::connect( bottom, &Bottom::cmbIDChanged,
                      oscilloScope, &OscilloScope::OscilloScopeInitialize );
    QObject::connect( bottom, &Bottom::btnSave_clicked,
                      pid, &PID::btnSave_clicked );
    QObject::connect( bottom, &Bottom::btnLoad_clicked,
                      pid, &PID::btnLoad_clicked );
    QObject::connect( this, &MainWindow::widgetAllReady,
                      bottom, &Bottom::waitingForWidgetReady );
    QObject::connect( setAndHealth, &SetAndHealth::IDChanged,
                      bottom, &Bottom::waitingForWidgetReady );
    QObject::connect( setAndHealth, &SetAndHealth::ZeroPositionSeted,
                      moveComponent, &Move::ClickStopButton );

    emit this->widgetAllReady();
}

MainWindow::~MainWindow()
{
  delete oscilloScope;
  delete moveComponent;
  delete bottom;
  delete pid;
  delete setAndHealth;
  delete ui;
  delete can1;
}
