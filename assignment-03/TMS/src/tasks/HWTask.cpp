#include "HWTask.h"
#include "Arduino.h"


HWTask::HWTask(Sonar* pSonar, Led* redLed, Led* greenLed)
{
    this->init(4000);
    this->pSonar = pSonar;
    this->redLed = redLed;
    this->greenLed = greenLed;
    this->setState(IDLE);
    this->msgHandler = new MessageHandlerClass(new ConnectionHandlerClass());
    Serial.println("end set up");
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
        delay(1);
    }
    
}

void HWTask::tick()
{
    switch(state)
    {
    case IDLE:
    {
        if(this->checkAndSetJustEntered()){
            this->greenLed->switchOn();
            this->redLed->switchOff();
        }
        if(!msgHandler->isConnectionOn()){
            this->setState(UNCONECTED);
        }
        
        float distance = this->pSonar->getDistance();
        this->sendMsg(distance);
        Serial.println(distance);
        break;
    }
    case UNCONECTED:
    {
        if(this->checkAndSetJustEntered()){
            this->redLed->switchOn();
            this->greenLed->switchOff();
        }
        Serial.println("unconected state");
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
