#include "HWTask.h"
#include "Arduino.h"


HWTask::HWTask(Sonar* pSonar, Led* redLed, Led* greenLed)
{
    this->init(1000);
    this->pSonar = pSonar;
    this->redLed = redLed;
    this->greenLed = greenLed;
    this->setState(IDLE);
    this->msgHandler = new MessageHandlerClass(new ConnectionHandlerClass());
    Serial.println("end set up HWtask");
}

void HWTask::task()
{
    long cicletime = millis();
    //infinte loop
    for(;;){
        //verify if hwtask is active and periodic, in that case call the new tick after the period define in the init
        if(this->isActive()){
            if(this->isPeriodic() && (millis() - cicletime) > this->getPeriod()){
                cicletime = millis();
                this->tick();
            }
            if(!this->isPeriodic()){
                this->tick();
            }
        }
        //delaing the cicle so the core can execute backgound task that accomulate
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
        //verify if the connection is on, else change state
        if(!msgHandler->isConnectionOn()){
            this->setState(UNCONNECTED);
        }
        //read sonar value 
        float distance = this->pSonar->getDistance();
        this->sendMsg(distance);
        
        break;
    }
    case UNCONNECTED:
    {
        if(this->checkAndSetJustEntered()){
            this->redLed->switchOn();
            this->greenLed->switchOff();
        }

        Serial.println("unconected state");
        this->msgHandler->tryReconection();

        //if is able to reconnect return to idle state 
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

//after reading the value send the message using the mesage handler
void HWTask::sendMsg(float value)
{
    char msg[50];
    snprintf(msg, sizeof(msg), MQTT_PREFIX_MSG "%.2f", value);
    this->msgHandler->sendMsg(msg);
    Serial.println(msg);
}
