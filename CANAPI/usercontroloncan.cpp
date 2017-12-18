#include "usercontroloncan.h"

using std::vector;

#define UPDATEIETMPOS 0
#define UPDATEIETMSPD 1
#define UPDATEIETMCUR 2

UserControlOnCan::UserControlOnCan()
{
}

UserControlOnCan::~UserControlOnCan()
{
}


/**
 * @brief UserControlOnCan::Init
 * @param deviceName
 * @return
 */
bool UserControlOnCan::Init(const char * deviceName)
{
    bool isOK = false;
    if (controller.BusInit(deviceName)) {
        isOK = controller.ThreadStart();
    }
    return isOK;
}

bool UserControlOnCan::Close(const char *deviceName)
{
    return controller.BusClose(deviceName);
}

bool UserControlOnCan::updateJointCurPos(uint16_t theID)
{
    return updateJointCurItem(theID, UPDATEIETMPOS);
}

float UserControlOnCan::readJointCurPos(uint16_t theID, int cmd)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        return 0;
    } else {
        return j->getCurPos(cmd);
    }
}

bool UserControlOnCan::setJointTagPos(uint16_t theID, float angle, int cmd)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        controller.errorTxt = "Can not find the ID!";
        return false;
    } else {
        controller.errorTxt = "";
        return j->setTagPos(angle, cmd);
    }
}

bool UserControlOnCan::setJointAutoUpdateCurPos(uint16_t theID, bool enabled)
{
    return setJointAutoUpdate(theID, enabled, MASK_MEAPOS);
}

bool UserControlOnCan::updateJointCurSpd(uint16_t theID)
{
    return updateJointCurItem(theID, UPDATEIETMSPD);
}

float UserControlOnCan::readJointCurSpd(uint16_t theID)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        return 0;
    } else {
        return j->getCurSpeed();
    }
}

bool UserControlOnCan::setJointTagSpd(uint16_t theID, float speed)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        controller.errorTxt = "Can not find the ID!";
        return false;
    } else {
        controller.errorTxt = "";
        return j->setTagSpeed(speed);
    }
}

bool UserControlOnCan::setJointAutoUpdateCurSpd(uint16_t theID, bool enabled)
{
    return setJointAutoUpdate(theID, enabled, MASK_MEASPD);
}

bool UserControlOnCan::updateJointCurI(uint16_t theID)
{
    return updateJointCurItem(theID, UPDATEIETMCUR);
}

float UserControlOnCan::readJointCurI(uint16_t theID)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        controller.errorTxt = "Can not find the ID!";
        return false;
    } else {
        return j->getCurI();
    }
}

bool UserControlOnCan::setJointTagI(uint16_t theID, int curI)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        controller.errorTxt = "Can not find the ID!";
        return false;
    } else {
        controller.errorTxt = "";
        return j->setTagI(curI);
    }
}

bool UserControlOnCan::setJointAutoUpdateCurI(uint16_t theID, bool enabled)
{
    return setJointAutoUpdate(theID, enabled, MASK_MEACUR);
}

bool UserControlOnCan::setGripperTagOpenStatus(uint16_t theID, int mode)
{
    Gripper * g = findGripperID(theID);
    if (g == NULL) {
        controller.errorTxt = "Can not find the ID!";
        return false;
    }
    controller.errorTxt = "";
    bool bSuccess = g->setFingerTagOpenStatus(mode);
    return bSuccess;
}

Joint * UserControlOnCan::findJointID(uint16_t theID)
{
    vector<Joint>::iterator i = controller.allJoint.begin();
    while (i != controller.allJoint.end()) {
        if (i->ID == theID) {
            break;
        }
        ++i;
    }
    Joint * j = NULL;
    if (i != controller.allJoint.end()) {
        j = &*i;
    } else {
        j = NULL;
    }
    return j;
}

Gripper *UserControlOnCan::findGripperID(uint16_t theID)
{
    vector<Gripper>::iterator i = controller.allGripper.begin();
    while (i != controller.allGripper.end()) {
        if (i->ID == theID) {
            break;
        }
        ++i;
    }
    Gripper * g = NULL;
    if (i != controller.allGripper.end()) {
        g = &*i;
    } else {
        g = NULL;
    }
    return g;
}

bool UserControlOnCan::setJointAutoUpdate(uint16_t theID, bool enabled, uint8_t label)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        controller.errorTxt = "Can not find the ID!";
        return false;
    }
    // 先清空errorTxt，因为下面调用的函数的错误信息是追加上的
    controller.errorTxt = "";
    j->updateScpMask();
    controller.delayMs(5); // wait response
    uint16_t mask = j->getScpMask();
    enabled ? mask |= label : mask &= ~(label);
    bool bSuccess = j->setScpMask(mask);
    controller.delayMs(10); // makesure that when leave this function, user can get data
    return bSuccess;
}

bool UserControlOnCan::updateJointCurItem(uint16_t theID, uint8_t item)
{
    Joint * j = findJointID(theID);
    if (j == NULL) {
        controller.errorTxt = "Can not find the ID!";
        return false;
    } else {
        // 先清空errorTxt，因为pdateCurPos()等函数的错误信息是追加上的
        controller.errorTxt = "";
        switch(item) {
        case 0: {
            return j->updateCurPos();
        }
        case 1: {
            return j->updateCurSpeed();
        }
        case 2: {
            return j->updateCurI();
        }
        }
    }
    controller.errorTxt = "Error in updateJointCurItem()";
    return false;
}
