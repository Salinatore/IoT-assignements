#include "DroneTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

#define NOT_INITIALIZE -1
#define D1 1
#define D2 10000000
#define T1 5000
#define T2 5000
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


DroneTask::DroneTask(Context* context, Pir* presenceDetector, Sonar* distanceDetector, ServoMotorImpl* servo, MyLcd* lcd){
    this->distanceDetector = distanceDetector;
    this->presenceDetector = presenceDetector;
    this->lcd = lcd;
    this->servo = servo;
    this->context = context;
    this->isTimerActive = false;
    this->imminentLanding = false;
    this->setState(IN);
    MsgService.init();
}

void DroneTask::tick(){
    switch (state){


    case IN: {
        if (this->checkAndSetJustEntered()){
            MsgService.sendMsg(FULLYIN);            //manda messaggio fully in
            Logger.log(F("DroneTask:IN"));
            this->lcd->writeStateMessage("DRONE INSIDE");
        }


        if (this->isDoorOpen()){
            this->servo->setPosition(0);
        }

        TakeoffPattern takeoff;
        if (!context->isPreAlarm() && MsgService.isMsgAvailable(takeoff)){
            MsgService.receiveMsg(takeoff);
            this->setState(TAKE_OFF);
            this->context->setTakeOff(true);
        }
        if(context->isAlarm()){
            this->setState(ALARM_IN);
        }    
        break;
    }


    case TAKE_OFF: {
        if (this->checkAndSetJustEntered()){
            this->time = 0;
            this->isTimerActive = false;
            Logger.log(F("DroneTask:TAKE-OFF"));
             this->lcd->writeStateMessage("TAKE OFF");
        }


        if (!this->isDoorOpen()){
            this->servo->setPosition(180);
        }
        float distance = this->distanceDetector->getDistance();

        if (distance > D1 && !this->isTimerActive){ //distance > D1 al posto di true
            this->time = millis();
            this->isTimerActive = true;
        } 
        if (distance < D1){ //distance < D1 al posto di false
            this->time = 0;
            this->isTimerActive = false;
        }


        if ((millis() - this->time) > T1 || context->isAlarm()){
            //manda messaggio fully out
            MsgService.sendMsg(FULLYOUT);
            this->setState(OUT);
            context->setDroneOut(true);
        }
        break;
    }


    case OUT: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("DroneTask:OUT"));
            this->lcd->writeStateMessage("DRONE OUT");
        }

        if (this->isDoorOpen()){
            this->context->setTakeOff(false);
            this->servo->setPosition(0);
        }


        ImminentLandingPattern landing;
        if (context->isAlarm()){
            this->setState(ALARM_OUT);
        }
        if (MsgService.isMsgAvailable(landing) && !context->isPreAlarm()){
            MsgService.receiveMsg(landing);
            this->context->setDroneOut(false);
            this->setState(WAITING_FOR_LANDING);
        }
        break;
    }


    case ALARM_OUT: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("DroneTask:ALARM-OUT"));
            this->lcd->writeStateMessage("DRONE OUT");
        }
        if (!context->isAlarm()){
            this->setState(OUT);
        }
        break;
    }


    case WAITING_FOR_LANDING: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("DroneTask:WAITING_FOR_LANDING"));
            this->lcd->writeStateMessage("DRONE OUT");
        }

        if (this->presenceDetector->isDetected()){ //this->presenceDetector->isDetected() al posto di true
            this->setState(LANDING);
            this->context->setLanding(true);
        }
        if (context->isAlarm()){
            this->setState(IN);
        }
        break;
    }

    case LANDING: {
        if (this->checkAndSetJustEntered()){
            this->time = 0;
            this->isTimerActive = false;
            Logger.log(F("DroneTask:LANDING"));
            this->lcd->writeStateMessage("LANDING");
        }

        if (!this->isDoorOpen()){
            this->servo->setPosition(180);
        }

        float distance = this->distanceDetector->getDistance();

        if (distance < D2 && !this->isTimerActive){ //distance < D2 al psoto di true
            this->time = millis();
            this->isTimerActive = true;
        } 
        if (distance > D2){ //distance > D2 al posto di false
            this->time = 0;
            this->isTimerActive = false;
        }

        if ((millis() - this->time) > T2 || context->isAlarm()){
            this->setState(IN);
            this->context->setLanding(false);
        }
        break;
    }

    case ALARM_IN: {
        if (this->checkAndSetJustEntered()){
            Logger.log(F("DroneTask:ALARM_IN"));
            this->lcd->writeStateMessage("DRONE INSIDE");
        }

        if(!context->isAlarm()){
            this->setState(IN);
        }
        break;
    }
    }
}

void DroneTask::setState(State s){
    state = s;
    justEntered = true;
}

bool DroneTask::checkAndSetJustEntered(){
    bool bak = justEntered;
    if (justEntered){
      justEntered = false;
    }
    return bak;
}

bool DroneTask::isDoorOpen(){
    return !(this->servo->getAngle() == NOT_INITIALIZE || this->servo->getAngle() == 0);
}
