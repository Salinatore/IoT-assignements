#ifndef __ALARM_TASK__
#define __ALARM_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include "kernel/MesageHandler.h"
#include "kernel/ConnectionHandler.h"
#include "config.h"
#include <Arduino.h>


class HWTask : Task{
public:
    HWTask(Sonar* pSonar, Led* redLed, Led* greenLed);
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
    void sendMsg(float value);

    HWPlatform* pHW;
    MessageHandlerClass* msgHandler;

    bool justEntered;

    Sonar* pSonar;
    Led* redLed;
    Led* greenLed;

};

#endif