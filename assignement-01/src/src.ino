#include "setup.h"
#include "game.h"
#include "wait.h"

void setup() {
  setUpLeds();
  setUpButtons();
  //setUpLCD();
  setUpWaitInterrupt();
  //randomSeed(analogRead(A0)); // inizializza casualità
}

void loop() {
  noInterrupts();
  bool currentWaiting = waiting;
  interrupts();
  //remember to add deep state behaviour
  if (currentWaiting) {
    waitModeOperations();
  } else {
    gameModeOperations();
  }
}
