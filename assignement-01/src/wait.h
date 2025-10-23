#ifndef WAIT_H
#define WAIT_H

extern volatile bool firstGameRound;
extern volatile bool waiting;

void waitModeOperations();

void setUpWaitInterrupt();

void detachWaitInterrupt();

#endif
