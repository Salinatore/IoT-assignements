#ifndef __DRONE_TASK__
#define __DRONE_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"

class DroneTask: public Task {
    public:
        DroneTask(HWPlatform* pHW);
        void tick();

    private:
        HWPlatform* pHW;
};


#endif