#ifndef __HW_PLATFORM__
#define __HW_PLATFORM__

#include "config.h"
#include "devices/Button.h"
#include "devices/Led.h"
#include "devices/LedExt.h"
#include "devices/ServoMotor.h"
#include "devices/TempSensorTMP36.h"
#include "devices/MyLcd.h"
#include <devices/Pir.h>
#include <devices/Sonar.h>


class HWPlatform {

public:
  HWPlatform();
  void test();


  Button* getButton();
  Led*  getLed1();
  Led*  getLed2();
  Led*  getLedR();
  ServoMotor* getMotor();
  MyLcd* getLCD();
  Pir* getDPD();
  Sonar* getDDD();
  TempSensorTMP36* getTempSensor();

private:
  Button* resetButton;
  Led* led1;
  Led* led2;
  Led* ledR;
  ServoMotor* servo;
  MyLcd* lcd;
  Pir* presenceDetector;
  Sonar* distanceDetector;
  TempSensorTMP36* tempSensor;
};

#endif
