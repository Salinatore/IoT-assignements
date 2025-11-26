#ifndef __CONTEXT__
#define __CONTEXT__

class Context {

  private:
    bool preAlarm;
    bool alarm;
    bool droneFullyOut;
    bool imminentLanding;
    bool droneDetected;
    bool fullyIn;
    bool takeOff;
    // bool droneIn;
    // bool droneOut;
    // bool waitingForLanding;

  public:
    Context();
    bool isPreAlarm();
    bool isTakeOff();
    bool isDroneFullyOut();
    bool isAlarm();
    bool isImminentLanding();
    bool isDroneDetected();
    bool isFullyIn();

};

#endif
