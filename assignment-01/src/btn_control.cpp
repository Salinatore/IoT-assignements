#include "btn_control.h"
#include "setup.h"
#include "interrupt.h"
#include <Arduino.h>

// Button constants
#define BUTTON_DEBOUNCE_DELAY_MS   300

// Button identifiers
#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 3
#define BUTTON_4 4

volatile int btnPressed = NO_BUTTON_PRESSED;
unsigned long int lastPress = millis();

void bouncingPrevention(int btnNum) {
  if (millis() - lastPress > BUTTON_DEBOUNCE_DELAY_MS) {
    btnPressed = btnNum;
    lastPress = millis();
  }
}

void controlBTN1() {
  bouncingPrevention(BUTTON_1);
}

void controlBTN2() {
  bouncingPrevention(BUTTON_2);
}

void controlBTN3() {
  bouncingPrevention(BUTTON_3);
}

void controlBTN4() {
  bouncingPrevention(BUTTON_4);
}

void setUpBTNControlInterrupt() {
  internalEnableInterrupt(BUTTON1_PIN, controlBTN1, RISING);
  internalEnableInterrupt(BUTTON2_PIN, controlBTN2, RISING);
  internalEnableInterrupt(BUTTON3_PIN, controlBTN3, RISING);
  internalEnableInterrupt(BUTTON4_PIN, controlBTN4, RISING);
}

void detachBTNControlInterrupt() {
  internalDisableInterrupt(BUTTON1_PIN);
  internalDisableInterrupt(BUTTON2_PIN);
  internalDisableInterrupt(BUTTON3_PIN);
  internalDisableInterrupt(BUTTON4_PIN);
}
