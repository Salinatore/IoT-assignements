#ifndef __DRONE_TASK__
#define __DRONE_TASK__

#include "kernel/Task.h"
#include "model/HWPlatform.h"
#include "model/Context.h"

class DroneTask: public Task {
    public:
        DroneTask(Context* context, Pir* presenceDetector, Sonar* distanceDetector, ServoMotorImpl* servo, MyLcd* lcd);
        void tick();

    private:
        unsigned long time;
        bool imminentLanding;
        bool isTimerActive;
        bool justEntered;
        HWPlatform* pHW;
        Context* context;
        Pir* presenceDetector;
        Sonar* distanceDetector;
        ServoMotorImpl* servo;
        MyLcd* lcd;

        enum State { IN, TAKE_OFF, OUT, WAITING_FOR_LANDING, LANDING, ALARM_IN, ALARM_OUT } state;

        void setState(State state);
        bool checkAndSetJustEntered();
        bool isDoorOpen();
};


#endif