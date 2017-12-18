#include "bottom.h"
#include "ui_bottom.h"
#include <iostream>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>
#include <QFileDialog>
#include <QPixmap> // 在label上显示图片
#include <numeric> // 给存ID的容器排序
#include <iterator>
#include <vector>

#define BOTTOM_UPDATE_INTEVAL 2000  // 底部更新周期（ms）
#define ENonP_RGB_R 131             // enable on power 按键的颜色定义
#define ENonP_RGB_G 177             //
#define ENonP_RGB_B 78              // Greenery 131, 177, 78

#define TGPOS_RGB_R 218 // Fandango 218, 73, 128
#define TGPOS_RGB_G 73
#define TGPOS_RGB_B 128
#define TGSPD_RGB_R 131 // Greenery 131, 177, 78
#define TGSPD_RGB_G 177
#define TGSPD_RGB_B 78
#define TGCUR_RGB_R 67  // Dazzling Blue 67, 73, 157
#define TGCUR_RGB_G 73
#define TGCUR_RGB_B 157
#define RLPOS_RGB_R 237 // Prism Pink 237, 158, 189
#define RLPOS_RGB_G 158
#define RLPOS_RGB_B 189
#define RLSPD_RGB_R 247 // Blazing Yellow 247, 235, 55
#define RLSPD_RGB_G 235
#define RLSPD_RGB_B 55
#define RLCUR_RGB_R 237 // Brilliant White 237, 239, 252
#define RLCUR_RGB_G 239
#define RLCUR_RGB_B 252

using std::vector;
using std::cout;
using std::endl;
using std::sort;


Bottom::Bottom(UserControlOnCan *can1, Joint *jointBeingUsed, QWidget *parent) :
  QWidget(parent),
  isCANInitialSucceed(false),
  uiBottom(new Ui::Bottom)
{
  uiBottom->setupUi(this);
  this->can1 = can1;
  this->jointBeingUsed = jointBeingUsed;
  qDebug()<< 123<<jointBeingUsed;

}

Bottom::~Bottom()
{
  delete uiBottom;
}

void Bottom::on_btnUpdateID_clicked()
{
  if ( !isCANInitialSucceed ) {  // 还没有初始化成功
    if( !can1->Init("pcan0") ) {
      QMessageBox::warning(this,"Warning","CAN initialization failed");   // 不能捕获CAN API输出的信息
      isCANInitialSucceed = false;
    } else {
      qDebug("PCAN initialization succeeds");
      isCANInitialSucceed = true;
    }
  } else {  // 已经初始化成功
    qDebug("pcan has initialized, execute updateAllID");
    if (!can1->controller.updateAllID()) {     // WARNING 上面是Init，这里却要updateAllID，API这样设计很难用
      // update失败，可能是连接断开了
      QMessageBox::warning(this,"Warning","CAN update failed");   // 不能捕获CAN API输出的信息
      isCANInitialSucceed = false;
    }
  }
  updatecmbID();                          // 从JointThread读已有的ID，添加到ui的cmbID控件上
}

void Bottom::waitingForWidgetReady()
{
  this->on_btnUpdateID_clicked();  // 模拟点击按键
}

void Bottom::dealIDChanged(int newID)
{
  on_btnUpdateID_clicked(); //TODO 更新CAN线上的设备，可能由于ID重复而更新失败，应该增加ID恢复的功能
  // 选中触发cmbID的index改变的槽函数
  for (int i = 1; i <= uiBottom->cmbID->count(); i++) {
      if (newID == uiBottom->cmbID->itemText(i - 1).toInt()) { // index从0开始，size从1开始
          uiBottom->cmbID->setCurrentIndex(i - 1);
          break;
      }
  }
}

void Bottom::updatecmbID()
{
    qDebug()<<12345<<jointBeingUsed;
  const unsigned int DEVICEMAXNUM = 100;
  // Fill the items of combo box
  uiBottom->cmbID->clear(); // 清空之后才能正确添加
  qDebug("combo box cmbID clear done");
  // 先把ID存出来，排序
  vector<uint32_t> vectID;
  unsigned long long sizeOfAllJoint = can1->controller.allJoint.size();
  qDebug("allJoint.size(): %ld", sizeOfAllJoint);
  if (sizeOfAllJoint > DEVICEMAXNUM) {
    QMessageBox::warning(this,"Warning","device number is wrong");
    return;  //BUG 不然接下去程序崩溃，CANAPI的问题
  }
  qDebug()<<123451111<<jointBeingUsed;
  for (vector<Joint>::iterator iter = can1->controller.allJoint.begin();
       iter != can1->controller.allJoint.end();
       ++iter) {
    vectID.push_back(iter._Ptr->ID);
  }
  qDebug()<<12345333<<jointBeingUsed;
  qDebug("vector vectID push_back done");
  sort(vectID.begin(), vectID.end());
  qDebug()<<123451233<<jointBeingUsed;
  qDebug("vector vectID sort done");
  // 再把排序好的ID添加到combox
  for (vector<uint32_t>::iterator iter = vectID.begin();
       iter != vectID.end();
       ++iter) {
    qDebug("addItem: %d", *(iter._Ptr));
    uiBottom->cmbID->addItem(QString::number(*iter, 10));
  }
  qDebug()<<123454444<<jointBeingUsed;
  qDebug("combo box cmbID addItem done");
  qDebug()<<12345222<<jointBeingUsed;
  // combox添加好后
  if (can1->controller.allJoint.size() == 0) {
    if (isCANInitialSucceed) { // 初始化都没成功，就不用再警告了
      qDebug("Module not detected");
      QMessageBox::warning(this,"WARNING","Module not detected");
    } else {
      qDebug("there is none module because CAN initial failed");
    }
//    if (jointBeingUsed != NULL) {
//      emit cmbIDChanged(jointBeingUsed->ID);
//    }
    //        OscilloScope(); ///<为了界面好看一点
    //        health();       ///<为了界面好看一点
    //        moveInitialize();
  } else {
    // 触发cmbID的index改变的槽函数。获取末尾ID，方便调试，index从0开始，size从1开始
    uiBottom->cmbID->setCurrentIndex(static_cast<int>(can1->controller.allJoint.size()) - 1);
  }
  qDebug()<<12345777<<jointBeingUsed;
}

void Bottom::on_enableDriverPushButton_clicked()
{
  if (jointBeingUsed == NULL) {
    return;
  }
  // 先更新当前的状态
  can1->controller.SendMsg(jointBeingUsed->ID, //WARNING 需要修改API中的类型定义
                           CMDTYPE_RD,
                           SYS_ENABLE_DRIVER,
                           NULL,
                           0x02);
  can1->controller.delayMs(5);
  uint16_t data_L = 0;
  can1->controller.GetValueInTable(jointBeingUsed->ID,
                                   SYS_ENABLE_DRIVER,
                                   data_L);
  // 准备反转当前的状态
  bool isEnable = false;
  if (data_L != 0) {
    isEnable = false;
  } else {
    isEnable = true;
  }
  // 向下位机请求数据
  jointBeingUsed->setEnable(isEnable);
  qDebug() << "on_enableDriverPushButton_clicked(): "
           << "isEnable = "
           << isEnable;
  // 重新更新状态，确认反转状态
  updateEnableDriver();
}

void Bottom::updateEnableDriver()
{
  enableDriverPushButtonOn = "background-color: rgb(";
  enableDriverPushButtonOn += QString::number(ENonP_RGB_R) + ','
                              + QString::number(ENonP_RGB_G) + ','
                              + QString::number(ENonP_RGB_B) + ");";
  enableDriverPushButtonOff = tgPOSPushButtonOn;
  can1->controller.SendMsg(jointBeingUsed->ID,
                           CMDTYPE_RD,
                           SYS_ENABLE_DRIVER,
                           NULL,
                           0x02);
  can1->controller.delayMs(2);
  uint16_t data_L = 0;
  can1->controller.GetValueInTable(jointBeingUsed->ID,
                                   SYS_ENABLE_DRIVER,
                                   data_L);
  if (data_L != 0) {
    // 按的是enableDriverPushButton但改变颜色的是enableDriverPushButton_2
    uiBottom->enableDriverPushButton_2->setStyleSheet(enableDriverPushButtonOn);
    uiBottom->enableDriverPushButton->setText("Enabled"); // 文字相应改变
  } else {
    uiBottom->enableDriverPushButton_2->setStyleSheet(enableDriverPushButtonOff);
    uiBottom->enableDriverPushButton->setText("Disabled"); // 文字相应改变
  }
}

void Bottom::updateIfError() {
  ifErrorPushButton_NoError = "background-color: rgb(";
  ifErrorPushButton_Error = "background-color: rgb(";
  ifErrorPushButton_NoError += QString::number(ENonP_RGB_R) + ','
                               + QString::number(ENonP_RGB_G) + ','
                               + QString::number(ENonP_RGB_B) + ");";
  ifErrorPushButton_Error = QString::number(TGPOS_RGB_R) + ','
                            + QString::number(TGPOS_RGB_G) + ','
                            + QString::number(TGPOS_RGB_B) + ");";
  // 读错误码
  uint16_t data_L = 0;
  can1->controller.SendMsg(jointBeingUsed->ID,
                           CMDTYPE_RD,
                           SYS_ERROR,
                           NULL,
                           0x02);
  can1->controller.delayMs(2);
  can1->controller.GetValueInTable(jointBeingUsed->ID,
                                   SYS_ERROR,
                                   data_L);
  if (data_L != 0) {
    uiBottom->ifErrorPushButton->setStyleSheet(ifErrorPushButton_Error);
  } else {
    uiBottom->ifErrorPushButton->setStyleSheet(ifErrorPushButton_NoError);
  }
}

void Bottom::updateConnected() {
  QString connected = "background-color: rgb(";
  connected += QString::number(ENonP_RGB_R) + ','
               + QString::number(ENonP_RGB_G) + ','
               + QString::number(ENonP_RGB_B) + ");";
  uiBottom->connectedPushButton->setStyleSheet(connected);
}

void Bottom::on_btnFlash_clicked()
{
  if (jointBeingUsed == NULL) {
    return;
  }
  bool isSuccess = jointBeingUsed->setSaveToFlash();
  if (isSuccess) {
    QMessageBox::information(this,tr("INFO"),tr("Succeed"));
  } else {
    QMessageBox::warning(this,"WARNING","Failed");
  }
}

void Bottom::on_cmbID_currentIndexChanged(const QString &arg1)
{
  jointBeingUsed = can1->findJointID(arg1.toUInt()); // 修改当前控制的模块ID
  if (jointBeingUsed == NULL) {
    return;
  }
  qDebug() << "on_cmbID_currentIndexChanged(): "
           << "ID = "
           << jointBeingUsed->ID;

  emit cmbIDChanged(jointBeingUsed->ID);
  //    moveInitialize();
  //    // 初始化示波器
  //    OscilloScope();
  //    // 初始化PID面板
  //    PID();
  //    // 初始化Set面板
  //    Set();
  //    // 初始化health面板
  //    health();
  // bottom的1个部分
  if (timerBottom == NULL) { // 若是首次运行
    timerBottom = new QTimer(this);
    connect(timerBottom,SIGNAL(timeout()),this,SLOT(slotTimeBottomDone()));
    timerBottom->start(BOTTOM_UPDATE_INTEVAL);
  }
  slotTimeBottomDone();
  // bottom的另1个部分，4条状态的显示
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_MODEL_TYPE,NULL,0x02);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_FW_VERSION,NULL,0x02);
  can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,SYS_REDU_RATIO,NULL,0x02);
  can1->controller.delayMs(5);
  uint16_t data_L = 0;
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_MODEL_TYPE,data_L);
  modelTypeBeingUsed = data_L;
  QString tmp;
  switch (modelTypeBeingUsed)
  {
    case 16: tmp = "M14"; break;
    case 17: tmp = "M14E"; break;
    case 2:
      //        case 32: tmp = "M17"; modelTypeBeingUsed = 2; break; // CANAPI .h里只定义了2的情况，没有定义32的情况
    case 32: tmp = "M17"; modelTypeBeingUsed = 32; break; // CANAPI .h里只定义了32的情况，没有定义2的情况
    case 33: tmp = "M17E"; break;
    case 48: tmp = "M20"; break;
    case 64: tmp = "LIFT"; break;
  }
  uiBottom->typeLabel->setText("TYPE: " + tmp);
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_FW_VERSION,data_L);
  uiBottom->firmLabel->setText("FIRM: " + QString::number(data_L,10));
  can1->controller.GetValueInTable(jointBeingUsed->ID, SYS_REDU_RATIO,data_L);
  uiBottom->ratioLabel->setText("RATIO:" + QString::number(data_L,10));
  //TODO bottom中的connect按钮
}

void Bottom::updateWorkModePushButton() {
  QString background = "background-color: rgb(";
  background += QString::number(ENonP_RGB_R) + ',' + QString::number(ENonP_RGB_G) + ',' + QString::number(ENonP_RGB_B) + ");";
  uiBottom->workModePushButton->setStyleSheet(background);


  int workMode = jointBeingUsed->getWorkMode();
  switch(workMode) {// 工作模式修改后，修改txtBias
    case MODE_OPEN:
      uiBottom->workModePushButton->setText("OPEN");
      break;
    case MODE_CURRENT:
      uiBottom->workModePushButton->setText("CURRENT");
      break;
    case MODE_SPEED:
      uiBottom->workModePushButton->setText("SPEED");
      break;
    case MODE_POSITION:
      uiBottom->workModePushButton->setText("POSITION");
      break;
    default: break;
  }
}

void Bottom::slotTimeBottomDone()  // TODO 改用图片，而不是用button
{
  if (jointBeingUsed == NULL) {
    return;
  }

  updateEnableDriver();
  updateIfError(); // 检查是否发生了错误
  updateConnected(); // 把按钮变成绿色
  updateWorkModePushButton(); // 把按钮变成绿色，并且显示工作模式
}

void Bottom::on_btnSave_clicked()
{
  //这个功能在PID里面实现，这里只需要emit个信号
  emit this->btnSave_clicked();
}

void Bottom::on_btnLoad_clicked()
{
  //这个功能在PID里面实现，这里只需要emit个信号
  emit this->btnLoad_clicked();
}
