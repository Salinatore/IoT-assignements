#ifndef BTN_CONTOL_H
#define BTN_CONTOL_H

extern volatile int btnPressed;

void setUpBTNControlInterrupt();

void detachBTNControlInterrupt();

void controlBTN1();

void controlBTN2();

void controlBTN3();

void controlBTN4();

#endif