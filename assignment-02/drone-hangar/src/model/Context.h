#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

  private:
    bool preAlarm;
    bool alarm;
    bool droneOut;
    bool takeOff;
    bool landing;
    // bool droneIn;
    // bool droneOut;
    // bool waitingForLanding;

  public:
    Context();
    bool isPreAlarm();
    bool isTakeOff();
    bool isAlarm();
    bool isDroneOut();
    bool isLanding();
    void setDroneOut(bool out);
    void setLanding(bool landing);
    void setTakeOff(bool takeoff);
};

#endif
