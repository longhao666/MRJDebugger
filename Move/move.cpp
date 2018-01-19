#include "move.h"
#include "ui_move.h"
#include <string>
#include <QDebug>

#define PI 3.1415926
#define MODE_MANUAL     0           // 手动控制
#define MODE_SINE       1           // 自动控制之正弦波
#define MODE_SQUARE     2           // 自动控制之方波
#define MODE_TRIANGLE   3           // 自动控制之三角波
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
#define MOTION_CONTROL_INTEVAL 30   // 运动控制周期（ms）

using std::string;

Move::Move(UserControlOnCan* can1, Joint* jointBeingUsed, QWidget* parent) :
  QWidget(parent),
  //  MC(NULL),
  uiMove(new Ui::Move)
{
  uiMove->setupUi(this);
  this->can1 = can1;
  //this->jointBeingUsed = jointBeingUsed;
  //qDebug()<<123<<"move"<<jointBeingUsed;
}

Move::~Move()
{
  delete uiMove;
}

void Move::moveInitialize(uint16_t ID)
{

   // qDebug()<<1234444<<"move"<<jointBeingUsed;
  // 设置启动、停止按钮的样式表，background-color背景色
  stopButtonOn = "color: rgb(";
  stopButtonOn += QString::number(TGPOS_RGB_R) + ','
                  + QString::number(TGPOS_RGB_G) + ','
                  + QString::number(TGPOS_RGB_B) + ");";
  stopButtonOff = "color: rgb(128, 128, 128);";
  confirmButtonOn = "color: rgb(";
  confirmButtonOn += QString::number(TGSPD_RGB_R) + ','
                     + QString::number(TGSPD_RGB_G) + ','
                     + QString::number(TGSPD_RGB_B) + ");";
  confirmButtonOff = stopButtonOff;
  uiMove->stopButton->setStyleSheet(stopButtonOn);
  uiMove->confirmButton->setStyleSheet(confirmButtonOff);
  // 定义运动控制为关
  EnableRun = false;
  uiMove->confirmButton->setText("Click to run"); // 此外还有一处设置了setText
  // 首次启动则定义运动控制定时器
  if (timerMove == NULL) {
    timerMove = new QTimer(this);
    connect(timerMove,SIGNAL(timeout()),this,SLOT(slotTimeMoveDone()));
    //        timerMove->start(MOTION_CONTROL_INTEVAL); // 先不启动
  }
  // 使用10ms手动位置运动控制器
  jointBeingUsed = can1->findJointID(ID); // 修改当前控制的模块ID
  if (jointBeingUsed != NULL) {
    jointBeingUsed->updateCurPos();
    can1->controller.delayMs(50);
  }
//  if (this->MC == NULL) {
//    this->MC = new MotionControl(jointBeingUsed, this);
//  } else {
//    delete this->MC;
//    MC = new MotionControl(jointBeingUsed, this);
//  }
  if (jointBeingUsed == NULL) {
    return;
  }
  // 由当前工作模式更新控制板块中的ComboBox
  int workMode = jointBeingUsed->getWorkMode();
  uiMove->cmbWorkMode->setCurrentIndex(workMode); // 会触发，也可能不会触发on_cmbWorkMode_currentIndexChanged()
  // 防止没有调用on_cmbWorkMode_currentIndexChanged()，强制运行下列2个函数
  // 工作模式更新bias
  workModeUpdatetxtBias();
  // bias更新滑块
  txtBiasChangemanualSlider();

}

void Move::ClickStopButton()
{
    //这样可以控死
    if(!EnableRun) {
    EnableRun = true;
        on_stopButton_clicked();
    }else {
        on_stopButton_clicked();
    }
    // 他这样不行,要动,去设置的点
   // on_stopButton_clicked();

}

void Move::on_txtBias_editingFinished()
{
  int workMode = uiMove->cmbWorkMode->currentIndex();
  if (MODE_POSITION != workMode) { // 若是位置控制，则不限制bias
    double max = uiMove->manualMax->text().toDouble();
    double min = uiMove->manualMin->text().toDouble();
    if (uiMove->txtBias->text().toDouble() > max) {
      uiMove->txtBias->setValue(max);
    } else if (uiMove->txtBias->text().toDouble() < min) {
      uiMove->txtBias->setValue(min);
    }
  }
  bias = uiMove->txtBias->text().toDouble();
//  qDebug() << bias << 123;
  txtBiasChangemanualSlider();
//  if (uiMove->cmbWorkMode->currentIndex() == MODE_POSITION
//      && uiMove->waveModeCombo->currentIndex() == MODE_MANUAL) {
//    // 改成用10ms控制器控制的方式
//    if (EnableRun == true) {
//      double angle = bias / 180.0 * PI;
//      qDebug("angle: %lf",angle);
//      //MC->SetTag(angle);
//    }
//  }
}

void Move::on_cmbWorkMode_currentIndexChanged(int index)
{
  if (jointBeingUsed == NULL) {
    return;
  }

  //    qDebug() << "in on_cmbWorkMode_currentIndexChanged";
  uint8_t workMode = static_cast<uint8_t>(index);

  jointBeingUsed->setWorkMode(workMode);
  can1->controller.delayMs(50);
  jointBeingUsed->updateWorkMode();
  can1->controller.delayMs(50);

  // 工作模式更新bias
  workModeUpdatetxtBias();
  // bias更新滑块
  txtBiasChangemanualSlider();
}

void Move::workModeUpdatetxtBias()
{
  if (jointBeingUsed == NULL) {
    return;
  }
  int workMode = uiMove->cmbWorkMode->currentIndex();

  switch(workMode) // 工作模式修改后，修改txtBias
  {
    case MODE_OPEN:
      jointBeingUsed->updateTagPWM();
      can1->controller.delayMs(50);
      uiMove->txtBias->setValue(jointBeingUsed->getTagPWM()); // 由当前目标PWM更新手动控制中的偏移量
      uiMove->manualMax->setText("30");
      uiMove->manualMin->setText("-30");
      break;
    case MODE_CURRENT:
      jointBeingUsed->updateCurI();
      can1->controller.delayMs(50);
      //            uiMove->txtBias->setValue(can1->getCurI(jointBeingUsed)); // 由当前实际电流更新手动控制中的偏移量，实际中处于0位的机械臂2关节突然掉下
      uiMove->txtBias->setValue(0); // 和上面注释的那句效果一样，不如就改成0
      uiMove->manualMax->setText("500");
      uiMove->manualMin->setText("-500");
      break;
    case MODE_SPEED:
      jointBeingUsed->updateCurSpeed();
      can1->controller.delayMs(50);
      uiMove->txtBias->setValue(static_cast<double>(jointBeingUsed->getCurSpeed())); // 由当前实际电流更新手动控制中的偏移量
      uiMove->manualMax->setText("20");
      uiMove->manualMin->setText("-20");
      break;
    case MODE_POSITION: {
        //            qDebug() << "ID: " << jointBeingUsed->ID;
        jointBeingUsed->updateCurPos();
        can1->controller.delayMs(50);
        float tempf = jointBeingUsed->getCurPos(JOINT_ANGLE);
        //            qDebug() << tempf;
        double tempd = static_cast<double>(tempf);
        qDebug("workModeUpdatetxtBias() MODE_POSITION %f", tempd);
        //            string s;
        //            can1->controller.GetErrorText(s);
        //            qDebug() << s.c_str();
        uiMove->txtBias->setValue(tempd); // 由当前实际位置更新手动控制中的偏移量
        uiMove->manualMax->setText("90");
        uiMove->manualMin->setText("-90");
        break;
      }
    default: break;
  }
  on_txtBias_editingFinished();
}

void Move::on_manualMax_editingFinished()
{
  float max = uiMove->manualMax->text().toFloat();
  QString s = QString("%1").arg(max * -1);
  uiMove->manualMin->setText(s);
  txtBiasChangemanualSlider();
}

void Move::on_manualMin_editingFinished()
{
  txtBiasChangemanualSlider();
}

void Move::txtBiasChangemanualSlider()
{
  int workMode = uiMove->cmbWorkMode->currentIndex();
  float bias = uiMove->txtBias->text().toFloat();
  float min = uiMove->manualMin->text().toFloat();
  float max = uiMove->manualMax->text().toFloat();
  if (MODE_POSITION == workMode) { // 如果是位置控制模式，滑块极值相应改变
    if (uiMove->manualSlider->hasFocus() || uiMove->manualMax->hasFocus() || uiMove->manualMin->hasFocus()) {
      ;
    } else {
      max = bias + 90;
      min = bias - 90;
      uiMove->manualMax->setText(QString::number(max, 'f' , 2));
      uiMove->manualMin->setText(QString::number(min, 'f' , 2));
    }
  }
  float f = 100 * (bias - min) / (max - min);
  int v = (int)f;
  uiMove->manualSlider->setValue(v);
}

void Move::on_manualSlider_valueChanged(int value)
{
  float min = uiMove->manualMin->text().toFloat();
  float max = uiMove->manualMax->text().toFloat();
  if (uiMove->manualSlider->hasFocus()) {
    float bias = min + (max - min) * value / 100;
    uiMove->txtBias->setValue(bias);
    on_txtBias_editingFinished();
  }
}

void Move::on_confirmButton_clicked()
{
  if (jointBeingUsed == NULL) {
    //        float angle = (float)(100 / 180.0 * PI);
    //    //        qDebug() << "angle: " << angle;
    //        MC->SetTag(angle);
    return;
  }
//  EnableRun = true;
//  uiMove->confirmButton->setText("Running");
//  uiMove->confirmButton->setStyleSheet(confirmButtonOn);
//  uiMove->stopButton->setStyleSheet(stopButtonOff);

//  if (uiMove->cmbWorkMode->currentIndex() == MODE_POSITION
//      && uiMove->waveModeCombo->currentIndex() == MODE_MANUAL) {
//    // 改成用10ms控制器控制的方式
//    float angle = (float)(bias / 180.0 * PI);
//    MC->SetTag(angle);
//  } else {
//    slotTimeMoveDone();
//    timerMove->start(MOTION_CONTROL_INTEVAL);
//  }
//    slotTimeMoveDone();
//    timerMove->start(MOTION_CONTROL_INTEVAL);
    if(!EnableRun) {
        EnableRun = true;
        uiMove->confirmButton->setText("Running");
        uiMove->confirmButton->setStyleSheet(confirmButtonOn);
        uiMove->stopButton->setStyleSheet(stopButtonOff);
        //slotTimeMoveDone();
        timerMove->start(MOTION_CONTROL_INTEVAL);
    }else {
        //qDebug() << 666;
        return ;
    }
}

void Move::on_stopButton_clicked()
{
  if (jointBeingUsed == NULL) {
    return;
  }
//  EnableRun = false;
//  uiMove->stopButton->setStyleSheet(stopButtonOn);
//  uiMove->confirmButton->setText("Click to run");
//  uiMove->confirmButton->setStyleSheet(confirmButtonOff);
//  timerMove->stop(); // 关闭定时器
  if(EnableRun) {
      EnableRun = false;
      uiMove->stopButton->setStyleSheet(stopButtonOn);
      uiMove->confirmButton->setText("Click to run");
      uiMove->confirmButton->setStyleSheet(confirmButtonOff);
      timerMove->stop();
  }else {
      return ;
  }
  const int workMode = uiMove->cmbWorkMode->currentIndex();
  switch(workMode) // Different WorkMode Different Stop way
  {
    case MODE_OPEN: {
        uint8_t pwm = 0;
        jointBeingUsed->setTagPWM(pwm);
        can1->controller.delayMs(50);
        break;
      }
    case MODE_CURRENT: {
        int curI = 0;
        jointBeingUsed->setTagI(curI);
        can1->controller.delayMs(50);
        break;
      }
    case MODE_SPEED: {
        float speed = 0;
        jointBeingUsed->setTagSpeed(speed);
        can1->controller.delayMs(50);
        break;
      }
    case MODE_POSITION: {
        // Can not be directly set to 0.
        // If read the current position first, then set the target position to the current position, that may be too slow.
        // So use another method: switch the WorkMode to speed then set the speed to 0, then switch back. The hardware could ensure to update the target location.
        // 但实际使用时，各关节都处于0位，2关节和4关节受到重力影响，这样的停止方式会发生运动，而且不停地按stop会持续性地运动
        jointBeingUsed->updateCurPos();
        can1->controller.delayMs(50);
//        if (uiMove->waveModeCombo->currentIndex() == MODE_MANUAL) {
//          //            // 改成用10ms控制器控制的方式
//          //            float currentPos = jointBeingUsed->getCurPos(JOINT_RADIAN);
//          //            MC->SetTag(currentPos);
//        } else {
//          float currentPos = jointBeingUsed->getCurPos(JOINT_ANGLE);
//          //        jointBeingUsed->setTagPos(currentPos, JOINT_ANGLE);
//          this->SetValue(currentPos);
//        }
        float currentPos = jointBeingUsed->getCurPos(JOINT_ANGLE);
        //        jointBeingUsed->setTagPos(currentPos, JOINT_ANGLE);
        this->SetValue(currentPos);
        break;
      }
    default: break;
  }
  // In fact, Stop means setting the Bias, and the text of Bias could be updated by simulate WorkMode change
  workModeUpdatetxtBias();
  // 实际使用中，在速度模式下，Stop后不会马上变成0，导致显示的速度不为0，而实际已经停止
  if (workMode == MODE_SPEED) {
    uiMove->txtBias->setValue(0.0f); // 补充修改Bias的值为0
    on_txtBias_editingFinished(); // 修改Bias调用相应槽函数
  }
  txtBiasChangemanualSlider();
}

/*
 * 运动控制的定时器
 */
void Move::slotTimeMoveDone()
{
  if (EnableRun) {
    static unsigned int s_iCount = 0;
    static unsigned int s_iCountforwave = 0;
    s_iCount++;
    //根据所选波形进入相应控制步骤
    switch (uiMove->waveModeCombo->currentIndex())
    {
      case MODE_MANUAL: {
          //                qDebug() << "in slotTimeMoveDone()";
          s_iCount = 0;
          SetValue(bias);
          break;
        }
      case MODE_SQUARE: { // 方波时根据选定频率发送，经过半个周期变换一次方向，所以是乘500
          double time = s_iCount * MOTION_CONTROL_INTEVAL;
          if (time >= 500.0 / frequency) {
            static bool s_bHigh = false;
            s_iCount = 0;
            s_bHigh = !s_bHigh;
            SetValue((int)(amplitude * (s_bHigh ? 1 : -1) + bias));
          }
          break;
        }
      case MODE_TRIANGLE: {//三角波时 TriangleInterval 个 Interval 发送一次
          const unsigned int TriangleInterval = 1;
          if (s_iCount >= TriangleInterval) {
            s_iCount = 0;
            //得到三角波的周期（s）
            double T = 1.0 / frequency;
            //校准指令发送的间隔时间
            s_iCountforwave++;
            //获得发送指令时真正的时间（ms）
            double time = s_iCountforwave * TriangleInterval * MOTION_CONTROL_INTEVAL;
            //若当前时间超过一个周期，校准时间使得时间回到周期开始
            if (time / 1000 >= T) {
              s_iCountforwave = 0;
            }
            //由当前时间得到当前控制值
            double tempf = time / 1000 * amplitude / T;
            //发送控制值
            SetValue(tempf + bias);
          }
          break;
        }
      case MODE_SINE: { // 正弦波时SineInterval 个Interval 发送一次
          const unsigned int SineInterval = 1;
          if (s_iCount >= SineInterval) {
            s_iCount = 0;
            //得到正弦波的周期（s）
            double T = 1.0 / frequency;
            //校准指令发送的间隔时间
            s_iCountforwave++;
            //获得发送指令时真正的时间（ms）
            double time = s_iCountforwave * SineInterval * MOTION_CONTROL_INTEVAL;
            //若当前时间超过一个周期，校准时间使得时间回到周期开始
            if (time / 1000 >= T) {
              s_iCountforwave = 0;
            }
            //由当前时间得到当前控制值
            double tempf = sin(time / 1000 * frequency * 2 * PI) * amplitude;
            //发送控制值
            SetValue((short)(tempf + bias));
          }
          break;
        }
    }
  }
}

void Move::SetValue(double value)
{
  int workMode = uiMove->cmbWorkMode->currentIndex();
  switch(workMode) // 不同控制模式，控制指令不同
  {
    case MODE_OPEN: {
        int pwm = (int)value;
        jointBeingUsed->setTagPWM(pwm);
        break;
      }
    case MODE_CURRENT: {
        int curI = (int)value;
        jointBeingUsed->setTagI(curI);
        break;
      }
    case MODE_SPEED: {
        float speed = (float)value;
        jointBeingUsed->setTagSpeed(speed);
        break;
      }
    case MODE_POSITION: {
        float angle = (float)value;
        jointBeingUsed->setTagPos(angle, JOINT_ANGLE);
        break;
      }
    default: break;
  }
}

void Move::on_waveModeCombo_currentIndexChanged(int index)
{
  switch (index)
  {
    case MODE_MANUAL: {
        uiMove->frequencyLineEdit->setEnabled(false);
        uiMove->frequencyLineEdit->setText("");
        uiMove->AmplitudeLineEdit->setEnabled(false);
        uiMove->AmplitudeLineEdit->setText("");
      }
    case MODE_SQUARE: {
        uiMove->frequencyLineEdit->setEnabled(true);
        uiMove->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        uiMove->AmplitudeLineEdit->setEnabled(true);
        uiMove->AmplitudeLineEdit->setText("20");
        on_AmplitudeLineEdit_editingFinished();
      }
    case MODE_TRIANGLE: {
        uiMove->frequencyLineEdit->setEnabled(true);
        uiMove->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        uiMove->AmplitudeLineEdit->setEnabled(true);
        uiMove->AmplitudeLineEdit->setText("20");
        on_AmplitudeLineEdit_editingFinished();
      }
    case MODE_SINE: {
        uiMove->frequencyLineEdit->setEnabled(true);
        uiMove->frequencyLineEdit->setText("0.5");
        on_frequencyLineEdit_editingFinished();
        uiMove->AmplitudeLineEdit->setEnabled(true);
        uiMove->AmplitudeLineEdit->setText("20");
        on_AmplitudeLineEdit_editingFinished();
      }
  }
}

void Move::on_frequencyLineEdit_editingFinished()
{
  frequency = uiMove->frequencyLineEdit->text().toDouble();
}

void Move::on_AmplitudeLineEdit_editingFinished()
{
  amplitude = uiMove->AmplitudeLineEdit->text().toDouble();
}
