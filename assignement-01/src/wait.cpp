#include "wait.h"
#include "setup.h"
#include "interrupt.h"
#include "lcd_control.h"
#include <Arduino.h>

volatile bool firstGameRound = false;
volatile bool waiting = true; 
bool displayed = false;
int currentValue = 0;
int direction = 1;


void waitModeOperations(){
  fadingLed();
  if (!displayed){
    displayed = true;
    dislpayWelcome(); 
  }
}

void fadingLed() {
  analogWrite(LED_RED_PIN, currentValue);
  currentValue = currentValue + direction;
  if (currentValue == 0 || currentValue == 255) {
    direction = -direction;
  }
}

void dislpayWelcome() {
  //display: Welcome to TOS! Press B1 to Start
  writeWelcomeMessage();
}

void setUpWaitInterrupt() {
  internalEnableInterrupt(BUTTON1_PIN, checkStartButton, RISING);
}

void checkStartButton() { //check for bouncing 
  if (waiting) {
    firstGameRound = true;
    displayed = false;
    waiting = false;
  }
}

void detachWaitInterrupt() {
  internalDisableInterrupt(BUTTON1_PIN);
}
