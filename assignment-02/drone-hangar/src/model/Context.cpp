#include "Context.h"

Context::Context(){
    // preAlarm = false;
    // alarm = false;
    // fullyOut = false;
    // imminentLanding = false;
    // droneDetected = false;
    // fullyIn = false;
    // takeOff = false;
    // droneIn = true;
    // droneOut = false;
    // waitingForLanding = false;
}

bool Context::isPreAlarm(){
    return preAlarm;
}

bool Context::isTakeOff(){
    return takeOff;
}

bool Context::isDroneFullyOut(){
    return droneFullyOut;
}

bool Context::isAlarm(){
    return alarm;
}

bool Context::isImminentLanding(){
    return imminentLanding;
}

bool Context::isDroneDetected()
{
    return droneDetected;
}

bool Context::isFullyIn()
{
    return false;
}
