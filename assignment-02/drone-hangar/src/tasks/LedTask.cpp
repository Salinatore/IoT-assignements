#include "tasks/LedTask.h"
#include <Arduino.h>
#include "config.h"
#include "kernel/Logger.h"

#define BLINK_PERIOD 500

LedTask::LedTask(Context* pContext, Led* pLed1, Led* pLed2, Led* pLedR): 
    pContext(pContext), pLed1(pLed1), pLed2(pLed2), pLedR(pLedR) {
    setState(IDLE);
}

void LedTask::tick(){
    switch (state){
    case IDLE:
        if (this->checkAndSetJustEntered()){
            Logger.log(F("LedTask:IDLE"));
            this->pLed1->switchOn();
            this->pLed2->switchOff();
            this->pLedR->switchOff();
        }
        if (this->pContext->isAlarm()){
            this->setState(ALARM);
        }
        if (this->pContext->isLanding() || this->pContext->isTakeOff()){
            this->setState(MOVE_FASE);
        }
        break;
    case MOVE_FASE:
        if (this->checkAndSetJustEntered()){
            Logger.log(F("LedTask:MOVE_FASE"));
            this->pLedR->switchOff();
            this->timestamp = millis();
        }
        long currentTime = millis();
        if (currentTime - this->elapsedTime() > BLINK_PERIOD){
            if (isOn){
                this->pLed2->switchOff();    
            } else {
                this->pLed2->switchOn();
            }
            this->timestamp = currentTime;
        }
        if (!(this->pContext->isLanding() || this->pContext->isTakeOff())){
            this->setState(IDLE);
        }
        break;
    case ALARM:
        if (this->checkAndSetJustEntered()){
            Logger.log(F("LedTask:ALARM"));
            this->pLed2->switchOff();
            this->pLedR->switchOn();
        }
        if (!this->pContext->isAlarm()){
            this->setState(IDLE);
        }
        break;
    default:
        break;
    }
}

void LedTask::setState(State s){
    state = s;
    justEntered = true;
}

long LedTask::elapsedTime(){
    return millis() - timestamp;
}

bool LedTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}