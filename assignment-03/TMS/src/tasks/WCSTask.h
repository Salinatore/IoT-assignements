#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include <Arduino.h>

TaskHandle_t Task1;

class WCSTask {
public:
    WCSTask(Sonar* pSonar);
    static void tick();
    TaskFunction_t task();

    static void taskWrapper(void* param) {
        WCSTask* self = static_cast<WCSTask*>(param);
        self->tick();
    }

private:
    enum WCSState { AUTOMATIC, LOC_MANUAL, REM_MANUAL, UNCONECTED } state;

    void setState(WCSState state);
    bool checkAndSetJustEntered();

    HWPlatform* pHW;

    bool justEntered;

    Sonar* pSonar;


};

#endif