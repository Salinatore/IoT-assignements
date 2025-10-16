#include "game.h"
#include "setup.h"
#include "wait.h"
#include "btn_control.h"
#include <Arduino.h>

#define LED_RED_PIN 9

#define N_BUTTONS 4

bool gameactivation = true;
int seq[N_BUTTONS];
unsigned long time = 0;
unsigned long t1 = 20000;
int current;
int score;

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
  //display: sequence
  time = millis();  //start the timer
  current = 0;      //restart the current expected btn
  btnPressed = -1;
}

void nextRound() {
  t1 = t1 - 500;
  score++;
  Serial.println("Score: " + String(score) + " WIN");
  //display score
  gameactivation = true;
}

void gameOver() {
  analogWrite(LED_RED_PIN, 255);
  delay(2000);
  Serial.println("Score: " + String(score) + " LOST");
  //print score
  delay(10000);
  waiting = true;
  detachBTNControlInterrupt();
  setUpWaitInterrupt();
}

void manageGame() {
  if (millis() - time > t1) {
    Serial.println("Lost for timeout");
    gameOver();
  } else {
    if (btnPressed != -1) {
      if (btnPressed == seq[current]) {
      current++;
        if (current == N_BUTTONS) {
          nextRound();
        }
      } else {
        Serial.println("Wrong Sequence");
        gameOver();
      }
    }
  }
  btnPressed = -1;
}

void gameModeOperations() {
  if (gameactivation) {
    startRound(seq);
    gameactivation = false;
  }
  manageGame();
}
