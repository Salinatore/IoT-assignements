#include "DroneTask.h"


DroneTask::DroneTask(HWPlatform *pHW, Context* context){
    this->pHW = pHW;
    this->context = context;
}

void DroneTask::tick(){
    switch (state){
    case IN: {
        if (!context->isPreAlarm() && context->isTakeOff()){
            state = TAKE_OFF;
        }
        if(context->isAlarm()){
            state = ALARM_IN;
        }
        break;
    }
    case TAKE_OFF: {
        if (context->isDroneFullyOut() || context->isAlarm()){
            state = OUT;
        }
    }
    case OUT: {
        if (context->isAlarm()){
            state = ALARM_OUT;
        }
        if (context->isImminentLanding() && !context->isPreAlarm()){
            state = WAITING_FOR_LANDING;
        }
    }
    case ALARM_OUT: {
        if (!context->isAlarm()){
            state = OUT;
        }
    }
    case WAITING_FOR_LANDING: {
        if (context->isDroneDetected()){
            state = LANDING;
        }
        if (context->isAlarm()){
            state = IN;
        }
    }
    case LANDING: {
        if (context->isFullyIn() || context->isAlarm()){
            state = IN;
        }
    }
    case ALARM_IN: {
        if(!context->isAlarm()){
            state = IN;
        }
    }
}
