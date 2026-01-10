#include "HWTask.h"
#include "Arduino.h"


HWTask::HWTask(Sonar* pSonar)
{
    this->pSonar = pSonar;
    this->setState(IDLE);
    this->connection = new ConnectionHandlerClass();
    this->msgHandler = new MessageHandlerClass(this->connection);

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
        if(!msgHandler->isConnectionOn()){
            this->setState(UNCONECTED);
        }

        float distance = this->pSonar->getDistance();
        this->sendMsg(distance);
        break;
    }
    case UNCONECTED:
    {
        if(this->checkAndSetJustEntered()){
            //led rosso
        }
        
        this->msgHandler->tryReconection();

        if(this->msgHandler->isConnectionOn()){
            this->setState(IDLE);
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

void HWTask::sendMsg(float value)
{
    char msg[50];
    snprintf(msg, sizeof(msg), MQTT_PREFIX_MSG "%.2f", value);
    this->msgHandler->sendMsg(msg);
}
