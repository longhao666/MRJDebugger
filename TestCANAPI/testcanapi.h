#ifndef TESTCANAPI_H
#define TESTCANAPI_H

#include "./usercontroloncan.h"

class TestCANAPI
{
public:
  TestCANAPI();
  virtual ~TestCANAPI();
  bool Start();
private:
  UserControlOnCan* pcan;
};

#endif // TESTCANAPI_H
