#include "game.h"
#include "setup.h"
#include "wait.h"
#include "btn_control.h"
#include "lcd_control.h"
#include <Arduino.h>

// Game timing constants
#define BASE_ROUND_TIME_MS 20000
#define TIME_DISPLAYING_GO_MS 2000
#define TIME_DISPLAYING_RED_LED_MS 2000
#define TIME_DISPLAYING_LOSE_SCORE_MS 10000
#define WIN_MESSAGE_DELAY_MS 1500

// Game state constants
#define INVALID_LED_PIN -1

// Difficulty settings
#define MIN_DIFFICULTY              1
#define MAX_DIFFICULTY              4
#define POTENTIOMETER_MIN           0
#define POTENTIOMETER_MAX           1023

// LED constants
#define LED_RED_FULL_INTENSITY      255
#define LED_RED_OFF                 0

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
    default: return INVALID_LED_PIN; 
  }
}

int difficultyTimeReduction(int difficulty) {
  switch (difficulty) {
    case 1: return 1000; //defalt case is implicitly this one
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
  btnPressed = NO_BUTTON_PRESSED;
}

void nextRound() {
  t1 = t1 - difficultyTimeReduction(difficulty);
  score++;

  writeWinMessage(score);
  delay(WIN_MESSAGE_DELAY_MS);
  turnOffAllGameLed();
  startingRound = true;
}

void gameOver() {
  analogWrite(LED_RED_PIN, LED_RED_FULL_INTENSITY);
  turnOffAllGameLed();

  delay(TIME_DISPLAYING_RED_LED_MS);
  
  writeLoseMessage(score);
  
  delay(TIME_DISPLAYING_LOSE_SCORE_MS);

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

  if (currentBtnPressed == NO_BUTTON_PRESSED) return;

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
  btnPressed = NO_BUTTON_PRESSED;
  interrupts();
}

void gameModeOperations() {
  if (startingRound) {
    startRound(seq);
    startingRound = false;
  }
  manageGame();
}

void difficultySetting () {
  difficulty = map(
    analogRead(POT_PIN), 
    POTENTIOMETER_MIN, 
    POTENTIOMETER_MAX, 
    MIN_DIFFICULTY, 
    MAX_DIFFICULTY
  );
  Serial.println(difficulty);
}

void firstGameRoundOperations() {
  firstGameRound = false;
  score = 0;
  t1 = BASE_ROUND_TIME_MS;

  writeGo();

  analogWrite(LED_RED_PIN, LED_RED_OFF);
  difficultySetting();
  
  detachWaitInterrupt();
  setUpBTNControlInterrupt();

  delay(TIME_DISPLAYING_GO_MS); 
}
