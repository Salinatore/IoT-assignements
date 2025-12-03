#include "AlarmTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

#define TEMP1 23.0
#define T3 5000
#define TEMP2 26.0
#define T4 5000

#define PREALARM_MSG "st-a-prealarm"
#define ALARM_MSG "st-a-alarm"
#define NORMAL_MSG "st-a-normal"

AlarmTask::AlarmTask(Context* pContext, TempSensorTMP36* pTempSensor, MyLcd* pLcd, Button* pButton): 
    pContext(pContext), pTempSensor(pTempSensor), pLcd(pLcd), pButton(pButton){
    setState(IDLE);
    this->pContext->setAlarm(false);
    this->pContext->setPreAlarm(false);
}

void AlarmTask::tick(){
    switch (state){
    case IDLE: { 
        if (this->checkAndSetJustEntered()){
            this->tempUp = false;
            this->pContext->setPreAlarm(false);
            this->pContext->setAlarm(false);
            this->pLcd->writeAlarmMessage(" ");
            Logger.log(F("AlarmTask:IDLE")); 
            MsgService.sendMsg(NORMAL_MSG);       
        }
        if (!this->pContext->isDroneOut()){
            if (this->pTempSensor->getTemperature() > TEMP1){
                if(this->tempUp){
                    if(this->elapsedTime() > T3){
                        setState(PRE_ALARM);
                    }
                } else {
                    this->timestamp = millis();
                    this->tempUp = true;
                }                    
            } else {
                this->tempUp = false;
            }
        }
        break; 
    }
    case PRE_ALARM: {
        if (this->checkAndSetJustEntered()){
            this->pContext->setPreAlarm(true);
            this->pContext->setAlarm(false);
            Logger.log(F("AlarmTask:PRE_ALARM"));
            this->timestamp = millis();
            MsgService.sendMsg(PREALARM_MSG);   
        }
        if (this->pTempSensor->getTemperature() > TEMP2){
            if(this->elapsedTime() > T4){
                setState(ALARM);
            }                  
        } else {
            this->pContext->setPreAlarm(false);
            this->setState(IDLE);
        }
        break; }
    case ALARM: {
        if (this->checkAndSetJustEntered()){
            this->pContext->setPreAlarm(false);
            this->pContext->setAlarm(true);
            Logger.log(F("AlarmTask:ALARM"));
            this->pLcd->writeAlarmMessage("ALARM");
            MsgService.sendMsg(ALARM_MSG);   
        }
        if(this->pButton->isPressed()){
            this->pContext->setAlarm(false);
            this->setState(IDLE);
        }
        break; }
    default:
        break;
    }
}

void AlarmTask::setState(AlarmState s){
    state = s;
    justEntered = true;
}

long AlarmTask::elapsedTime(){
    return millis() - timestamp;
}

bool AlarmTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}