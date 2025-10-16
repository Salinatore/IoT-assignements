#include "btn_control.h"
#include "setup.h"
#include "interrupt.h"
#include <Arduino.h>

volatile int btnPressed = -1;

unsigned long int lastPress = millis();

void bouncingPrevention(int btnNum) {
  Serial.println("PRESSED " + btnNum);
  if (millis() - lastPress > 300) {
    Serial.println("succeded");
    btnPressed = btnNum;
    lastPress = millis();
  }
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

void controlBTN1() {
  bouncingPrevention(1);
}

void controlBTN2() {
  bouncingPrevention(2);
}

void controlBTN3() {
  bouncingPrevention(3);
}

void controlBTN4() {
  bouncingPrevention(4);
}
