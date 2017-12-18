#ifndef USERCONTROLONCAN_H
#define USERCONTROLONCAN_H

#include "driveroncan.h"

class CANAPI_EXPORT UserControlOnCan
{
public:
    UserControlOnCan();
    ~UserControlOnCan();
    bool Init(const char * deviceName);
    bool Close(const char* deviceName);

    bool updateJointCurPos(uint16_t theID);
    float readJointCurPos(uint16_t theID, int cmd = JOINT_RADIAN);
    bool setJointTagPos(uint16_t theID, float angle, int cmd = JOINT_RADIAN);
    bool setJointAutoUpdateCurPos(uint16_t theID, bool enabled);

    bool updateJointCurSpd(uint16_t theID);
    float readJointCurSpd(uint16_t theID);
    bool setJointTagSpd(uint16_t theID, float speed);
    bool setJointAutoUpdateCurSpd(uint16_t theID, bool enabled);

    bool updateJointCurI(uint16_t theID);
    float readJointCurI(uint16_t theID);
    bool setJointTagI(uint16_t theID, int curI);
    bool setJointAutoUpdateCurI(uint16_t theID, bool enabled);

    bool setGripperTagOpenStatus(uint16_t theID, int mode);
    DriverOnCan controller;

    Joint *findJointID(uint16_t theID);
    Gripper *findGripperID(uint16_t theID);
private:
    bool setJointAutoUpdate(uint16_t theID, bool enabled, uint8_t label);
    bool updateJointCurItem(uint16_t theID, uint8_t item);
};

#endif // USERCONTROLONCAN_H
