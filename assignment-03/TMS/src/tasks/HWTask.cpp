#include "HWTask.h"
#include "Arduino.h"

#define PREALARM_MSG "st-a-prealarm"
#define ALARM_MSG "st-a-alarm"
#define NORMAL_MSG "st-a-normal"


HWTask::HWTask(Sonar* pSonar) : pSonar(pSonar)
{
    this->setState(IDLE);
}

void HWTask::task(void * pvParameters)
{
    long cicletime = millis();
    for(;;){
        if(this->isActive()){
            if(this->isPeriodic() && (millis() - cicletime) > this->getPeriod()){
                cicletime = millis();
                this->tick();
            }
            if(!this->isPeriodic()){
                this->tick();
            }
        }
        
    }
    
}

void HWTask::tick()
{
    switch(state)
    {
    case IDLE:
    {
        if(this->checkAndSetJustEntered()){
            //ledverde
        }
        this->pSonar->getDistance();
        break;
    }
    case UNCONECTED:
    {
        if(this->checkAndSetJustEntered()){
            //led rosso
        }
        break;
    }
    }
    
}

void HWTask::setState(WCSState s)
{
    state = s;
    justEntered = true;
}

bool HWTask::checkAndSetJustEntered()
{
    bool bak = justEntered;
    if (justEntered)
    {
        justEntered = false;
    }
    return bak;
}
