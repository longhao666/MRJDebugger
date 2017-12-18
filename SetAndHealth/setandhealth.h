#ifndef SETANDHEALTH_H
#define SETANDHEALTH_H

#include <QWidget>
#include <QTimer>
#include "setandhealth_global.h"
#include "./usercontroloncan.h"

namespace Ui {
class SetAndHealth;
}

class SETANDHEALTHSHARED_EXPORT SetAndHealth : public QWidget {
  Q_OBJECT

public:
  explicit SetAndHealth(UserControlOnCan *can1,
                  Joint *jointBeingUsed,
                  QWidget *parent = 0);
  virtual ~SetAndHealth();

public slots:
  void SetAndHealthInitialize(uint16_t ID);
signals:
  void IDChanged(int newID);
  void ZeroPositionSeted();

private slots:
  void on_IDPushButton_clicked();
  void on_setZeroPushButton_clicked();
  void on_ENonPPushButton_clicked();
  void on_clearErrorButton_clicked();
  void on_updateButton_clicked();
  void slotTimeMonitorDone();

private:
  void set();
  void health();
  Ui::SetAndHealth *uiSetAndHealth;
  UserControlOnCan *can1;                     // 标记所用的CAN总线
  Joint *jointBeingUsed;                      // 标记所控制的关节模块
  QString ENonPPushButtonOn;
  QString ENonPPushButtonOff;
  QTimer* timerMonitor = NULL;
};

#endif // SETANDHEALTH_H
