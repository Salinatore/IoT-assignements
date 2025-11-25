#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/Led.h"
#include "devices/Pir.h"
#include "devices/Sonar.h"
#include "devices/ServoMotor.h"
#include "devices/TempSensorTMP36.h"

class HWPlatform {

public:
  HWPlatform();
  void init();
  void test();

  Button* getButton();
  Led*  getLed();
  ServoMotor* getMotor();

private:
  Button* resetButton;
  Led* led1;
  Led* led2;
  Led* ledR;
  ServoMotor* servo;
  //create a class to manage LCD
  Pir* presenceDetector;
  Sonar* distanceDetector;
  TempSensorTMP36* tempSensor;
  
};

#endif
