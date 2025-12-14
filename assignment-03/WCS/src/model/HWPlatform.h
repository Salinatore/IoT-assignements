#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/ServoMotorImpl.h"
#include "devices/Lcd.h"
#include "devices/Pot.h"


class HWPlatform
{

public:
  HWPlatform();
  void test();

  Button *getButton();
  ServoMotorImpl *getMotor();
  Lcd *getLCD();
  Potentiometer *getPot();

private:
  Button *resetButton;
  ServoMotorImpl *servo;
  Lcd *lcd;
  Potentiometer *pot;
};

#endif
