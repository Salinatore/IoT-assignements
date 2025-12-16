#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"

class WCSTask: public Task {
public:
    WCSTask(ServoMotor* pServo, Lcd* pLcd, Button* pButton, Potentiometer* pPot);
    void tick();

private:
    enum WCSState { AUTOMATIC, LOCAL_MANUAL, REMOTE_MANUAL, UNCONECTED } state;

    void setState(WCSState state);
    bool checkAndSetJustEntered();
    int msgMotorPerc(String message);

    HWPlatform* pHW;

    bool justEntered;

    ServoMotor* pServo;
    Lcd* pLcd;
    Button* pButton;
    Potentiometer* pPot;
};

#endif