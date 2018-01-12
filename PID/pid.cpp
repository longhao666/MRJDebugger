#include <QString>
#include <QDateTime>
#include <QFileInfo>
#include <QDebug>
#include <QTextCodec>
#include <QFileDialog>
#include <QMessageBox>
#include "pid.h"
#include "ui_pid.h"

PID::PID(UserControlOnCan* can1, Joint* jointBeingUsed, QWidget* parent) :
  QWidget(parent),
  uiPID(new Ui::PID)
{
  uiPID->setupUi(this);
  this->can1 = can1;
  this->jointBeingUsed = jointBeingUsed;
}

PID::~PID()
{
  delete uiPID;
}

void PID::PIDInitialize(uint16_t ID)
{
  jointBeingUsed = can1->findJointID(ID); // 修改当前控制的模块ID
    if (jointBeingUsed == NULL) {
        return;
    }
    // 初始化API中的MCT里的PID
    for (int i = SEV_CURRENT_P; i <= SEV_POSITION_DS; i++) {
        can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,i,NULL,0x02);
        can1->controller.delayMs(1);
    }
    for (int i = M_CURRENT_P; i <= M_POSITION_DS; i++) {
        can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,i,NULL,0x02);
        can1->controller.delayMs(1);
    }
    for (int i = L_CURRENT_P; i <= L_POSITION_DS; i++) {
        can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,i,NULL,0x02);
        can1->controller.delayMs(1);
    }
    can1->controller.delayMs(5);
    on_adjustGroupComboBox_currentIndexChanged(uiPID->adjustGroupComboBox->currentIndex());

    // 初始化API中的MCT里的限制值
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MAX_CURRENT,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MAX_SPEED,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MAX_ACC,NULL,0x02);
    can1->controller.delayMs(1);
    can1->controller.SendMsg(jointBeingUsed->ID,CMDTYPE_RD,LIT_MIN_POSITION_L,NULL,0x08);
    can1->controller.delayMs(5);

    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_CURRENT,data_L);
    uiPID->maxCurLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_SPEED,data_L);
    uiPID->maxSpdLineEdit->setText(QString::number(data_L/100, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_ACC,data_L);
    uiPID->maxAccLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_L,data_L);
    uiPID->minPosLLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_L,data_L);
    uiPID->maxPosLLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_H,data_L);
    uiPID->minPosHLineEdit->setText(QString::number(data_L, 10));
    can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_H,data_L);
    uiPID->maxPosHLineEdit->setText(QString::number(data_L, 10));

}

void PID::showSEVPID()
{
    if (jointBeingUsed == NULL) {
        return;
    }



    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_P,data_L);
    uiPID->POS_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_I,data_L);
    uiPID->POS_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_D,data_L);
    uiPID->POS_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_POSITION_DS,data_L);
    uiPID->POS_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_P,data_L);
    uiPID->SPD_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_I,data_L);
    uiPID->SPD_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_D,data_L);
    uiPID->SPD_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_SPEED_DS,data_L);
    uiPID->SPD_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_CURRENT_P,data_L);
    uiPID->CUR_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_CURRENT_I,data_L);
    uiPID->CUR_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, SEV_CURRENT_D,data_L);
    uiPID->CUR_DSpinBox->setValue(data_L);
}

void PID::showMPID()
{
    if (jointBeingUsed == NULL) {
        return;
    }
    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_P,data_L);
    uiPID->POS_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_I,data_L);
    uiPID->POS_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_D,data_L);
    uiPID->POS_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_POSITION_DS,data_L);
    uiPID->POS_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_P,data_L);
    uiPID->SPD_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_I,data_L);
    uiPID->SPD_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_D,data_L);
    uiPID->SPD_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_SPEED_DS,data_L);
    uiPID->SPD_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, M_CURRENT_P,data_L);
    uiPID->CUR_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_CURRENT_I,data_L);
    uiPID->CUR_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, M_CURRENT_D,data_L);
    uiPID->CUR_DSpinBox->setValue(data_L);
}

void PID::showLPID()
{
    if (jointBeingUsed == NULL) {
        return;
    }
    uint16_t data_L = 0;

    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_P,data_L);
    uiPID->POS_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_I,data_L);
    uiPID->POS_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_D,data_L);
    uiPID->POS_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_POSITION_DS,data_L);
    uiPID->POS_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_P,data_L);
    uiPID->SPD_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_I,data_L);
    uiPID->SPD_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_D,data_L);
    uiPID->SPD_DSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_SPEED_DS,data_L);
    uiPID->SPD_DSSpinBox->setValue(data_L);

    can1->controller.GetValueInTable(jointBeingUsed->ID, L_CURRENT_P,data_L);
    uiPID->CUR_PSpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_CURRENT_I,data_L);
    uiPID->CUR_ISpinBox->setValue(data_L);
    can1->controller.GetValueInTable(jointBeingUsed->ID, L_CURRENT_D,data_L);
    uiPID->CUR_DSpinBox->setValue(data_L);
}

void PID::on_adjustGroupComboBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        showSEVPID();
        break;
    case 1:
        showMPID();
        break;
    case 2:
        showLPID();
        break;
    }
}

void PID::setNewPID(int value,uint8_t index)
{
    if (jointBeingUsed == NULL) {
        return;
    }
    // 向下位机请求数据
    uint8_t data[2] = {0,0};
    data[1] = (uint8_t)( (value & 0xff00) >> 8 );  // 小于256的数都是0
    data[0] = (uint8_t)( value & 0xff ); // 200: 1100 1000 & 1111 1111 = 0xc8 就是200;
    // data = 0xc800
    can1->controller.SendMsg(jointBeingUsed->ID, CMDTYPE_WR_NR, index, data, 2);
    can1->controller.delayMs(50);
//    qDebug("PID::setNewPID(): value = %d", value);
    can1->controller.SendMsg(jointBeingUsed->ID,
                             CMDTYPE_RD,
                             index,
                             NULL,
                             0x02);
    can1->controller.delayMs(50);
}

void PID::on_POS_PSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_P;
        break;
    case 1:
        index = M_POSITION_P;
        break;
    case 2:
        index = L_POSITION_P;
        break;
    }
    setNewPID(uiPID->POS_PSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->POS_PSpinBox->setValue(data_L);
}

void PID::on_POS_ISpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_I;
        break;
    case 1:
        index = M_POSITION_I;
        break;
    case 2:
        index = L_POSITION_I;
        break;
    }
    setNewPID(uiPID->POS_ISpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
//    qDebug() << data_L << "============POS_ISpinBox========1";
    uiPID->POS_ISpinBox->setValue(data_L);
}

void PID::on_POS_DSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_D;
        break;
    case 1:
        index = M_POSITION_D;
        break;
    case 2:
        index = L_POSITION_D;
        break;
    }
    setNewPID(uiPID->POS_DSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->POS_DSpinBox->setValue(data_L);

}

void PID::on_POS_DSSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_POSITION_DS;
        break;
    case 1:
        index = M_POSITION_DS;
        break;
    case 2:
        index = L_POSITION_DS;
        break;
    }
    setNewPID(uiPID->POS_DSSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->POS_DSSpinBox->setValue(data_L);

}

void PID::on_SPD_PSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_P;
        break;
    case 1:
        index = M_SPEED_P;
        break;
    case 2:
        index = L_SPEED_P;
        break;
    }
    setNewPID(uiPID->SPD_PSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->SPD_PSpinBox->setValue(data_L);

}

void PID::on_SPD_ISpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_I;
        break;
    case 1:
        index = M_SPEED_I;
        break;
    case 2:
        index = L_SPEED_I;
        break;
    }
    int a = uiPID->SPD_ISpinBox->value();
//    qDebug() << "====SPD_ISpinBox===========" << a;
    setNewPID(uiPID->SPD_ISpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
//    qDebug() << data_L << "====SPD_ISpinBox===========" << a;
    uiPID->SPD_ISpinBox->setValue(data_L);

}

void PID::on_SPD_DSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_D;
        break;
    case 1:
        index = M_SPEED_D;
        break;
    case 2:
        index = L_SPEED_D;
        break;
    }
    setNewPID(uiPID->SPD_DSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->SPD_DSpinBox->setValue(data_L);

}

void PID::on_SPD_DSSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_SPEED_DS;
        break;
    case 1:
        index = M_SPEED_DS;
        break;
    case 2:
        index = L_SPEED_DS;
        break;
    }
    setNewPID(uiPID->SPD_DSSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->SPD_DSSpinBox->setValue(data_L);

}

void PID::on_CUR_PSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_CURRENT_P;
        break;
    case 1:
        index = M_CURRENT_P;
        break;
    case 2:
        index = L_CURRENT_P;
        break;
    }
    setNewPID(uiPID->CUR_PSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->CUR_PSpinBox->setValue(data_L);

}

void PID::on_CUR_ISpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_CURRENT_I;
        break;
    case 1:
        index = M_CURRENT_I;
        break;
    case 2:
        index = L_CURRENT_I;
        break;
    }
    setNewPID(uiPID->CUR_ISpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->CUR_ISpinBox->setValue(data_L);

}

void PID::on_CUR_DSpinBox_editingFinished()
{
    int index = 0;

    switch(uiPID->adjustGroupComboBox->currentIndex()) {
    case 0:
        index = SEV_CURRENT_D;
        break;
    case 1:
        index = M_CURRENT_D;
        break;
    case 2:
        index = L_CURRENT_D;
        break;
    }
    setNewPID(uiPID->CUR_DSpinBox->value(),index);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, index,data_L);
    }
    uiPID->CUR_DSpinBox->setValue(data_L);

}

void PID::on_maxCurLineEdit_editingFinished()
{
    setNewPID(uiPID->maxCurLineEdit->text().toInt(),LIT_MAX_CURRENT);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_CURRENT,data_L);
    }
    uiPID->maxCurLineEdit->setText(QString::number(data_L, 10));
}

void PID::on_maxSpdLineEdit_editingFinished()
{
    setNewPID(uiPID->maxSpdLineEdit->text().toInt()*100,LIT_MAX_SPEED);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_SPEED,data_L);
    }
    uiPID->maxSpdLineEdit->setText(QString::number(data_L/100, 10));
}

void PID::on_maxAccLineEdit_editingFinished()
{
    setNewPID(uiPID->maxAccLineEdit->text().toInt(),LIT_MAX_ACC);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_ACC,data_L);
    }
//    qDebug() << data_L << "====maxAccLineEdit================1";
    uiPID->maxAccLineEdit->setText(QString::number(data_L, 10));
}

void PID::on_minPosLLineEdit_editingFinished()
{
    setNewPID(uiPID->minPosLLineEdit->text().toInt(),LIT_MIN_POSITION_L);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_L,data_L);
    }
//    qDebug() << data_L << "====================1";
    uiPID->minPosLLineEdit->setText(QString::number(data_L, 10));
}

void PID::on_maxPosLLineEdit_editingFinished()
{
    setNewPID(uiPID->maxPosLLineEdit->text().toInt(),LIT_MAX_POSITION_L);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_L,data_L);
    }
    uiPID->maxPosLLineEdit->setText(QString::number(data_L, 10));
}

void PID::on_minPosHLineEdit_editingFinished()
{
    setNewPID(uiPID->minPosHLineEdit->text().toInt(),LIT_MIN_POSITION_H);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MIN_POSITION_H,data_L);
    }
    uiPID->minPosHLineEdit->setText(QString::number(data_L, 10));
}

void PID::on_maxPosHLineEdit_editingFinished()
{
    setNewPID(uiPID->maxPosHLineEdit->text().toInt(),LIT_MAX_POSITION_H);

    uint16_t data_L = 0;

    if (jointBeingUsed != NULL) {
        can1->controller.GetValueInTable(jointBeingUsed->ID, LIT_MAX_POSITION_H,data_L);
    }
    uiPID->maxPosHLineEdit->setText(QString::number(data_L, 10));
}

void PID::btnSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存PID数据"), "./PID.dat", tr("PID数据文件(*.dat)"));
    if (fileName == "")
        return;
    QFile file(fileName);
    if (! file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug("%s", file.errorString().toStdString().c_str());
    int oldIndex = uiPID->adjustGroupComboBox->currentIndex();
    uiPID->adjustGroupComboBox->setCurrentIndex(0);
    QString data = uiPID->POS_PSpinBox->text() + tr("\n")
                 + uiPID->POS_ISpinBox->text() + tr("\n")
                 + uiPID->POS_DSpinBox->text() + tr("\n")
                 + uiPID->POS_DSSpinBox->text() + tr("\n")
                 + uiPID->SPD_PSpinBox->text() + tr("\n")
                 + uiPID->SPD_ISpinBox->text() + tr("\n")
                 + uiPID->SPD_DSpinBox->text() + tr("\n")
                 + uiPID->SPD_DSSpinBox->text() + tr("\n")
                 + uiPID->CUR_PSpinBox->text() + tr("\n")
                 + uiPID->CUR_ISpinBox->text() + tr("\n")
                 + uiPID->CUR_DSpinBox->text() + tr("\n"); // 第1组完成，需要切换组别
    uiPID->adjustGroupComboBox->setCurrentIndex(1);
    data += uiPID->POS_PSpinBox->text() + tr("\n")
                 + uiPID->POS_ISpinBox->text() + tr("\n")
                 + uiPID->POS_DSpinBox->text() + tr("\n")
                 + uiPID->POS_DSSpinBox->text() + tr("\n")
                 + uiPID->SPD_PSpinBox->text() + tr("\n")
                 + uiPID->SPD_ISpinBox->text() + tr("\n")
                 + uiPID->SPD_DSpinBox->text() + tr("\n")
                 + uiPID->SPD_DSSpinBox->text() + tr("\n")
                 + uiPID->CUR_PSpinBox->text() + tr("\n")
                 + uiPID->CUR_ISpinBox->text() + tr("\n")
                 + uiPID->CUR_DSpinBox->text() + tr("\n"); // 第2组完成，需要切换组别
    uiPID->adjustGroupComboBox->setCurrentIndex(2);
    data += uiPID->POS_PSpinBox->text() + tr("\n")
                 + uiPID->POS_ISpinBox->text() + tr("\n")
                 + uiPID->POS_DSpinBox->text() + tr("\n")
                 + uiPID->POS_DSSpinBox->text() + tr("\n")
                 + uiPID->SPD_PSpinBox->text() + tr("\n")
                 + uiPID->SPD_ISpinBox->text() + tr("\n")
                 + uiPID->SPD_DSpinBox->text() + tr("\n")
                 + uiPID->SPD_DSSpinBox->text() + tr("\n")
                 + uiPID->CUR_PSpinBox->text() + tr("\n")
                 + uiPID->CUR_ISpinBox->text() + tr("\n")
                 + uiPID->CUR_DSpinBox->text(); // 第3组完成，需要切换回组别
    uiPID->adjustGroupComboBox->setCurrentIndex(oldIndex);
    QByteArray ba = data.toLatin1();
    file.write(ba);
    file.close();
    QMessageBox::information(this,"NOTE","PID value has saved");
}

void PID::btnLoad_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开PID数据文件"),".",tr("PID数据文件(*.dat)"));
    if (fileName == "")
        return;
    QFile file(fileName);
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug("%s", file.errorString().toStdString().c_str());
    file.seek(0);
    QTextStream in(&file);
    int count = 0;
    int oldIndex = uiPID->adjustGroupComboBox->currentIndex(); // 保存当前的组别
    qDebug("%d", oldIndex);
    while (! in.atEnd()) {
        QString line = in.readLine();
        switch (count) {
            case 0: { // 不是on_adjustGroupComboBox_currentIndexChanged(0);
                uiPID->adjustGroupComboBox->setCurrentIndex(0);
                qDebug("on 0");
                uiPID->POS_PSpinBox->setValue(line.toInt());
                on_POS_PSpinBox_editingFinished();
                break;
            }
            case 1: {
                uiPID->POS_ISpinBox->setValue(line.toInt());
                on_POS_ISpinBox_editingFinished();
                break;
            }
            case 2: {
                uiPID->POS_DSpinBox->setValue(line.toInt());
                on_POS_DSpinBox_editingFinished();
                break;
            }
            case 3: {
                uiPID->POS_DSSpinBox->setValue(line.toInt());
                on_POS_DSSpinBox_editingFinished();
                break;
            }
            case 4: {
                uiPID->SPD_PSpinBox->setValue(line.toInt());
                on_SPD_PSpinBox_editingFinished();
                break;
            }
            case 5: {
                uiPID->SPD_ISpinBox->setValue(line.toInt());
                on_SPD_ISpinBox_editingFinished();
                break;
            }
            case 6: {
                uiPID->SPD_DSpinBox->setValue(line.toInt());
                on_SPD_DSpinBox_editingFinished();
                break;
            }
            case 7: {
                uiPID->SPD_DSSpinBox->setValue(line.toInt());
                on_SPD_DSSpinBox_editingFinished();
                break;
            }
            case 8: {
                uiPID->CUR_PSpinBox->setValue(line.toInt());
                on_CUR_PSpinBox_editingFinished();
                break;
            }
            case 9: {
                uiPID->CUR_ISpinBox->setValue(line.toInt());
                on_CUR_ISpinBox_editingFinished();
                break;
            }
            case 10: {
                uiPID->CUR_DSpinBox->setValue(line.toInt());
                on_CUR_DSpinBox_editingFinished();
                break;
            }
            case 11: { // 不是on_adjustGroupComboBox_currentIndexChanged(1);
                uiPID->adjustGroupComboBox->setCurrentIndex(1);
                qDebug("on 1");
                uiPID->POS_PSpinBox->setValue(line.toInt());
                on_POS_PSpinBox_editingFinished();
                break;
            }
            case 12: {
                uiPID->POS_ISpinBox->setValue(line.toInt());
                on_POS_ISpinBox_editingFinished();
                break;
            }
            case 13: {
                uiPID->POS_DSpinBox->setValue(line.toInt());
                on_POS_DSpinBox_editingFinished();
                break;
            }
            case 14: {
                uiPID->POS_DSSpinBox->setValue(line.toInt());
                on_POS_DSSpinBox_editingFinished();
                break;
            }
            case 15: {
                uiPID->SPD_PSpinBox->setValue(line.toInt());
                on_SPD_PSpinBox_editingFinished();
                break;
            }
            case 16: {
                uiPID->SPD_ISpinBox->setValue(line.toInt());
                on_SPD_ISpinBox_editingFinished();
                break;
            }
            case 17: {
                uiPID->SPD_DSpinBox->setValue(line.toInt());
                on_SPD_DSpinBox_editingFinished();
                break;
            }
            case 18: {
                uiPID->SPD_DSSpinBox->setValue(line.toInt());
                on_SPD_DSSpinBox_editingFinished();
                break;
            }
            case 19: {
                uiPID->CUR_PSpinBox->setValue(line.toInt());
                on_CUR_PSpinBox_editingFinished();
                break;
            }
            case 20: {
                uiPID->CUR_ISpinBox->setValue(line.toInt());
                on_CUR_ISpinBox_editingFinished();
                break;
            }
            case 21: {
                uiPID->CUR_DSpinBox->setValue(line.toInt());
                on_CUR_DSpinBox_editingFinished();
                break;
            }
            case 22: { // 不是on_adjustGroupComboBox_currentIndexChanged(2);
                uiPID->adjustGroupComboBox->setCurrentIndex(2);
                qDebug("on 2");
                uiPID->POS_PSpinBox->setValue(line.toInt());
                on_POS_PSpinBox_editingFinished();
                break;
            }
            case 23: {
                uiPID->POS_ISpinBox->setValue(line.toInt());
                on_POS_ISpinBox_editingFinished();
                break;
            }
            case 24: {
                uiPID->POS_DSpinBox->setValue(line.toInt());
                on_POS_DSpinBox_editingFinished();
                break;
            }
            case 25: {
                uiPID->POS_DSSpinBox->setValue(line.toInt());
                on_POS_DSSpinBox_editingFinished();
                break;
            }
            case 26: {
                uiPID->SPD_PSpinBox->setValue(line.toInt());
                on_SPD_PSpinBox_editingFinished();
                break;
            }
            case 27: {
                uiPID->SPD_ISpinBox->setValue(line.toInt());
                on_SPD_ISpinBox_editingFinished();
                break;
            }
            case 28: {
                uiPID->SPD_DSpinBox->setValue(line.toInt());
                on_SPD_DSpinBox_editingFinished();
                break;
            }
            case 29: {
                uiPID->SPD_DSSpinBox->setValue(line.toInt());
                on_SPD_DSSpinBox_editingFinished();
                break;
            }
            case 30: {
                uiPID->CUR_PSpinBox->setValue(line.toInt());
                on_CUR_PSpinBox_editingFinished();
                break;
            }
            case 31: {
                uiPID->CUR_ISpinBox->setValue(line.toInt());
                on_CUR_ISpinBox_editingFinished();
                break;
            }
            case 32: {
                uiPID->CUR_DSpinBox->setValue(line.toInt());
                on_CUR_DSpinBox_editingFinished();
                break;
            }
        }
        ++count;
    }
    file.close();

    uiPID->adjustGroupComboBox->setCurrentIndex(oldIndex); // 切换回组别
    QMessageBox::information(this,"NOTE","PID value has loaded");
}
