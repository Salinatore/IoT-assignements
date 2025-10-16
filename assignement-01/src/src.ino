#include "setup.h"
#include "game.h"
#include "wait.h"
#include "btn_control.h"

void setup() {
  Serial.begin(9600);
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
    Serial.println("waiting");
    waitModeOperations();
  } else {
    if (firstGameRound) {
      firstGameRoundOperations();
    }
    gameModeOperations();
  }
}

void firstGameRoundOperations() {
  analogWrite(LED_RED_PIN, 0);
  score = 0;
  //display: GO!
  detachWaitInterrupt();
  setUpBTNControlInterrupt();
  delay(2000);
}