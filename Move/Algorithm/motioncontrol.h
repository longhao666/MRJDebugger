/*
 * 这个类是用于对关节模块进行10ms周期控制的，基于Qt
 * 利用Qt的高精度定时器确保控制周期
 * 提供一个设置目标值的public函数，设置完目标值之后，根据需要进行插值，插值的结果添加到队列中
 */
#ifndef MOTIONCONTROL_H
#define MOTIONCONTROL_H

#include <QObject>
#include <QTimer>
#include <queue>
#include "joint.h"

using std::queue;

class MotionControl : public QObject
{
    Q_OBJECT
public:
    // 构造函数里初始化要控制的joint
    explicit MotionControl(Joint* tempJoint, QObject *parent = 0);

    // 为joint设置目标位置
    void SetTag(double Tag);

protected:
    Joint* joint;           // 要控制的joint
    double lastOut;         // 保存上一次发送的目标值，初始化时为joint的目标值
    queue<double> OutQueue; // 待发送的目标值队列

private:
    QTimer sendTimer;       // 发送消息的定时器

private slots:
    // 发送消息的定时器的槽函数
    void sendTimerSlot();
};

#endif // MOTIONCONTROL_H
