#include "game.h"
#include "setup.h"
#include "wait.h"
#include "btn_control.h"
#include <Arduino.h>

#define LED_RED_PIN 9

#define N_BUTTONS 4

bool roundActivation = true;
int seq[N_BUTTONS];
unsigned long time = 0;
unsigned long t1;
int current;
int score;

int getLedPin(int buttonId) {
  switch (buttonId) {
    case 1: return LED1_PIN;
    case 2: return LED2_PIN;
    case 3: return LED3_PIN;
    case 4: return LED4_PIN;
    default: return -1;  // nessun LED associato
  }
}

void turnOffAllGameLed() {
  for (int i = 1; i <= N_BUTTONS; i++) {
    digitalWrite(getLedPin(i), LOW);
  }
}

void generateSequence(int seq[N_BUTTONS]) {
  int pool[N_BUTTONS] = { 1, 2, 3, 4 };
  for (int i = (N_BUTTONS - 1); i > 0; --i) {
    int j = random(i + 1);
    int tmp = pool[i];
    pool[i] = pool[j];
    pool[j] = tmp;
  }
  for (int i = 0; i < N_BUTTONS; i++) {
    seq[i] = pool[i];
    Serial.print(seq[i]);
  }
  Serial.println("");
}

void startRound(int seq[N_BUTTONS]) {
  generateSequence(seq);
  Serial.println();
  //display: sequence
  time = millis();  //start the timer
  current = 0;      //restart the current expected btn
  btnPressed = -1;
}

void nextRound() {
  t1 = t1 - 1000;
  score++;
  Serial.println("Score: " + String(score) + " WIN");
  turnOffAllGameLed();
  //display score
  roundActivation = true;
}

void gameOver() {
  analogWrite(LED_RED_PIN, 255);
  turnOffAllGameLed();
  delay(2000);
  Serial.println("Score: " + String(score) + " LOST");
  //print score
  delay(10000);
  waiting = true;
  detachBTNControlInterrupt();
  setUpWaitInterrupt();
  roundActivation = true;
}

void manageGame() {
  if (millis() - time > t1) {
    Serial.println("Lost for timeout");
    gameOver();
    return;
  } 

  noInterrupts();
  int currentBtnPressed = btnPressed;
  interrupts();

  if (currentBtnPressed == -1) return;

  if (currentBtnPressed == seq[current]) {
    digitalWrite(getLedPin(seq[current]), HIGH);
    current++;
    Serial.println(String("current") + String(current));
    if (current >= N_BUTTONS) {
      nextRound();
    }
  } else if (currentBtnPressed != seq[current]) {
    Serial.println(String("Wrong Sequence") + String(seq[current]) + String(currentBtnPressed));
    gameOver();
  }

  noInterrupts();
  btnPressed = -1;
  interrupts();
}

void gameModeOperations() {
  if (roundActivation) {
    startRound(seq);
    roundActivation = false;
  }
  manageGame();
}
