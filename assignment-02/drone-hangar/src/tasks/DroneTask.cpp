#include "DroneTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"


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


DroneTask::DroneTask(Context* context, Pir* presenceDetector, Sonar* distanceDetector, TempSensorTMP36* tempSensor, ServoMotor* servo){
    this->distanceDetector = distanceDetector;
    this->presenceDetector = presenceDetector;
    this->tempSensor = tempSensor;
    this->servo = servo;
    this->context = context;
    this->time = 0;
    this->isTimerActive = false;
    this->isDoorOpen = false;
    this->imminentLanding = false;
    this->state = IN;
    MsgService.init();
}

void DroneTask::tick(){
    switch (state){


    case IN: {
        Logger.log(F("in"));
        if (this->isDoorOpen){
            this->servo->setPosition(0);
            this->isDoorOpen = false;
        }

        TakeoffPattern t;
        if (!context->isPreAlarm() && MsgService.receiveMsg(t)){
            Logger.log(F("TAKE_OFF"));
            state = TAKE_OFF;
            this->context->setTakeOff(true);
        }
        if(context->isAlarm()){
            Logger.log(F("ALARM_IN"));
            state = ALARM_IN;
        }    
        Logger.log(F("2in"));
        break;
    }


    case TAKE_OFF: {
        Logger.log(F("take-off"));
        if (!this->isDoorOpen){
            this->servo->setPosition(180);
            this->isDoorOpen = true;
        } else {
            float distance = this->distanceDetector->getDistance();

            if (distance > D1 && !this->isTimerActive){
                this->time = millis();
                this->isTimerActive = true;
            } 
            if (distance < D1){
                this->time = 0;
            }
        }



        if ((this->time - millis()) > BLINK_PERIOD || context->isAlarm()){
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
            this->servo->setPosition(0);
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
        if (this->presenceDetector->isDetected()){
            state = LANDING;
            this->context->setLanding(true);
        }
        if (context->isAlarm()){
            state = IN;
        }
    }
    case LANDING: {
        if (!this->isDoorOpen){
            this->servo->setPosition(0);
            this->isDoorOpen = true;
        } else {
            float distance = this->distanceDetector->getDistance();

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
}
