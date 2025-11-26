#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/Led.h"
#include "devices/LightSensorImpl.h"
#include "devices/ServoMotorImpl.h"
#include "kernel/Logger.h"

void wakeUp(){}

HWPlatform::HWPlatform(){
  this->led1 = new Led(LED_PIN1);
  this->led2 = new Led(LED_PIN2);
  this->ledR = new Led(LED_PINR);
  this->resetButton = new ButtonImpl(RST_BT_PIN);
  this->servo = new ServoMotorImpl(MOTOR_PIN);
  this->lcd = new MyLcd();
  this->presenceDetector = new Pir(PIR_PIN);
  this->distanceDetector = new Sonar(ECHO_PIN, TEMP_PIN, SONAR_TIME);
  this->tempSensor = new TempSensorTMP36(TEMP_PIN);
}


void HWPlatform::init(){
}

