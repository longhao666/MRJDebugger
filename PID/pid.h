// Copyright 2017, Inc.
// All rights reserved.
// ----------------------------------------------------------------------------
// Author :
// ----------------------------------------------------------------------------

#ifndef PID_H
#define PID_H

#include <QWidget>
#include "pid_global.h"
#include "./usercontroloncan.h"

namespace Ui {
class PID;
}

class PIDSHARED_EXPORT PID : public QWidget {
  Q_OBJECT

public:
  explicit PID(UserControlOnCan *can1,
                  Joint *jointBeingUsed,
                  QWidget *parent = 0);
  virtual ~PID();

public slots:
  void PIDInitialize(uint16_t ID);
  void btnSave_clicked();
  void btnLoad_clicked();

signals:

private slots:
  void on_POS_PSpinBox_editingFinished();
  void on_POS_ISpinBox_editingFinished();
  void on_POS_DSpinBox_editingFinished();
  void on_POS_DSSpinBox_editingFinished();
  void on_SPD_PSpinBox_editingFinished();
  void on_SPD_ISpinBox_editingFinished();
  void on_SPD_DSpinBox_editingFinished();
  void on_SPD_DSSpinBox_editingFinished();
  void on_CUR_PSpinBox_editingFinished();
  void on_CUR_ISpinBox_editingFinished();
  void on_CUR_DSpinBox_editingFinished();
  void on_maxCurLineEdit_editingFinished();
  void on_maxSpdLineEdit_editingFinished();
  void on_maxAccLineEdit_editingFinished();
  void on_minPosLLineEdit_editingFinished();
  void on_maxPosLLineEdit_editingFinished();
  void on_minPosHLineEdit_editingFinished();
  void on_maxPosHLineEdit_editingFinished();
  void on_adjustGroupComboBox_currentIndexChanged(int index);

private:
  void showSEVPID();
  void showMPID();
  void showLPID();
  void setNewPID(int value, uint8_t index);
  Ui::PID *uiPID;
  UserControlOnCan *can1;                     // 标记所用的CAN总线
  Joint *jointBeingUsed;                      // 标记所控制的关节模块
};

#endif // PID_H
