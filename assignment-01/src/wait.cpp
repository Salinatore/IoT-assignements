#include "wait.h"
#include "setup.h"
#include "interrupt.h"
#include "lcd_control.h"
#include <Arduino.h>
#include <avr/sleep.h>

// Timing constants
#define MS_UNTIL_SLEEP 10000
#define FADING_DELAY_MS 20

// LED fading constants
#define MAX_LIGHT_VALUE 200
#define MIN_LIGHT_VALUE 0
#define BASE_DIRACTION 1

volatile bool firstGameRound = false;
volatile bool waiting = true; 
volatile bool firstWait = true;
int ledValue = 0;
int direction = BASE_DIRACTION;
unsigned long timeSinceStart = 0;

void fadingLed() {
  analogWrite(LED_RED_PIN, ledValue);
  ledValue = ledValue + direction;
  if (ledValue == MIN_LIGHT_VALUE || ledValue == MAX_LIGHT_VALUE) {
    direction = -direction;
  }
}

void displayWelcome() {
  writeWelcomeMessage();
}

void checkStartButton() { 
  //no need to check for bouncing since multiple presses don't have any affect  
  if (waiting) {
    firstGameRound = true;
    firstWait = true;
    waiting = false;
  }
}

void setUpWaitInterrupt() {
  internalEnableInterrupt(BUTTON1_PIN, checkStartButton, RISING);
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
    sleepLCD();
    sleep_mode();

    sleep_disable(); 
    wakeUpLCD();

    detachWaitInterrupt();
    setUpWaitInterrupt();

    Serial.println("Wake up!");
  }
}

void firstWaitOperations() {
  ledValue = 0;
  direction = BASE_DIRACTION;
  timeSinceStart = millis();
  firstWait = false;
  displayWelcome(); 
}

void waitModeOperations() {
  checkTimeToSleep();
  fadingLed();
  delay(FADING_DELAY_MS);
}
