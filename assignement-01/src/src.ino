#include "setup.h"
#include "game.h"
#include "wait.h"
#include "btn_control.h"
#include "lcd_control.h"

bool currentWaiting = true;
bool currentfirstWait = false;
bool currentfirstGameRound = false;

void setup() {
  setUpLeds();
  setUpButtons();
  setUpLCD();
  setUpWaitInterrupt();
  setUpPotentiometer();
  //randomSeed(millis()); // inizializza casualità

  Serial.begin(9600); //debug
}

void loop() {
  noInterrupts();
  currentWaiting = waiting;
  currentfirstWait = firstWait;
  currentfirstGameRound = firstGameRound;
  interrupts();
  
  if (currentWaiting) {
    if (currentfirstWait) {
      firstWaitOperations();
    }
    waitModeOperations();
  } else {
    if (currentfirstGameRound) {
      firstGameRoundOperations();
    }
    gameModeOperations();
  }
}

