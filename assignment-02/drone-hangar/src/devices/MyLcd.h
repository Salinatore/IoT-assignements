#ifndef MYLCD_H
#define MYLCD_H

#include <Arduino.h>

class MyLcd {
private:
public:
    MyLcd();
    void writeAlarmMessage(String message);
    void writeStateMessage(String message);
};

#endif