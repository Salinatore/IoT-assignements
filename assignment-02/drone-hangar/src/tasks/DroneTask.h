#ifndef __DRONE_TASK__
#define __DRONE_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include "model/Context.h"

class DroneTask: public Task {
    public:
        DroneTask(HWPlatform* pHW, Context* context);
        void tick();

    private:
        HWPlatform* pHW;
        Context* context;

        enum { IN, TAKE_OFF, OUT, WAITING_FOR_LANDING, LANDING, ALARM_IN, ALARM_OUT } state;
};


#endif