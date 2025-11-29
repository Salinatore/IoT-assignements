#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include "model/Context.h"

class AlarmTask: public Task {
public:
    AlarmTask(Context* pContext, TempSensorTMP36* pTempSensor, MyLcd* pLcd, Button* pButton);
    void tick();

private:
    enum State { IDLE, ALARM, PRE_ALARM } state;

    void setState(State state);
    long elapsedTime();
    bool checkAndSetJustEntered();

    HWPlatform* pHW;
    Context* pContext;

    long timestamp;
    bool justEntered;
    bool tempUp;

    TempSensorTMP36* pTempSensor;
    MyLcd* pLcd;
    Button* pButton;
};

#endif