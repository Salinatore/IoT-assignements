#include "HWPlatform.h"
#include <Arduino.h>
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/ServoMotorImpl.h"
#include "kernel/Logger.h"

#define TEST_ANGLE 56
#define TEST_MIN_TEMP 0.0
#define TEST_MAX_TEMP 40
#define TEST_MAX_DISTANCE 331.5 + 0.6 * SONAR_TIME
#define TEST_MIN_DISTANCE -1

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

Button* HWPlatform::getButton(){
  return this->resetButton;
}

Led*  HWPlatform::getLed1(){
  return this->led1;
}

Led*  HWPlatform::getLed2(){
  return this->led2;
}

Led*  HWPlatform::getLedR(){
  return this->ledR;
}

ServoMotor* HWPlatform::getMotor(){
  return this->servo;
}

MyLcd* HWPlatform::getLCD(){
  return this->lcd;
}

Pir* HWPlatform::getDPD(){
  return this->presenceDetector;
}

Sonar* HWPlatform::getDDD(){
  return this->distanceDetector;
}

TempSensorTMP36* HWPlatform::getTempSensor(){
  return this->tempSensor;
}

void HWPlatform::test(){
  this->led1->switchOn();
  this->led2->switchOn();
  this->ledR->switchOn();
  this->servo->setPosition(TEST_ANGLE);
  this->lcd->writeMessage("TEST");
  if(this->presenceDetector->isDetected()){
    Logger.log(F("TEST: precence detector = detect motion"));
  }
  if(this->distanceDetector->getDistance() > TEST_MAX_DISTANCE && this->distanceDetector->getDistance() < TEST_MIN_DISTANCE){
    Logger.log(F("TEST: distance detector = detect something"));
  }
  if(this->tempSensor->getTemperature() >  TEST_MIN_TEMP && this->tempSensor->getTemperature() < TEST_MAX_TEMP ){
    Logger.log(F("TEST: temperature sensor = acceptable temperature detection"));
  }

}