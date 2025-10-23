#ifndef WAIT_H
#define WAIT_H

extern volatile bool firstGameRound;
extern volatile bool waiting;
extern volatile bool firstWait;

void waitModeOperations();

void setUpWaitInterrupt();

void detachWaitInterrupt();

void firstWaitOperations();

#endif
