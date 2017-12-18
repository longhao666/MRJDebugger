#include "gripper.h"
#include <iostream>
#include <stdio.h>

#define PI 3.14159265358979323846

using std::cout;
using std::endl;

Gripper::Gripper(uint32_t ID, Driver *d) : Module(ID, d)
{
    for (int i = 0; i < CMDLENGTH_FINGER; ++i) {
        MemoryControlTable[i] = 0;
    }
    MemoryControlTable[SYS_ID_FINGER] = ID;
}

void Gripper::MemoryTableInit() {
    for (int i = 0; i < CMDLENGTH_FINGER; ++i) {
        busDriver->SendMsg(this->ID, CMDTYPE_RD, i, NULL, 0x02);
    }
}

bool Gripper::FingerMotorInfoUpdate(int type) {
    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((type & 0xff00) >> 8);
    data[0] = (uint8_t)(type & 0xff);

    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_GET_STATE_MASK_FINGER, data, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

bool Gripper::FingerClearErr() {

    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((0 & 0xff00) >> 8);
    data[0] = (uint8_t)(1 & 0xff);

    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_CLEAR_ERROR_FINGER, data, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

bool Gripper::setFingerCurPosAsZero() {

    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((0 & 0xff00) >> 8);
    data[0] = (uint8_t)(1 & 0xff);

    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_SET_ZERO_POS_FINGER, data, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

bool Gripper::setFingerID(int newid) {
    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((newid & 0xff00) >> 8);
    data[0] = (uint8_t)(newid & 0xff);
    bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_ID_FINGER, data, 2);
    return bSuccess;
}

bool Gripper::readFingerModelType() {

    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_MODEL_TYPE_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

int Gripper::getFingerModelType() {
    uint16_t data;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_MODEL_TYPE_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }

    return (int)data;
}

bool Gripper::readFingerFirmwareVer() {

    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_FW_VERSION_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

bool Gripper::readFingerLimTorque() {

    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, LIMIT_TORQUE_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

int Gripper::getFingerLimTorque() {
    uint16_t data;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, LIMIT_TORQUE_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return (int)data;
}

bool Gripper::setFingerLimTorque(int way) {
    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((way & 0xff00) >> 8);
    data[0] = (uint8_t)(way & 0xff);
    bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, LIMIT_TORQUE_FINGER, data, 2);
    return bSuccess;
}

bool Gripper::readFingerCurPosLeft() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_POSITION_LEFT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurPosLeft() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_POSITION_LEFT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float ang = (float)(data * 300.00 / 1023.00);
    return ang;
}

bool Gripper::readFingerCurPosRight() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_POSITION_RIGHT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurPosRight cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurPosRight() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_POSITION_RIGHT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float ang = (float)(data * 300.00 / 1023.00);
    return ang;
}

bool Gripper::readFingerCurSpeedLeft() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_SPEED_LEFT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurSpeedLeft cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurSpeedLeft() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_SPEED_LEFT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float speed = (float)(data * 114.00 / 1023.00);
    return speed;
}

bool Gripper::readFingerCurSpeedRight() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_SPEED_RIGHT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurSpeedRight cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurSpeedRight() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_SPEED_RIGHT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float speed = (float)(data * 114.00 / 1023.00);
    return speed;
}

bool Gripper::readFingerCurLoadLeft() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_LOAD_LEFT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurLoadLeft cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurLoadLeft() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_LOAD_LEFT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float load = (float)((data & 0x03ff)* 33.7 / 1023.0);
    return load;
}

bool Gripper::readFingerCurLoadRight() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_LOAD_RIGHT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurLoadRight cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurLoadRight() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_LOAD_RIGHT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float load = (float)((data & 0x03ff)* 33.7 / 1023.0);
    return load;
}

bool Gripper::readFingerCurVoltLeft() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_VOLTAGE_LEFT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurVoltLeft cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurVoltLeft() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_VOLTAGE_LEFT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return ((float)(0.1*data));
}

bool Gripper::readFingerCurVoltRight() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_VOLTAGE_RIGHT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurVoltRight cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerCurVoltRight() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_VOLTAGE_RIGHT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return ((float)(0.1*data));
}

bool Gripper::readFingerCurTempLeft() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_TEMP_LEFT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurTempLeft cmd is error!";
        return false;
    }
    return true;
}

int Gripper::getFingerCurTempLeft() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_TEMP_LEFT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return ((int)data);
}

bool Gripper::readFingerCurTempRight() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_TEMP_RIGHT_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerCurTempRight cmd is error!";
        return false;
    }
    return true;
}

int Gripper::getFingerCurTempRight() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_TEMP_RIGHT_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return ((int)data);
}

bool Gripper::readFingerEnableStatus() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_ENABLE_TORQUE_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerTagStatus cmd is error!";
        return false;
    }
    return true;
}

int Gripper::getFingerEnableStatus() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_ENABLE_TORQUE_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return ((int)data);
}

bool Gripper::setFingerEnableStatus(int status) {

    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((status & 0xff00) >> 8);
    data[0] = (uint8_t)(status & 0xff);
    bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_ENABLE_TORQUE_FINGER, data, 2);
    return bSuccess;
}

bool Gripper::setFingerEnableOnPower(int status) {

    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((status & 0xff00) >> 8);
    data[0] = (uint8_t)(status & 0xff);
    bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_ENABLE_ON_POWER_FINGER, data, 2);
    return bSuccess;
}

bool Gripper::readFingerTagSpeed() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, MOVING_SPEED_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerTagSpeed cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerTagSpeed() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, MOVING_SPEED_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float speed = (float)(data * 114.0 / 1023.0);
    return speed;
}

bool Gripper::setFingerTagSpeed(float speed) {
    uint16_t spe = (uint16_t)(speed * 1023.0 / 114.0);
    uint8_t data[2];
    data[1] = (uint8_t)((spe & 0xff00) >> 8);
    data[0] = (uint8_t)(spe & 0xff);
    bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, MOVING_SPEED_FINGER, data, 2);
    return bSuccess;
}

bool Gripper::readFingerTagOpenRadio() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_OPEN_ANGLE_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerTagPosRadio cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerTagOpenRadio() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_OPEN_ANGLE_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float ang = (float)(data * 300.00 / 1023.00);
    ang = (float)(ang * PI / 180.0);
    return ang;
}

bool Gripper::setFingerTagOpenRadio(float ang) {

    float angtmp = (float)(ang * 180.0 / PI);
    uint16_t  angle = (uint16_t)(angtmp * 1023.0 / 300.0);
    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((angle & 0xff00) >> 8);
    data[0] = (uint8_t)(angle & 0xff);
    bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_OPEN_ANGLE_FINGER, data, 2);
    return bSuccess;

}

bool Gripper::readFingerTagOpenAng() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_OPEN_ANGLE_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerTagPosAng cmd is error!";
        return false;
    }
    return true;
}

float Gripper::getFingerTagOpenAng() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_OPEN_ANGLE_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    float ang = (float)(data * 300.00 / 1023.00);
    return ang;
}

bool Gripper::setFingerTagOpenAng(float ang) {

    uint16_t  angle = (uint16_t)(ang * 1023.0 / 300.0);
    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((angle & 0xff00) >> 8);
    data[0] = (uint8_t)(angle & 0xff);
    bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_OPEN_ANGLE_FINGER, data, 2);
    return bSuccess;
}

bool Gripper::readFingerTagWorkMode() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_WORK_MODE_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend readFingerTagPosAng cmd is error!";
        return false;
    }
    return true;
}

int Gripper::getFingerTagWorkMode() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_WORK_MODE_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return (int)data;
}

bool Gripper::setFingerTagWorkMode(int mode) {

    if (mode == 1 || mode == 2) {
        uint8_t data[2] = { 0 };
        data[1] = (uint8_t)((mode & 0xff00) >> 8);
        data[0] = (uint8_t)(mode & 0xff);
        bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_WORK_MODE_FINGER, data, 2);
        return bSuccess;
    }
    else {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
}

bool Gripper::readFingerTagOpenStatus() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_OPEN_STA_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

int Gripper::getFingerTagOpenStatus() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_OPEN_STA_FINGER, data);
    if (!bSuccess) {
        return -10000;
    }
    return (int)data;
}

bool Gripper::setFingerTagOpenStatus(int mode) {

    if (mode == 0 || mode == 1) {
        uint8_t data[2] = { 0 };
        data[1] = (uint8_t)((mode & 0xff00) >> 8);
        data[0] = (uint8_t)(mode & 0xff);
        bool bSuccess = busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_OPEN_STA_FINGER, data, 2);
        return bSuccess;
    }
    else {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
}

bool Gripper::FingerSysSaveToFlash() {
    uint8_t data[2] = { 0 };
    data[1] = (uint8_t)((0 & 0xff00) >> 8);
    data[0] = (uint8_t)(1 & 0xff);
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_SAVE_TO_FLASH_FINGER, data, 0x02)) {
        busDriver->errorTxt += "\nsend save to flash error!";
        return false;
    }
    return true;
}

bool Gripper::readFingerError() {
    if (!busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_ERROR_FINGER, NULL, 0x02)) {
        busDriver->errorTxt += "\nsend update cmd is error!";
        return false;
    }
    return true;
}

void Gripper::getFingerError() {
    uint16_t data = 0;
    this->busDriver->GetValueInTable(this->ID, SYS_ERROR_FINGER, data);
    if (data & 0x01) {
        busDriver->errorTxt += "\nover voltage error";
    }
    if (data & 0x02) {
        busDriver->errorTxt += "\nunder voltage error";
    }
    if (data & 0x04) {
        busDriver->errorTxt += "\nover temperature error";
    }
    if (data & 0x08) {
        busDriver->errorTxt += "\nmotor RX28 error";
    }
}
