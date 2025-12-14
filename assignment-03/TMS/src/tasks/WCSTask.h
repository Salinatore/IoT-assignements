#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"

class WCSTask: public Task {
public:
    WCSTask(Sonar* pSonar);
    void tick();

private:
    enum WCSState { AUTOMATIC, LOC_MANUAL, REM_MANUAL, UNCONECTED } state;

    void setState(WCSState state);
    bool checkAndSetJustEntered();

    HWPlatform* pHW;

    bool justEntered;

    Sonar* pSonar;


};

#endif