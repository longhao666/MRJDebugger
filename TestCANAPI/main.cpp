#include "testcanapi.h"

int main() {
  TestCANAPI test1;
  if (test1.Start()) {
    qDebug("test succeed");
  } else {
    qDebug("test failed");
  }
  return 0;
}
