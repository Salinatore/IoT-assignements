#include "DroneTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

#define DOOR_CLOSED_ANGLE 0
#define DOOR_OPEN_ANGLE 180
#define D1 1
#define D2 100
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
    this->time = 0;
    this->isTimerActive = false;
    this->justEntered = true;
    this->setState(IN);
    MsgService.init();
}

void DroneTask::tick(){
    switch (state){


    case IN: {
        if (this->checkAndSetJustEntered()){
            this->context->setLanding(false);
            MsgService.sendMsg(FULLYIN);  
            this->lcd->writeStateMessage("DRONE INSIDE");
            Logger.log(F("DroneTask:IN"));
        }


        if (this->isDoorOpen()){
            this->servo->setPosition(DOOR_CLOSED_ANGLE);
        }

        static TakeoffPattern takeoff;
        if (!context->isPreAlarm() && MsgService.isMsgAvailable(takeoff)){
            Msg* msg = MsgService.receiveMsg(takeoff);
            delete msg;
            this->setState(TAKE_OFF);
        }
        if(context->isAlarm()){
            this->setState(ALARM_IN);
        }    
        break;
    }


    case TAKE_OFF: {
        if (this->checkAndSetJustEntered()){
            this->context->setTakeOff(true);
            this->resetTimer();
            this->lcd->writeStateMessage("TAKE OFF");
            Logger.log(F("DroneTask:TAKE-OFF"));
        }


        if (!this->isDoorOpen()){
            this->servo->setPosition(DOOR_OPEN_ANGLE);
        }
        float distance = this->distanceDetector->getDistance();

        if (distance > D1 && !this->isTimerActive){ //distance > D1 al posto di true
            this->startTimer();
        } 
        if (distance < D1){ //distance < D1 al posto di false
            this->resetTimer();
        }


        if (this->isTimerElapsed(T1) || context->isAlarm()){
            this->setState(OUT);
        }
        break;
    }


    case OUT: {
        if (this->checkAndSetJustEntered()){
            this->context->setDroneOut(true);
            MsgService.sendMsg(FULLYOUT);
            this->lcd->writeStateMessage("DRONE OUT");
            Logger.log(F("DroneTask:OUT"));
        }

        if (this->isDoorOpen()){
            this->context->setTakeOff(false);
            this->servo->setPosition(DOOR_CLOSED_ANGLE);
        }


        static ImminentLandingPattern landing;
        if (context->isAlarm()){
            this->setState(ALARM_OUT);
        }
        if (MsgService.isMsgAvailable(landing) && !context->isPreAlarm()){
            Msg* msg = MsgService.receiveMsg(landing);
            delete msg;
            this->setState(WAITING_FOR_LANDING);
        }
        break;
    }


    case ALARM_OUT: {
        if (this->checkAndSetJustEntered()){
            this->lcd->writeStateMessage("DRONE OUT");
            Logger.log(F("DroneTask:ALARM-OUT"));
        }
        if (!context->isAlarm()){
            this->setState(OUT);
        }
        break;
    }


    case WAITING_FOR_LANDING: {
        if (this->checkAndSetJustEntered()){
            this->context->setDroneOut(false);
            this->lcd->writeStateMessage("DRONE OUT");
            Logger.log(F("DroneTask:WAITING_FOR_LANDING"));
        }
        this->presenceDetector->sync();
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
            this->context->setLanding(true);
            this->resetTimer();
            this->lcd->writeStateMessage("LANDING");
            Logger.log(F("DroneTask:LANDING"));     
        }

        if (!this->isDoorOpen()){
            this->servo->setPosition(DOOR_OPEN_ANGLE);
        }

        float distance = this->distanceDetector->getDistance();

        if (distance < D2 && !this->isTimerActive){ //distance < D2 al psoto di true
            this->startTimer();
        } 
        if (distance > D2){ //distance > D2 al posto di false
            this->resetTimer();
        }

        if (this->isTimerElapsed(T2) || context->isAlarm()){
            this->setState(IN);
        }
        break;
    }

    case ALARM_IN: {
        if (this->checkAndSetJustEntered()){
            this->lcd->writeStateMessage("DRONE INSIDE");
            Logger.log(F("DroneTask:ALARM_IN"));
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

void DroneTask::startTimer(){
    this->time = millis();
    this->isTimerActive = true;
}

void DroneTask::resetTimer(){
    this->time = 0;
    this->isTimerActive = false;
}

bool DroneTask::isTimerElapsed(unsigned long t){
    return (millis() - this->time) > t;
}

bool DroneTask::isDoorOpen(){
    return !(this->servo->getAngle() == this->servo->NOT_INITIALIZE || this->servo->getAngle() == DOOR_CLOSED_ANGLE);
}