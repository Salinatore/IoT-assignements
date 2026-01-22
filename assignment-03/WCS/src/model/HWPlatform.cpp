#include "HWPlatform.h"
#include "devices/ButtonImpl.h"
#include "kernel/MsgService.h"
#include "config.h"
#include "devices/ServoMotorImpl.h"
#include "kernel/Logger.h"
#include <Arduino.h>


#define TEST_ANGLE 56


void wakeUp(){}

HWPlatform::HWPlatform(){
  this->resetButton = new ButtonImpl(BTN);
  this->servo = new ServoMotorImpl(MOTOR_PIN);
  this->lcd = new Lcd();
  this->pot = new Potentiometer(POT_PIN);
  
  this->servo->on();
}

Button* HWPlatform::getButton(){
  return this->resetButton;
}


ServoMotorImpl* HWPlatform::getMotor(){
  return this->servo;
}

Lcd* HWPlatform::getLCD(){
  return this->lcd;
}

Potentiometer* HWPlatform::getPot(){
  return this->pot;
}

void HWPlatform::test(){

  if(this->servo->getAngle()  == -1 || this->servo->getAngle() == 1){
    this->servo->setPosition(179);
  } 
   else if (this->servo->getAngle() == 179){
    this->servo->setPosition(1);
  }

  if(this->getButton()->isPressed()){
    Logger.log(F("TEST: button"));
  }

  this->lcd->writeModeMessage("TEST");
  this->lcd->writePercMessage("TEST");
  
}