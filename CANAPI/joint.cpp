#include "joint.h"

#define PI 3.14159265358979323846

Joint::Joint(uint32_t ID, Driver *d) : Module(ID, d)
{
    for (int i = 0; i < CMDLENGTH; ++i) {
        MemoryControlTable[i] = 0;
    }
    MemoryControlTable[SYS_ID] = ID;
}

bool Joint::setSaveToFlash() {
    uint16_t tmp = 1;
    uint8_t data[2] = {0, 0};
    data[1] = (uint8_t)((tmp & 0xff00) >> 8);
    data[0] = (uint8_t)(tmp & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_SAVE_TO_FLASH, data, 2)) {
        busDriver->errorTxt += "\nSend setSaveToFlash CMD Error!";
        return false;
    }
    return true;
}

bool Joint::setEnable(bool enabled) {
    /**
     * @brief tmp
     * 0
     * 0000 0000 & 1111 1111 0000 0000 = 0000 0000; 0x00
     * 0000 0000 & 1111 1111 = 0000 0000; 0x00
     *
     * 1
     * 0000 0001 & 1111 1111 0000 0000 = 0000 0000; 0x00
     * 0000 0001 & 1111 1111 = 0000 0001; 0x01
     */
    uint16_t tmp = enabled;
    uint8_t data[2] = {0, 0};
    data[1] = (uint8_t)((tmp & 0xff00) >> 8);
    data[0] = (uint8_t)(tmp & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_ENABLE_DRIVER, data, 2)) {
        busDriver->errorTxt += "\nSend setEnable CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateEnable() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_ENABLE_DRIVER, NULL, 2)) {
        busDriver->errorTxt += "\nSend updateEnable CMD Error!";
        return false;
    }
    return true;
}

bool Joint::getEnableStatus() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_ENABLE_DRIVER, data);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetEnableStatus is failed!";
    }
    return (bool)data;
}

bool Joint::clearErrStatus() {
    uint16_t tmp = 1;
    uint8_t data[2] = {0, 0};
    data[1] = (uint8_t)((tmp & 0xff00) >> 8);
    data[0] = (uint8_t)(tmp & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_CLEAR_ERROR, data, 2)) {
        busDriver->errorTxt += "\nSend clearErrStatus CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateErrStatus() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_ERROR, NULL, 2)) {
        busDriver->errorTxt += "\nSend updateErrStatus CMD Error!";
        return false;
    }
    return true;
}

uint8_t Joint::getErrStatus() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_ERROR, data);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetErrStatus is failed!";
    }
    return (uint8_t)data;
}

bool Joint::setWorkMode(uint8_t workMode) {
    if (workMode == MODE_OPEN || workMode == MODE_CURRENT || workMode == MODE_SPEED || workMode == MODE_POSITION) {
        uint8_t data[2] = {0, 0};
        data[1] = (uint8_t)((workMode & 0xff00) >> 8);
        data[0] = (uint8_t)(workMode & 0xff);
        if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_WORK_MODE, data, 2)) {
            busDriver->errorTxt += "\nSend setWorkMode CMD Error!";
            return false;
        }
    }
    else {
        busDriver->errorTxt += "\nInvalid workMode!";
        return false;
    }
    return true;
}

bool Joint::updateWorkMode() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_WORK_MODE, NULL, 2)) {
        busDriver->errorTxt += "\nSend updateWorkMode CMD Error!";
        return false;
    }
    return true;
}

uint8_t Joint::getWorkMode() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_WORK_MODE, data);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetWorkMode is failed!";
    }
    return data;
}

bool Joint::setZeroPos() {
    if (MemoryControlTable[TAG_WORK_MODE] == MODE_POSITION) {
        uint16_t tmp = 1;
        uint8_t data[2] = {0, 0};
        data[1] = (uint8_t)((tmp & 0xff00) >> 8);
        data[0] = (uint8_t)(tmp & 0xff);
        if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_SET_ZERO_POS, data, 2)) {
            busDriver->errorTxt += "\nSend setZeroPos CMD Error!";
            return false;
        }
        return true;
    }
    else {
        busDriver->errorTxt += "\nFailed! Please set workmode on MODE_POSITION!";
        return false;
    }
}

bool Joint::setID(uint16_t newid) {
    uint8_t data[2] = {0, 0};
    data[1] = (uint8_t)((newid & 0xff00) >> 8);
    data[0] = (uint8_t)(newid & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, SYS_ID, data, 2)) {
        busDriver->errorTxt += "\nSend setID CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateCurI() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_CURRENT_L, NULL, 4)) {
        busDriver->errorTxt += "\nSend updateCurI CMD Error!";
        return false;
    }
    return true;
}

int Joint::getCurI() {
    uint16_t data_L = 0;
    uint16_t data_H = 0;
    int currentI = 0;

    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_CURRENT_L, data_L);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetCurI is failed!";
    }
    bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_CURRENT_H, data_H);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetCurI is failed!";
    }
    currentI = (int)(data_L + data_H * 65536);
    return currentI;
}

bool Joint::updateCurSpeed() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_SPEED_L, NULL, 4)) {
        busDriver->errorTxt += "\nSend updateCurSpeed CMD Error!";
        return false;
    }
    return true;
}

float Joint::getCurSpeed() {
    uint16_t data_L = 0;
    uint16_t data_H = 0;
    float currentSpeed = 0;

    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_SPEED_L, data_L);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetCurSpeed is failed!";
    }
    bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_SPEED_H, data_H);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetCurSpeed is failed!";
    }
    currentSpeed = (float) ((data_L + data_H * 65536) * 60.0 / 65536.0);

    // 根据模块类型转换减速比
    switch (MemoryControlTable[SYS_MODEL_TYPE]) {
        case MODEL_TYPE_M14:
            currentSpeed /= GEAR_RATIO_M14;
        break;
        case MODEL_TYPE_M14E:
            currentSpeed /= GEAR_RATIO_M14E;
        break;
        case MODEL_TYPE_M17:
            currentSpeed /= GEAR_RATIO_M17;
        break;
        case MODEL_TYPE_M17E:
            currentSpeed /= GEAR_RATIO_M17E;
        break;
        case MODEL_TYPE_M20:
            currentSpeed /= GEAR_RATIO_M20;
        break;
        case MODEL_TYPE_LIFT:
            currentSpeed /= GEAR_RATIO_LIFT;
        break;
        default:
            busDriver->errorTxt += "\nUnknow model!";
    }
    return currentSpeed;
}

bool Joint::updateCurPos() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_POSITION_L, NULL, 4)) {
        busDriver->errorTxt += "\nSend updateCurPos CMD Error!";
        return false;
    }
    return true;
}

float Joint::getCurPos(int cmd) {
    uint16_t data_L = 0;
    uint16_t data_H = 0;
    float ang = 0;

    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_POSITION_L, data_L);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetCurPos is failed!";
    }
    bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_POSITION_H, data_H);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetCurPos is failed!";
    }
    ang = (float) ((data_L + data_H * 65536) * 360.0 / 65536.0);
    if (cmd == JOINT_RADIAN) {
        ang = (float)(ang * PI / 180.00);
    }
    // 不需要根据模块类型处理减速比
    return ang;
}

bool Joint::setTagPWM(uint8_t pwm) {
    uint8_t data[2] = {0, 0};
    data[1] = (uint8_t)((pwm & 0xff00) >> 8);
    data[0] = (uint8_t)(pwm & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_OPEN_PWM, data, 2)) {
        busDriver->errorTxt += "\nSend setTagPWM CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateTagPWM() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_OPEN_PWM, NULL, 2)) {
        busDriver->errorTxt += "\nSend updateTagPWM CMD Error!";
        return false;
    }
    return true;
}

uint8_t Joint::getTagPWM() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_OPEN_PWM, data);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetTagPWM is failed!";
    }
    return data;
}

bool Joint::setTagI(int curI) {
    int units = curI;
    uint8_t data[4] = {0, 0, 0, 0};
    data[3] = (uint8_t)(units >> 24);
    data[2] = (uint8_t)((units >> 16) & 0xff);
    data[1] = (uint8_t)((units >> 8) & 0xff);
    data[0] = (uint8_t)(units & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_CURRENT_L, data, 4)) {
        busDriver->errorTxt += "\nSend setTagI CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateTagI() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_CURRENT_L, NULL, 4)) {
        busDriver->errorTxt += "\nSend updateTagI CMD Error!";
        return false;
    }
    return true;
}

int Joint::getTagI() {
    uint16_t data_L = 0;
    uint16_t data_H = 0;
    int currentI = 0;

    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_CURRENT_L, data_L);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetTagI is failed!";
    }
    bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_CURRENT_H, data_H);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetTagI is failed!";
    }
    currentI = (int)(data_L + data_H * 65536);
    return currentI;
}

bool Joint::setTagSpeed(float speed) {
    // 根据模块类型转换减速比
    switch (MemoryControlTable[SYS_MODEL_TYPE]) {
        case MODEL_TYPE_M14:
            speed *= GEAR_RATIO_M14;
        break;
        case MODEL_TYPE_M14E:
            speed *= GEAR_RATIO_M14E;
        break;
        case MODEL_TYPE_M17:
            speed *= GEAR_RATIO_M17;
        break;
        case MODEL_TYPE_M17E:
            speed *= GEAR_RATIO_M17E;
        break;
        case MODEL_TYPE_M20:
            speed *= GEAR_RATIO_M20;
        break;
        case MODEL_TYPE_LIFT:
            speed *= GEAR_RATIO_LIFT;
        break;
        default:
            busDriver->errorTxt += "\nUnknow model!";
    }
    int units = (int)(speed * 65536 / 60);
    uint8_t data[4] = {0, 0, 0, 0};
    data[3] = (uint8_t)(units >> 24);
    data[2] = (uint8_t)((units >> 16) & 0xff);
    data[1] = (uint8_t)((units >> 8) & 0xff);
    data[0] = (uint8_t)(units & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_SPEED_L, data, 4)) {
        busDriver->errorTxt += "\nSend setTagSpeed CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateTagSpeed() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_SPEED_L, NULL, 4)) {
        busDriver->errorTxt += "\nSend updateTagSpeed CMD Error!";
        return false;
    }
    return true;
}

float Joint::getTagSpeed() {
    uint16_t data_L = 0;
    uint16_t data_H = 0;
    float tagSpeed = 0;

    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_SPEED_L, data_L);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetTagSpeed is failed!";
    }
    bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_SPEED_H, data_H);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetTagSpeed is failed!";
    }
    tagSpeed = (float) ((data_L + data_H * 65536) * 60.0 / 65536.0);

    // 根据模块类型转换减速比
    switch (MemoryControlTable[SYS_MODEL_TYPE]) {
        case MODEL_TYPE_M14:
            tagSpeed /= GEAR_RATIO_M14;
        break;
        case MODEL_TYPE_M14E:
            tagSpeed /= GEAR_RATIO_M14E;
        break;
        case MODEL_TYPE_M17:
            tagSpeed /= GEAR_RATIO_M17;
        break;
        case MODEL_TYPE_M17E:
            tagSpeed /= GEAR_RATIO_M17E;
        break;
        case MODEL_TYPE_M20:
            tagSpeed /= GEAR_RATIO_M20;
        break;
        case MODEL_TYPE_LIFT:
            tagSpeed /= GEAR_RATIO_LIFT;
        break;
        default:
            busDriver->errorTxt += "\nUnknow model!";
    }
    return tagSpeed;
}

bool Joint::setTagPos(float angle, int cmd) {
    if (cmd == JOINT_RADIAN) {
        angle = (float)(angle * 180.0 / PI);
    }
    // 不需要根据模块类型处理减速比
    int units = (int)(angle * 65536 / 360);
    uint8_t data[4] = {0, 0, 0, 0};
    data[3] = (uint8_t)(units >> 24);
    data[2] = (uint8_t)((units >> 16) & 0xff);
    data[1] = (uint8_t)((units >> 8) & 0xff);
    data[0] = (uint8_t)(units & 0xff);
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_WR_NR, TAG_POSITION_L, data, 4)) {
        busDriver->errorTxt += "\nSend setTagPos CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateTagPos() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, TAG_POSITION_L, NULL, 4)) {
        busDriver->errorTxt += "\nSend updateTagSpeed CMD Error!";
        return false;
    }
    return true;
}

float Joint::getTagPos(int cmd) {
    uint16_t data_L = 0;
    uint16_t data_H = 0;
    float ang = 0;

    bool bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_POSITION_L, data_L);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetTagSpeed is failed!";
    }
    bSuccess = this->busDriver->GetValueInTable(this->ID, TAG_POSITION_H, data_H);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetTagSpeed is failed!";
    }
    ang = (float) ((data_L + data_H * 65536) * 360.0 / 65536.0);
    if (cmd == JOINT_RADIAN) {
        ang = (float)(ang * PI / 180.00);
    }
    // 不需要根据模块类型处理减速比
    return ang;
}

bool Joint::updateGearRadioType() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_REDU_RATIO, NULL, 4)) {
        busDriver->errorTxt += "\nSend updateGearRadioType CMD Error!";
        return false;
    }
    return true;
}

uint16_t Joint::getGearRadioType() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_REDU_RATIO, data);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetGearRadioType is failed!";
    }
    return data;
}

bool Joint::updateModelType() {
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SYS_MODEL_TYPE, NULL, 2)) {
        busDriver->errorTxt += "\nSend updateModelType CMD Error!";
        return false;
    }
    return true;
}

uint16_t Joint::getModelType() {
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SYS_MODEL_TYPE, data);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetModelType is failed!";
    }
    return data;
}

void Joint::MemoryTableInit() {
    for (int i = 0; i < CMDLENGTH; ++i) {
        busDriver->SendMsg(this->ID, CMDTYPE_RD, i, NULL, 0x02);
    }
}

bool Joint::setScpMask(uint16_t mask)
{
    uint8_t data[2] = {0, 0};
    data[1] = (uint8_t)((mask & 0xff00) >> 8);
    data[0] = (uint8_t)(mask & 0x00ff);
    if (! this->busDriver->SendMsg(this->ID,
                                   CMDTYPE_WR_NR,
                                   SCP_MASK,
                                   data,
                                   2)) {
        busDriver->errorTxt += "\nSend setScpMask CMD Error!";
        return false;
    }
    return true;
}

bool Joint::updateScpMask()
{
    if (! this->busDriver->SendMsg(this->ID, CMDTYPE_RD, SCP_MASK, NULL, 2)) {
        busDriver->errorTxt += "\nSend updateScpMask CMD Error!";
        return false;
    }
    return true;
}

uint16_t Joint::getScpMask()
{
    uint16_t data = 0;
    bool bSuccess = this->busDriver->GetValueInTable(this->ID, SCP_MASK, data);
    if (!bSuccess) {
        busDriver->errorTxt += "\ngetScpMask is failed!";
    }
    return data;
}
