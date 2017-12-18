#include "motioncontrol.h"
#include <QDebug>
#include <QTime>
#include <math.h>

#define CONTROLINTERVAL 10
#define PI 3.1415926

MotionControl::MotionControl(Joint *tempJoint, QObject *parent) :
    QObject(parent),
    joint(tempJoint)
{
    // 清空队列
    while (!OutQueue.empty()) {
        OutQueue.pop();
    }
    // 设置定时器
    sendTimer.setTimerType(Qt::PreciseTimer);
    connect( &sendTimer, SIGNAL(timeout()), this, SLOT(sendTimerSlot()) );
    sendTimer.setInterval(CONTROLINTERVAL);
    emit sendTimer.start();
    // 初始化lastOut
    if (joint != NULL) {
        lastOut = joint->getCurPos(JOINT_RADIAN);
//        qDebug() << "lastOut: " << lastOut;
    } else {
        lastOut = 0.0;
    }
}

void MotionControl::SetTag(double Tag)
{
    // 关闭发送定时器，填充完队列后再恢复，确保只有1个线程在访问队列
    emit sendTimer.stop();
    // 清空队列
    while (!OutQueue.empty()) {
        OutQueue.pop();
    }
    // 比较Tag和lastOut，进行插值（算法很low，有待改进，改成按照加速度插值）。假设1s转0.35
    const double P1 = 0.08291;
//    const double Vmin = 0.10472;
    const double Vmax = 1.570796;
    const double t1 = -0.05278;
//    const double t2 = 0.05278;
    const double deltaT = CONTROLINTERVAL/1000.0;
    queue<double> deltaQueue;
    while (!deltaQueue.empty()) {
        deltaQueue.pop();
    }
    double tempValue = 0;
    double t = t1;
    while ( (tempValue < P1) && (tempValue < fabs(Tag - lastOut)/2.0) ) { // 加速
        double delta = PI/(2*(exp(-50*t) + 1)) * deltaT;
        tempValue += delta;
        deltaQueue.push(delta);
        t += deltaT;
    }
    while (tempValue < fabs(Tag - lastOut) - P1) { // 匀速
        double delta = Vmax * deltaT;
        tempValue += delta;
        deltaQueue.push(delta);
    }
    t = t1;
    while (tempValue < fabs(Tag - lastOut)) { // 减速
        double delta = PI/(2*(exp(50*t) + 1)) * deltaT;
        tempValue += delta;
        deltaQueue.push(delta);
        t += deltaT;
    }
    deltaQueue.push(fabs(Tag - lastOut)-tempValue);
    if (Tag > lastOut) {
        double tempOut = lastOut;
        while (!deltaQueue.empty()) {
            tempOut += deltaQueue.front();
            OutQueue.push(tempOut);
            deltaQueue.pop();
        }
    } else {
        double tempOut = lastOut;
        while (!deltaQueue.empty()) {
            tempOut -= deltaQueue.front();
            OutQueue.push(tempOut);
            deltaQueue.pop();
        }
    }
    // 恢复发送定时器
    emit sendTimer.start();
}

void MotionControl::sendTimerSlot()
{
//    QTime tempTime = QTime::currentTime();
    if (OutQueue.empty()) {
//        qDebug().noquote() << tempTime.toString("hh:mm:ss.zzz")
//                 << ": "
//                 << "Empty";
        return;
    } else {
//        qDebug().noquote() << tempTime.toString("hh:mm:ss.zzz")
//                 << ": "
//                 << "no Empty";
//        return;
    }
    lastOut = OutQueue.front();
    OutQueue.pop();
//    qDebug().noquote() << tempTime.toString("hh:mm:ss.zzz")
//             << ": "
//             << "Out "
//             << lastOut;
    if (joint != NULL) {
        joint->setTagPos((float)lastOut, JOINT_RADIAN);
    }
}
