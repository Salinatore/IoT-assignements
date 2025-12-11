#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

  private:
    bool preAlarm;
    bool alarm;
    bool droneOut;
    bool takeOff;
    bool landing;

  public:
    Context();

    bool isTakeOff();
    bool isDroneOut();
    bool isLanding();

    bool isAlarm();
    bool isPreAlarm();

    void setDroneOut(bool out);
    void setLanding(bool landing);
    void setTakeOff(bool takeoff);
    
    void setAlarm(bool alarm);
    void setPreAlarm(bool preAlarm);
};

#endif
