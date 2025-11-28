#ifndef __LED_TASK__
#define __LED_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include "model/Context.h"

class LedTask: public Task {
public:
    LedTask(Led* pLed1, Led* pLed2, Led* pLedR, Context* pContext);
    void tick();

private:
    enum State { IDLE, MOVE_FASE, ALARM } state;
    void setState(State state);
    long elapsedTime();
    bool checkAndSetJustEntered();
    
    bool isOn;
    long timestamp;
    bool justEntered;

    Led* pLed1;
    Led* pLed2;
    Led* pLedR;
    Context* pContext;
};


#endif