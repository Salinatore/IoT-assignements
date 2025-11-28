#ifndef __DRONE_TASK__
#define __DRONE_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include "model/Context.h"

class DroneTask: public Task {
    public:
        DroneTask(Context* context, Pir* presenceDetector, Sonar* distanceDetector, TempSensorTMP36* tempSensor, ServoMotor* servo);
        void tick();

    private:
        HWPlatform* pHW;
        Context* context;
        bool isDoorOpen;
        bool imminentLanding;
        unsigned long time;
        bool isTimerActive;
        Pir* presenceDetector;
        Sonar* distanceDetector;
        TempSensorTMP36* tempSensor;
        ServoMotor* servo;

        enum { IN, TAKE_OFF, OUT, WAITING_FOR_LANDING, LANDING, ALARM_IN, ALARM_OUT } state;
};


#endif