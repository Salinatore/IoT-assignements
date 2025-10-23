#include "game.h"
#include "setup.h"
#include "wait.h"
#include "btn_control.h"
#include "lcd_control.h"
#include <Arduino.h>

#define BASE_ROUND_TIME 20000
#define TIME_DISPLAYING_GO 2000
#define TIME_DISPLAYING_RED_LED 2000
#define TIME_DISPLAYING_LOSE_SCORE 10000

bool startingRound = true;
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
  writeSequence(seq);
  
  time = millis();  
  current = 0;      
  btnPressed = -1;
}

void nextRound() {
  t1 = t1 - difficultyTimeReduction(difficulty);
  score++;

  writeWinMessage(score);
  delay(1500);
  turnOffAllGameLed();
  startingRound = true;
}

void gameOver() {
  analogWrite(LED_RED_PIN, 255);
  turnOffAllGameLed();

  delay(TIME_DISPLAYING_RED_LED);
  
  writeLoseMessage(score);
  
  delay(TIME_DISPLAYING_LOSE_SCORE);

  detachBTNControlInterrupt();
  setUpWaitInterrupt();

  waiting = true;
  startingRound = true;
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
    if (current >= N_SEQ) {
      nextRound();
    }
  } else if (currentBtnPressed != seq[current]) {
    gameOver();
  }

  noInterrupts();
  btnPressed = -1;
  interrupts();
}

void gameModeOperations() {
  if (startingRound) {
    startRound(seq);
    startingRound = false;
  }
  manageGame();
}

//first time switching to game mode operations

void difficultySetting () {
  difficulty = map(analogRead(POT_PIN), 0, 1023, 1, 4);
  Serial.println(difficulty);
}

void firstGameRoundOperations() {
  firstGameRound = false;
  score = 0;
  t1 = BASE_ROUND_TIME;

  writeGo();

  analogWrite(LED_RED_PIN, 0);
  difficultySetting();
  
  detachWaitInterrupt();
  setUpBTNControlInterrupt();

  delay(TIME_DISPLAYING_GO); 
}
