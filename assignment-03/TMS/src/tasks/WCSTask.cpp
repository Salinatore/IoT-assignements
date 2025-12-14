#include "WCSTask.h"
#include "Arduino.h"

#define PREALARM_MSG "st-a-prealarm"
#define ALARM_MSG "st-a-alarm"
#define NORMAL_MSG "st-a-normal"

WCSTask::WCSTask(Sonar* pSonar) : pSonar(pSonar)
{
    setState(AUTOMATIC);
}

void WCSTask::tick()
{
    switch (state)
    {
    case AUTOMATIC:
    {
        
    }
    case LOC_MANUAL:
    {
        
        break;
    }
    case REM_MANUAL:
    {
        
        break;
    }
    case UNCONECTED:
    {
        
        break;
    }   
    default:
        break;
    }
}

void WCSTask::setState(WCSState s)
{
    state = s;
    justEntered = true;
}

bool WCSTask::checkAndSetJustEntered()
{
    bool bak = justEntered;
    if (justEntered)
    {
        justEntered = false;
    }
    return bak;
}