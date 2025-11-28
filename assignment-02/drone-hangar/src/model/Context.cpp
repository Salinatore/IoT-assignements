#include "Context.h"

Context::Context(){
    preAlarm = false;
    alarm = false;
    droneOut = false;
    takeOff = false;
}

bool Context::isPreAlarm(){
    return preAlarm;
}

bool Context::isTakeOff(){
    return takeOff;
}

bool Context::isAlarm(){
    return alarm;
}

bool Context::isDroneOut(){
    return this->droneOut;
}

void Context::setDroneOut(bool out){
    this->droneOut = out;
}

void Context::setLanding(bool landing){
    this->landing = landing;
}

void Context::setTakeOff(bool takeoff){
    this->takeOff = takeOff;
}
