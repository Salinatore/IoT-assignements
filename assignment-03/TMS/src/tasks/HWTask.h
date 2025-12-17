#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include <Arduino.h>


class HWTask : Task{
public:
    HWTask(Sonar* pSonar);
    void tick();
    void task(void * pvParameters);

    static void taskWrapper(void* pvParameters) {
        HWTask* self = static_cast<HWTask*>(pvParameters);
        self->task(pvParameters);
    }


private:
    enum WCSState { IDLE, UNCONECTED } state;

    void setState(WCSState state);
    bool checkAndSetJustEntered();

    HWPlatform* pHW;

    bool justEntered;

    Sonar* pSonar;

};

#endif