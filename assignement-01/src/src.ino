#include "setup.h"
#include "game.h"
#include "wait.h"
#include "btn_control.h"
#include "lcd_control.h"

bool currentWaiting = true;

void setup() {
  setUpLeds();
  setUpButtons();
  setUpLCD();
  setUpWaitInterrupt();
  setUpPotentiometer();
  //randomSeed(analogRead(A0)); // inizializza casualità

  Serial.begin(9600); //debug
}

void loop() {
  noInterrupts();
  currentWaiting = waiting;
  interrupts();
  
  if (currentWaiting) {
    waitModeOperations();
  } else {
    if (firstGameRound) {
      firstGameRoundOperations();
    }
    gameModeOperations();
  }
}

