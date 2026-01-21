#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Sonar.h"
#include "devices/Led.h"


class HWPlatform
{

public:
  HWPlatform();

  Sonar *getSonar();
  Led *getRedLed();
  Led *getGreenLed();

private:
  Sonar *wLSensor;
  Led *rLed;
  Led *gLed;
};

#endif
