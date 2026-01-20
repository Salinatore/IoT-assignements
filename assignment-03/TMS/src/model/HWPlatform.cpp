#include "HWPlatform.h"
#include <Arduino.h>
#include "config.h"


#define TEST_ANGLE 56


void wakeUp(){}

HWPlatform::HWPlatform(){
  this->wLSensor = new Sonar(ECHO_PIN, TRIG_PIN, SONAR_TIME);
  this->rLed = new Led(LED_PIN1);
  this->gLed = new Led(LED_PIN2);
  
}

Sonar* HWPlatform::getSonar(){
  return this->wLSensor;
}

Led*  HWPlatform::getRedLed(){
  return this->rLed;
}

Led*  HWPlatform::getGreenLed(){
  return this->gLed;
}
