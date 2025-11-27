#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

  private:
    bool preAlarm;
    bool alarm;
    bool fullyOut;
    bool takeOff;
    bool landing;
    // bool droneIn;
    // bool droneOut;
    // bool waitingForLanding;

  public:
    Context();
    bool isPreAlarm();
    bool isTakeOff();
    bool isDroneFullyOut();
    bool isAlarm();
    bool isFullyOut();
    void setFullyOut(bool out);
    void setLanding(bool landing);
    void setTakeOff(bool takeoff);
};

#endif
