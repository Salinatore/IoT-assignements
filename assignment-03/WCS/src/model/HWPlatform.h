#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/ServoMotorImpl.h"
#include "devices/MyLcd.h"
#include "devices/Pot.h"


class HWPlatform
{

public:
  HWPlatform();
  void test();

  Button *getButton();
  ServoMotorImpl *getMotor();
  MyLcd *getLCD();
  Potentiometer *getPot();

private:
  Button *resetButton;
  ServoMotorImpl *servo;
  MyLcd *lcd;
  Potentiometer *pot;
};

#endif
