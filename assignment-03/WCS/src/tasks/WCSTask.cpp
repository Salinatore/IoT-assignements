#include "WCSTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

#define PREALARM_MSG "st-a-prealarm"
#define ALARM_MSG "st-a-alarm"
#define NORMAL_MSG "st-a-normal"

WCSTask::WCSTask(ServoMotor* pServo, MyLcd *pLcd, Button *pButton, Potentiometer* pPot) : pServo(pServo), pLcd(pLcd), pButton(pButton), pPot(pPot)
{
    setState(AUTOMATIC);
}

void WCSTask::tick()
{
    switch (state)
    {
    case AUTOMATIC:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("AUTOMATIC");
        }
        
        if(pButton->isPressed()){
            setState(LOC_MANUAL);
        }
        
        break;
    }
    case LOC_MANUAL:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("LOC MANUAL");
        }

        if(pButton->isPressed()){
            setState(AUTOMATIC);
        }
        pPot->sync();
        int potValue = pPot->getValue();
        pServo->setPosition(potValue);
        pLcd->writePercMessage(String(potValue));

        break;
    }
    case REM_MANUAL:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("REMOTE MANUAL");
        }
        break;
    }
    case UNCONECTED:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("UNCONECTED");
        }
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