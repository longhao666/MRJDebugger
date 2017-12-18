// Copyright 2017, Inc.
// All rights reserved.
// ----------------------------------------------------------------------------
// Author :
// ----------------------------------------------------------------------------
#ifndef MOVE_H
#define MOVE_H

#include <QWidget>
#include "move_global.h"
#include "./usercontroloncan.h"
#include "./Algorithm/motioncontrol.h"

namespace Ui {
class Move;
}

class MOVESHARED_EXPORT Move : public QWidget {
  Q_OBJECT

public:
  explicit Move(UserControlOnCan *can1,
                  Joint *jointBeingUsed,
                  QWidget *parent = 0);
  virtual ~Move();
  MotionControl * MC = NULL;
  QTimer* timerMove = NULL;
  bool EnableRun = false;
  double bias = 0.0;
  double frequency = 0.0;
  double amplitude = 0.0;
  QString stopButtonOn;
  QString stopButtonOff;
  QString confirmButtonOn;
  QString confirmButtonOff;
  void workModeUpdatetxtBias();
  void txtBiasChangemanualSlider();

public slots:
  void moveInitialize(uint16_t ID);
  void ClickStopButton();

signals:

private slots:
  void on_txtBias_editingFinished();
  void on_cmbWorkMode_currentIndexChanged(int index);
  void on_manualMax_editingFinished();
  void on_manualMin_editingFinished();
  void on_manualSlider_valueChanged(int value);
  void on_confirmButton_clicked();
  void on_stopButton_clicked();
  void on_waveModeCombo_currentIndexChanged(int index);
  void on_frequencyLineEdit_editingFinished();
  void on_AmplitudeLineEdit_editingFinished();
  void slotTimeMoveDone();

private:
  void SetValue(double value);
  Ui::Move *uiMove;
  UserControlOnCan *can1;                     // 标记所用的CAN总线
  Joint *jointBeingUsed;                      // 标记所控制的关节模块
};

#endif // MOVE_H
