#include "testcanapi.h"
#include <iostream>
#include <string>
#include <QDebug>

using std::cout;
using std::endl;
using std::vector;
using std::string;

TestCANAPI::TestCANAPI()/**/ :
  pcan(new UserControlOnCan)
{

}

TestCANAPI::~TestCANAPI()
{

}

bool TestCANAPI::Start()
{
  // 测试能否初始化
  if (! pcan->Init("pcan0")) {
      string s;
      pcan->controller.GetErrorText(s);
      qDebug() << s.c_str();
      return false;
  }
  qDebug("Init done");
  // 查看读到多少模块
  qDebug("Joint ID in CANBUS:");
  for (vector<Joint>::iterator iter = pcan->controller.allJoint.begin();
       iter != pcan->controller.allJoint.end();
       ++iter) {
      qDebug(" --- %d", iter._Ptr->ID);
  }
  qDebug("");

  // 查看读到多少手爪
  qDebug("Finger ID in CANBUS:");
  for (vector<Gripper>::iterator iter = pcan->controller.allGripper.begin();
       iter != pcan->controller.allGripper.end();
       ++iter) {
      qDebug(" --- %d", iter._Ptr->ID);
  }
  qDebug("");

  // 设置自动更新位置
  qDebug() << pcan->readJointCurPos(27);
  if (! pcan->setJointAutoUpdateCurPos(27, false)) {
      string s;
      pcan->controller.GetErrorText(s);
      qDebug() << s.c_str();
  }

  // 测试能否关闭总线
  pcan->Close("pcan0");
  return false;
}
