#include "wait.h"
#include "setup.h"
#include "interrupt.h"
#include "lcd_control.h"
#include <Arduino.h>
#include <avr/sleep.h>

#define MS_UNTIL_SLEEP 1000

volatile bool firstGameRound = false;
volatile bool waiting = true; 
bool firstTime = false;
int currentValue = 0;
int direction = 1;
unsigned int timeSinceStart = 0;


void waitModeOperations() {
  checkTimeToSleep();
  fadingLed();
  if (!firstTime){
    timeSinceStart = millis();
    firstTime = true;
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
    firstTime = false;
    waiting = false;
  }
}

void detachWaitInterrupt() {
  internalDisableInterrupt(BUTTON1_PIN);
}

void wakeUpNow() {
  timeSinceStart = millis();
}

void checkTimeToSleep() {
  if (millis() - timeSinceStart > MS_UNTIL_SLEEP) {
    Serial.println("Going to sleep...");
    Serial.flush();

    detachWaitInterrupt();
    internalEnableInterrupt(BUTTON1_PIN, wakeUpNow, RISING);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
    sleep_enable(); 
    sleep_mode();

    sleep_disable(); 

    detachWaitInterrupt();
    setUpWaitInterrupt();

    Serial.println("Wake up!");
  }
}


