#ifndef WAIT_H
#define WAIT_H

extern volatile bool firstGameRound;
extern volatile bool waiting;

void waitModeOperations();

void setUpWaitInterrupt();

//internal procedure

void fadingLed();

void dislpayWelcome();

void checkStartButton();

#endif
