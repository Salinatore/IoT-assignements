#include "WCSTask.h"
#include "Arduino.h"
#include "kernel/MsgService.h"
#include "kernel/Logger.h"

#define UNCONECTED_MSG "wcs-unconected"
#define AUTOMATIC_MSG "wcs-automatic"
#define REMOTE_MANUAL_MSG "wcs-remote"
#define LOCAL_MANUAL_MSG "wcs-local"
#define CONTROL_MSG "wcs-perc:"
#define POTENTIOMETER_MIN 0
#define POTENTIOMETER_MAX 1023


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

class ControlPattern : public Pattern {
public:
  boolean match(const Msg& m) override {
    return m.getContent().startsWith(CONTROL_MSG);
  }
};

static UnconectedPattern unconected;
static RemotePattern remote;
static AutomaticPattern automatic;
static ControlPattern control;

WCSTask::WCSTask(ServoMotor* pServo, Lcd *pLcd, Button *pButton, Potentiometer* pPot) : pServo(pServo), pLcd(pLcd), pButton(pButton), pPot(pPot)
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
        
        if(pButton->isPressed()){
            this->setState(LOC_MANUAL);
        }
        if(MsgService.isMsgAvailable(unconected)){
            Msg* msg = MsgService.receiveMsg(unconected);
            delete msg;
            this->setState(UNCONECTED);
        }
        if(MsgService.isMsgAvailable(remote)){
            Msg* msg = MsgService.receiveMsg(remote);
            delete msg;
            this->setState(REM_MANUAL);
        }
        if(MsgService.isMsgAvailable(control)){
            Msg* msg = MsgService.receiveMsg(control);
            int perc = this->msgMotorPerc(msg->getContent());
            this->pServo->setPosition(perc);
            delete msg;
            this->setState(REM_MANUAL);
        }
        break;
    }
    case LOC_MANUAL:
    {
        if (checkAndSetJustEntered())
        {
            Logger.log(F("WCS:LOCAL_START"));
            pLcd->writeModeMessage("LOC MANUAL");
        }

        if(pButton->isPressed()){
            setState(AUTOMATIC);
            Logger.log(F("WCS:LOCAL_END"));
        }

        if(MsgService.isMsgAvailable(unconected)){
            Msg* msg = MsgService.receiveMsg(unconected);
            delete msg;
            this->setState(UNCONECTED);
            Logger.log(F("WCS:LOCAL_END"));
        }

        pPot->sync();
        int potValue = pPot->getValue();
        int angle = map(potValue, POTENTIOMETER_MIN, POTENTIOMETER_MAX, 0, 90);
        pServo->setPosition(angle);
        pLcd->writePercMessage(String(angle));
        char msg_angle[32];
        snprintf(msg_angle, sizeof(msg_angle), "WCS:LOCAL_PERC:%d", angle);
        Logger.log(msg_angle);

        break;
    }
    case REM_MANUAL:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("REMOTE MANUAL");
        }
        if(MsgService.isMsgAvailable(unconected)){
            Msg* msg = MsgService.receiveMsg(unconected);
            delete msg;
            this->setState(UNCONECTED);
        }
        if(MsgService.isMsgAvailable(automatic)){
            Msg* msg = MsgService.receiveMsg(automatic);
            delete msg;
            this->setState(AUTOMATIC);
        }
        if(MsgService.isMsgAvailable(control)){
        Msg* msg = MsgService.receiveMsg(control);
        int perc = this->msgMotorPerc(msg->getContent());
        this->pServo->setPosition(perc);
        delete msg;
        this->setState(REM_MANUAL);
        }
 

        break;
    }
    case UNCONECTED:
    {
        if (checkAndSetJustEntered())
        {
            pLcd->writeModeMessage("UNCONECTED");
        }
        if(MsgService.isMsgAvailable(automatic)){
            Msg* msg = MsgService.receiveMsg(automatic);
            delete msg;
            this->setState(AUTOMATIC);
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

int WCSTask::msgMotorPerc(String m)
{
    return m.substring(String(CONTROL_MSG).length()).toInt();
}