#include "game.h"
#include "setup.h"
#include "wait.h"
#include "btn_control.h"
#include "lcd_control.h"
#include <Arduino.h>

#define LED_RED_PIN 9

bool roundActivation = true;
int seq[N_SEQ];
unsigned long time = 0;
unsigned long t1;
int difficulty;
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

int difficultyTimeReduction(int difficulty) {
  switch (difficulty) {
    case 1: return 1000;
    case 2: return 2000;
    case 3: return 3000;
    case 4: return 4000;
  }
}

void turnOffAllGameLed() {
  for (int i = 1; i <= N_SEQ; i++) {
    digitalWrite(getLedPin(i), LOW);
  }
}

void generateSequence(int seq[N_SEQ]) {
  int pool[N_SEQ] = { 1, 2, 3, 4 };
  for (int i = (N_SEQ - 1); i > 0; --i) {
    int j = random(i + 1);
    int tmp = pool[i];
    pool[i] = pool[j];
    pool[j] = tmp;
  }
  for (int i = 0; i < N_SEQ; i++) {
    seq[i] = pool[i];
    Serial.print(seq[i]);
  }
  Serial.println("");
}

void startRound(int seq[N_SEQ]) {
  generateSequence(seq);
  Serial.println();
  writeSequence(seq);
  //display: sequence
  time = millis();  //start the timer
  current = 0;      //restart the current expected btn
  btnPressed = -1;
}

void nextRound() {
  t1 = t1 - difficultyTimeReduction(difficulty);
  score++;
  Serial.println("Score: " + String(score) + " WIN");
  writeWinMessage(score);
  turnOffAllGameLed();
  //display score
  roundActivation = true;
}

void gameOver() {
  analogWrite(LED_RED_PIN, 255);
  turnOffAllGameLed();
  delay(2000);
  Serial.println("Score: " + String(score) + " LOST");
  writeLoseMessage(score);
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
    if (current >= N_SEQ) {
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
