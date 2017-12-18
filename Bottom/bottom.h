// Copyright 2017, Inc.
// All rights reserved.
// ----------------------------------------------------------------------------
// Author :
// ----------------------------------------------------------------------------
#ifndef BOTTOM_BOTTOM_H_
#define BOTTOM_BOTTOM_H_

#include <QWidget>
#include <QTimer>
#include "./bottom_global.h"
#include "./usercontroloncan.h"

namespace Ui {
class Bottom;
}

class BOTTOMSHARED_EXPORT Bottom : public QWidget {
  Q_OBJECT

public:
  explicit Bottom(UserControlOnCan *can1,
                  Joint *jointBeingUsed,
                  QWidget *parent = 0);
  virtual ~Bottom();
  bool isCANInitialSucceed;
  QTimer *timerBottom;
  uint16_t modelTypeBeingUsed;

public slots:
  void updateEnableDriver();
  void updateIfError();
  void updateConnected();
  void updateWorkModePushButton();
  void updatecmbID();
  void on_btnUpdateID_clicked();
  void waitingForWidgetReady();
  void dealIDChanged(int newID);

signals:
  void cmbIDChanged(uint16_t ID);   // 给别的组件发信号
  void btnSave_clicked();
  void btnLoad_clicked();

private slots:
  void on_enableDriverPushButton_clicked();
  void slotTimeBottomDone();
  void on_btnSave_clicked();
  void on_btnLoad_clicked();
  void on_btnFlash_clicked();
  void on_cmbID_currentIndexChanged(const QString &arg1);

private:
  Ui::Bottom *uiBottom;
  UserControlOnCan *can1;                     // 标记所用的CAN总线
  Joint *jointBeingUsed;                      // 标记所控制的关节模块
  QString tgPOSPushButtonOn;
  QString tgPOSPushButtonOff;
  QString tgSPDPushButtonOn;
  QString tgSPDPushButtonOff;
  QString tgCURPushButtonOn;
  QString tgCURPushButtonOff;
  QString rlPOSPushButtonOn;
  QString rlPOSPushButtonOff;
  QString rlSPDPushButtonOn;
  QString rlSPDPushButtonOff;
  QString rlCURPushButtonOn;
  QString rlCURPushButtonOff;
  QString scopeEnablePushButtonOn;
  QString scopeEnablePushButtonOff;
  QString enableDriverPushButtonOn;
  QString enableDriverPushButtonOff;
  QString ifErrorPushButton_NoError;
  QString ifErrorPushButton_Error;
};

#endif  // BOTTOM_BOTTOM_H_
