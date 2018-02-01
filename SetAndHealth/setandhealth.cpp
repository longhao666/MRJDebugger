#include <QMessageBox>
#include "setandhealth.h"
#include "ui_setandhealth.h"

#define ENonP_RGB_R 131             // enable on power 按键的颜色定义
#define ENonP_RGB_G 177             //
#define ENonP_RGB_B 78              // Greenery 131, 177, 78
#define TGPOS_RGB_R 218 // Fandango 218, 73, 128
#define TGPOS_RGB_G 73
#define TGPOS_RGB_B 128
#define MONITOR_INTEVAL 500         // 监视器更新周期（ms）

SetAndHealth::SetAndHealth(UserControlOnCan* can1, Joint* jointBeingUsed, QWidget* parent) :
  QWidget(parent),
  uiSetAndHealth(new Ui::SetAndHealth)
{
  uiSetAndHealth->setupUi(this);
  this->can1 = can1;
  this->jointBeingUsed = jointBeingUsed;
}

SetAndHealth::~SetAndHealth()
{
  delete uiSetAndHealth;
}

void SetAndHealth::SetAndHealthInitialize(uint16_t ID)
{
  jointBeingUsed = can1->findJointID(ID); // 修改当前控制的模块ID
  this->set();
  this->health();
}

void SetAndHealth::set()
{
  if (jointBeingUsed == NULL) {
    return;
  }
  // initialize ENABLE_ON_POWER - related content
  ENonPPushButtonOn = "color: rgb(";
  ENonPPushButtonOn += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
  ENonPPushButtonOff = "color: rgb("; //background-color背景色
  ENonPPushButtonOff += QString::number(TGPOS_RGB_R) + ',' + QString::number(TGPOS_RGB_G) + ',' + QString::number(TGPOS_RGB_B) + ");";
  // 初始化API中的MCT里的ENABLE_ON_POWER
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_ENABLE_ON_POWER,NULL,0x02);
  can1->controller.delayMs(50); // 1 ms并不能成功更新MCT
  uint16_t data_L = 0;
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ENABLE_ON_POWER,data_L);
  //    cout << "Set() - data_L: " << data_L << endl;
  if (data_L != 0) {
    uiSetAndHealth->ENonPPushButton->setStyleSheet(ENonPPushButtonOn);
  } else {
    uiSetAndHealth->ENonPPushButton->setStyleSheet(ENonPPushButtonOff);
  }
  // 把设ID的输入框填上当前的ID 北科小伙点set ID的事故
  uiSetAndHealth->IDLineEdit->setText(QString::number(jointBeingUsed->ID));
}

void SetAndHealth::on_IDPushButton_clicked()
{
  int newID = uiSetAndHealth->IDLineEdit->text().toInt();
  if (newID > SEARCH_MAX_ID) {
    QMessageBox::warning(this,"Waring","ID too large");
  } else {
    // 设新的ID
    jointBeingUsed->setID(newID);
    // 总线上的通信还是以旧ID进行的
    jointBeingUsed->setSaveToFlash();
    // 将软件当前控制的模块ID，更新为新ID，不然软件崩溃
    emit IDChanged(newID);
    QMessageBox::information(this, "提示", "请将模块重新上电并在软件中更新ID");
  }
}

void SetAndHealth::on_setZeroPushButton_clicked()
{
  //如果当前是位置控制模式，则向内存控制表SYS_SET_ZERO_POS写入1
  int workMode = jointBeingUsed->getWorkMode();
  if (3 == workMode) {
    // 停止运动控制
      // 一停下来马上就点击设置0位按钮会有很大的抖动,停下一会后再点击相对来说会好一点
    emit ZeroPositionSeted();
    // 设置0位
    jointBeingUsed->setZeroPos();
    //给出下一步提示，可以直接点击烧写
    QMessageBox::information(this,"NOTE","succeed，please load flash");
    // 停止运动控制，以更新bias显示
    emit ZeroPositionSeted();
  }
  else {
    QMessageBox::warning(this,"WARNING","NO POS mode");
  }
}

void SetAndHealth::on_ENonPPushButton_clicked()
{
  uint16_t data_L = 0;
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ENABLE_ON_POWER, data_L);
  int value = 0;
  if (data_L != 0) {
    value = 0;
  } else {
    value = 1;
  }
  // 向下位机请求数据
  uint8_t data[2] = {0,0};
  data[1] = (uint8_t)( (value & 0xff00) >> 8 );
  data[0] = (uint8_t)( value & 0xff );
  can1->controller.SendMsg(jointBeingUsed->ID, CMDTYPE_WR_NR, SYS_ENABLE_ON_POWER, data, 2);
  can1->controller.delayMs(50);
  qDebug("SetAndHealth::on_ENonPPushButton_clicked(): value = %d", value);
  set();
}

void SetAndHealth::health()
{
  slotTimeMonitorDone();// 运行一次更新数据
  if (timerMonitor == NULL) {
    timerMonitor = new QTimer(this);
    connect(timerMonitor,SIGNAL(timeout()),this,SLOT(slotTimeMonitorDone()));
//    timerMonitor->start(MONITOR_INTEVAL);
//    uiSetAndHealth->updateButton->setVisible(false);
  } // 暂时持续性地更新数据 // 暂时不需要持续性地更新数据
}

void SetAndHealth::on_clearErrorButton_clicked()
{
  if (jointBeingUsed == NULL) {
    return;
  }
  jointBeingUsed->clearErrStatus();
  qDebug("SetAndHealth::on_clearErrorButton_clicked(): done.");
}

void SetAndHealth::on_updateButton_clicked()
{
  slotTimeMonitorDone();
}

void SetAndHealth::slotTimeMonitorDone()
{
  if (jointBeingUsed == NULL) {
    return;
  }

  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_VOLTAGE,NULL,0x02);
  can1->controller.delayMs(5);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_CURRENT_L,NULL,0x04);
  can1->controller.delayMs(5);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_TEMP,NULL,0x02);
  can1->controller.delayMs(5);
  //    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_SPEED_L,NULL,0x04);
  jointBeingUsed->updateCurSpeed();
  can1->controller.delayMs(5);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_POSITION_L,NULL,0x04);
  can1->controller.delayMs(5);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,MOT_ST_DAT,NULL,0x04);
  can1->controller.delayMs(5);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_ERROR,NULL,0x02);
  can1->controller.delayMs(5);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,BAT_VOLT,NULL,0x02);
  can1->controller.delayMs(5);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,ACC_X,NULL,0x06);
  can1->controller.delayMs(5);

  uint16_t data_L = 0;
  uint16_t data_H = 0;

  // 更新电压，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_VOLTAGE, data_L);
  uiSetAndHealth->VolLineEdit->setText(QString::number((double)data_L / 100, 'f' ,2) + "V");

  // 更新电流，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_CURRENT_L, data_L);
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_CURRENT_H, data_H);
  double temp = data_L + (data_H * 65536);
  uiSetAndHealth->CurLineEdit->setText(QString::number(temp / 1000, 'f', 2) + "A");

  // 更新温度，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_TEMP,data_L);
  uiSetAndHealth->TempLineEdit->setText(QString::number(static_cast<double>(data_L) / 10, 'f', 1) + "°C");

  // 更新速度，并显示
  //    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_SPEED_L, data_L);
  //    can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_SPEED_H, data_H);
  //    temp = data_L + (data_H * 65536);
  //    temp *= 60; // rpm
  //    temp /= 65536; // rpm
  //    temp /= GEAR_RATIO_M14;
  temp = jointBeingUsed->getCurSpeed();
  uiSetAndHealth->SpeedLineEdit->setText(QString::number(temp, 'f', 2) + "rpm");

  // 更新位置，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_POSITION_L, data_L);
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_POSITION_H, data_H);
  uiSetAndHealth->POS_LLineEdit->setText(QString::number((double)data_L * 360.0 / 65536.0, 'f', 2) + "°");
  uiSetAndHealth->POS_HLineEdit->setText(QString::number((short)data_H));

  // 更新编码器，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, MOT_ST_DAT, data_L);
  can1->controller.GetValueInTable(jointBeingUsed->ID, MOT_MT_DAT, data_H);
  uiSetAndHealth->STurnLineEdit->setText(QString::number((double)data_L * 360.0 / 65536.0, 'f', 2) + "°");
  uiSetAndHealth->MTurnLineEdit->setText(QString::number((short)data_H));

  // 更新错误码，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_ERROR, data_L);
  // 按错误码显示文字
  //    uiSetAndHealth->errorLineEdit->setVisible(false); // 临时性
  //    data_L = ERROR_MASK_OVER_CURRENT; // 测试用
  uiSetAndHealth->errorTextEdit->clear();
  if (data_L & ERROR_MASK_OVER_CURRENT) {
    uiSetAndHealth->errorTextEdit->append("过流，错误码0x0001\n");
  }
  if (data_L & ERROR_MASK_OVER_VOLTAGE) {
    uiSetAndHealth->errorTextEdit->append("过压，错误码0x0002\n");
  }
  if (data_L & ERROR_MASK_UNDER_VOLTAGE) {
    uiSetAndHealth->errorTextEdit->append("欠压，错误码0x0004\n");
  }
  if (data_L & ERROR_MASK_OVER_TEMP) {
    uiSetAndHealth->errorTextEdit->append("过温，错误码0x0008\n");
  }
  if (data_L & ERROR_MASK_BATTERY) {
    uiSetAndHealth->errorTextEdit->append("编码器电池错误，错误码0x0010\n");
  }
  if (data_L & ERROR_MASK_ENCODER) {
    uiSetAndHealth->errorTextEdit->append("码盘错误，错误码0x0020\n");
  }
  if (data_L & ERROR_MASK_POTEN) {
    uiSetAndHealth->errorTextEdit->append("电位器错误，错误码0x0040\n");
  }
  if (data_L & ERROR_MASK_CURRENT_INIT) {
    uiSetAndHealth->errorTextEdit->append("电流检测错误，错误码0x0080\n");
  }
  if (data_L & ERROR_MASK_FUSE) {
    uiSetAndHealth->errorTextEdit->append("保险丝断开错误，错误码0x0100\n");
  }
  if (data_L == 0) {
    uiSetAndHealth->errorTextEdit->append("No Error\n");
  }

  // 更新电池电压，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, BAT_VOLT, data_L);
  if (data_L == 0) {
    uiSetAndHealth->BATVoltLineEdit->setText("-");
  } else {
    uiSetAndHealth->BATVoltLineEdit->setText(QString::number((double)data_L / 100, 'f' ,2) + "V");
  }

  // 更新三轴加速度，并显示
  can1->controller.GetValueInTable(jointBeingUsed->ID, ACC_X, data_L);
  uiSetAndHealth->AccXLineEdit->setText(QString::number((short)data_L) + "mg");
  can1->controller.GetValueInTable(jointBeingUsed->ID, ACC_Y, data_L);
  uiSetAndHealth->AccYLineEdit->setText(QString::number((short)data_L) + "mg");
  can1->controller.GetValueInTable(jointBeingUsed->ID, ACC_Z, data_L);
  uiSetAndHealth->AccZLineEdit->setText(QString::number((short)data_L) + "mg");

}
