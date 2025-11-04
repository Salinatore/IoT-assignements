#ifndef BTN_CONTOL_H
#define BTN_CONTOL_H

#define NO_BUTTON_PRESSED -1

extern volatile int btnPressed;

void setUpBTNControlInterrupt();

void detachBTNControlInterrupt();

#endif
