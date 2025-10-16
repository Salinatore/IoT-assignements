#include "setup.h"
#include "game.h"
#include "wait.h"
#include "btn_control.h"

bool currentWaiting = true;

void setup() {
  Serial.begin(9600);
  setUpLeds();
  setUpButtons();
  //setUpLCD();
  setUpWaitInterrupt();
  //randomSeed(analogRead(A0)); // inizializza casualità
}

void loop() {
  //remember to add deep state behaviour
  noInterrupts();
  currentWaiting = waiting;
  interrupts();

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
  firstGameRound = false;
  analogWrite(LED_RED_PIN, 0);
  score = 0;
  //display: GO!
  Serial.println("GO!!");
  detachWaitInterrupt();
  setUpBTNControlInterrupt();
  delay(2000);
}