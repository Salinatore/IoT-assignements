#include "WCSTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

#define UNCONECTED_MSG "cus->wcs-st-unconnected"
#define AUTOMATIC_MSG "cus->wcs-st-automatic"
#define REMOTE_MANUAL_MSG "cus->wcs-st-remote-manual"
#define LOCAL_MANUAL_MSG "cus->wcs-st-local-manual"
#define CONTROL_MSG "cus->wcs-op-"

#define LOCAL_MANUAL_SEND_MSG "wcs->cus-st-local-manual"
#define AUTOMATIC_SEND_MSG "wcs->cus-st-automatic"
#define CONTROL_SEND_MSG "wcs->cus-op-"

#define POTENTIOMETER_MIN 0
#define POTENTIOMETER_MAX 1023

#define PERCENTAGE_MIN 0
#define PERCENTAGE_MAX 100

class AutomaticPattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent() == AUTOMATIC_MSG;
  }
};

class UnconectedPattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent() == UNCONECTED_MSG;
  }
};

class RemotePattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent() == REMOTE_MANUAL_MSG;
  }
};

class LocalPattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent() == LOCAL_MANUAL_MSG;
  }
};

class ControlPattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent().startsWith(CONTROL_MSG);
  }
};

static UnconectedPattern unconected;
static RemotePattern remote;
static LocalPattern local;
static AutomaticPattern automatic;
static ControlPattern control;

WCSTask::WCSTask(ServoMotor* pServo, Lcd *pLcd, Button *pButton, Potentiometer* pPot) : 
        pServo(pServo), pLcd(pLcd), pButton(pButton), pPot(pPot)
{
    this->setState(AUTOMATIC);    
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
        
        // Message Handling 
        this->checkUnconnectedMessage();
        this->checkControlMessage();
        this->checkRemoteMessage();
        this->checkLocalMessage();

        // Mode change requests
        if(pButton->isPressed())
        {
            MsgService.sendMsg(LOCAL_MANUAL_SEND_MSG);
        }

        break;
    }
    case LOCAL_MANUAL:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("LOCAL MANUAL");
        }

        // Message Handling 
        this->checkUnconnectedMessage();
        this->checkAutomaticMessage();

        // Mode change requests
        if(pButton->isPressed())
        {
            MsgService.sendMsg(AUTOMATIC_SEND_MSG);  
        } 

        this->processPotentiometerInput();

        break;
    }
    case REMOTE_MANUAL:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("REMOTE MANUAL");
        }

        // Message Handling 
        this->checkUnconnectedMessage();
        this->checkAutomaticMessage();
        this->checkControlMessage();
 
        break;
    }
    case UNCONECTED:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("UNCONECTED");
        }

        // Message Handling 
        this->checkAutomaticMessage();

        break;
    }   
    default:
        break;
    }
}

void WCSTask::checkUnconnectedMessage(){
    if(MsgService.isMsgAvailable(unconected))
    {
        Msg* msg = MsgService.receiveMsg(unconected);
        delete msg;
        this->setState(UNCONECTED);
    }
}

void WCSTask::checkAutomaticMessage(){
    if(MsgService.isMsgAvailable(automatic))
    {
        Msg* msg = MsgService.receiveMsg(automatic);
        delete msg;
        this->setState(AUTOMATIC);
    }
}

void WCSTask::checkControlMessage(){
    if(MsgService.isMsgAvailable(control))
    {
        Msg* msg = MsgService.receiveMsg(control);
        int perc = this->msgMotorPerc(msg->getContent());
        int angle =  map(perc, PERCENTAGE_MIN, PERCENTAGE_MAX, 0, 90);
        this->pServo->setPosition(angle);
        delete msg;
    }
}

void WCSTask::checkRemoteMessage(){
    if(MsgService.isMsgAvailable(remote))
    {
        Msg* msg = MsgService.receiveMsg(remote);
        delete msg;
        this->setState(REMOTE_MANUAL);
    }
}

void WCSTask::checkLocalMessage(){
    if(MsgService.isMsgAvailable(local))
    {
        Msg* msg = MsgService.receiveMsg(local);
        delete msg;
        this->setState(LOCAL_MANUAL);
    }
}

void WCSTask::processPotentiometerInput(){
    pPot->sync();
    int potValue = pPot->getValue();
    int angle = map(potValue, POTENTIOMETER_MIN, POTENTIOMETER_MAX, 0, 90);
    pServo->setPosition(angle);
    pLcd->writePercMessage(String(angle));
    MsgService.sendMsg(CONTROL_SEND_MSG+String(angle));
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

int WCSTask::msgMotorPerc(String m)
{
    return m.substring(String(CONTROL_MSG).length()).toInt();
}