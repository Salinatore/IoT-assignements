#include "game.h"
#include "setup.h"
#include "wait.h"
#include <Arduino.h>

#define LED_RED_PIN 9

#define N_BUTTONS 4

bool gameactivation = true;
int seq[N_BUTTONS];    
unsigned long time = 0;
unsigned long t1 = 5000;
int current;
int score;

//btn_pressed

void startRound(int seq[N_BUTTONS]) {
  int pool[N_BUTTONS] = {1, 2, 3, 4};
  for (int i = (N_BUTTONS - 1); i > 0; --i) {
    int j = random(i + 1);
    int tmp = pool[i];
    pool[i] = pool[j];
    pool[j] = tmp;
  }
  for (int i = 0; i < N_BUTTONS; i++) seq[i] = pool[i];

  time = millis(); //start the timer
  current = 0; //restart the current expected btn
}

void nextRound() {
  t1 = t1 - 500;
  score++;
  //display score
  gameactivation = true;
}

void gameOver() {
  analogWrite(LED_RED_PIN, 255);
  delay(2000);
  //print score
  delay(10000);
  waiting = true;
}

void manageGame() {
  if(millis() - time > t1) {
    //game over (tempo scaduto)
    gameOver();
  } else {
    if(btn_pressed == seq[current] && btn_pressed != -1) {
      current++;
      if(current == N_BUTTONS) {
        nextRound();
      }
    } else {
      gameOver();
    }
    btn_pressed = -1;
    // attesa input
  }
}

void gameModeOperations() {
  if(gameactivation) {
    startRound(seq);
    gameactivation = false;
  }
  manageGame();
}


