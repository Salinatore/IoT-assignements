#include "Context.h"

Context::Context(){
    preAlarm = false;
    alarm = false;
    fullyOut = false;
    takeOff = false;
}

bool Context::isPreAlarm(){
    return preAlarm;
}

bool Context::isTakeOff(){
    return takeOff;
}

bool Context::isDroneFullyOut(){
    return fullyOut;
}

bool Context::isAlarm(){
    return alarm;
}

bool Context::isFullyOut(){
    return this->fullyOut;
}

void Context::setFullyOut(bool out){
    this->fullyOut = out;
}

void Context::setLanding(bool landing){
    this->landing = landing;
}

void Context::setTakeOff(bool takeoff){
    this->takeOff = takeOff;
}
