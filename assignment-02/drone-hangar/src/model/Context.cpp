#include "Context.h"

Context::Context(){
    this->preAlarm = false;
    this->alarm = false;
    this->droneOut = false;
    this->takeOff = false;
    this->landing = false;
}

bool Context::isPreAlarm(){
    return this->preAlarm;
}

bool Context::isTakeOff(){
    return this->takeOff;
}

bool Context::isAlarm(){
    return this->alarm;
}

bool Context::isDroneOut(){
    return this->droneOut;
}

bool Context::isLanding(){
    return this->landing;
}

void Context::setDroneOut(bool out){
    this->droneOut = out;
}

void Context::setLanding(bool landing){
    this->landing = landing;
}

void Context::setTakeOff(bool takeoff){
    this->takeOff = takeoff;
}

void Context::setAlarm(bool alarm){
    this->alarm = alarm;
}

void Context::setPreAlarm(bool preAlarm){
    this->preAlarm = preAlarm;
}
