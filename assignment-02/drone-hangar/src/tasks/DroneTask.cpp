#include "DroneTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"


#define D1 30
#define D2 5
#define T1 5
#define T2 5
#define TAKEOFF "free-your-wings"
#define IMMINENTLANDING "landing"
#define FULLYOUT "st-d-fully-out"
#define FULLYIN "st-d-fully-in"

class TakeoffPattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent() == TAKEOFF;
  }
};

class ImminentLandingPattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent() == IMMINENTLANDING;
  }
};


DroneTask::DroneTask(HWPlatform *pHW, Context* context){
    this->pHW = pHW;
    this->context = context;
    this->time = 0;
    this->isTimerActive = false;
    this->isDoorOpen = false;
    this->imminentLanding = false;
    MsgService.init();
}

void DroneTask::tick(){
    switch (state){


    case IN: {
        if (this->isDoorOpen){
            this->pHW->closeDoor();
            this->isDoorOpen = false;
        }

        TakeoffPattern t;
        if (!context->isPreAlarm() && MsgService.receiveMsg(t)){
            state = TAKE_OFF;
            this->context->setTakeOff(true);
        }
        if(context->isAlarm()){
            state = ALARM_IN;
        }
        break;
    }


    case TAKE_OFF: {
        if (!this->isDoorOpen){
            this->pHW->openDoor();
            this->isDoorOpen = true;
        } else {
            float distance = this->pHW->getDistance();

            if (distance > D1 && !this->isTimerActive){
                this->time = millis();
                this->isTimerActive = true;
            } 
            if (distance < D1){
                this->time = 0;
            }
        }



        if ((this->time - millis()) > T1 || context->isAlarm()){
            //manda messaggio fully out
            MsgService.sendMsg(FULLYOUT);
            this->time = 0;
            state = OUT;
            context->setFullyOut(false);
        }
    }


    case OUT: {
        if (this->isDoorOpen){
            this->context->setTakeOff(false);
            this->pHW->closeDoor();
            this->isDoorOpen = false;
        }


        ImminentLandingPattern l;
        if (context->isAlarm()){
            state = ALARM_OUT;
        }
        if (MsgService.receiveMsg(l) && !context->isPreAlarm()){
        }
    }


    case ALARM_OUT: {
        if (!context->isAlarm()){
            state = OUT;
        }
    }


    case WAITING_FOR_LANDING: {
        if (this->pHW->isDetected()){
            state = LANDING;
            this->context->setLanding(true);
        }
        if (context->isAlarm()){
            state = IN;
        }
    }
    case LANDING: {
        if (!this->isDoorOpen){
            this->pHW->openDoor();
            this->isDoorOpen = true;
        } else {
            float distance = this->pHW->getDistance();

            if (distance < D2 && !this->isTimerActive){
                this->time = millis();
                this->isTimerActive = true;
            } 
            if (distance > D2){
                this->time = 0;
            }
        }



        if ((this->time - millis()) > T2 || context->isAlarm()){
            //manda messaggio fully in
            MsgService.sendMsg(FULLYIN);
            state = IN;
            this->time = 0;
            this->context->setLanding(false);
        }
    }


    case ALARM_IN: {
        if(!context->isAlarm()){
            state = IN;
        }
    }
}
