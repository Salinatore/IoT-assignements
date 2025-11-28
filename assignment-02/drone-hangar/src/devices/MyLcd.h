#ifndef MYLCD_H
#define MYLCD_H

#include <Arduino.h>

class MyLcd {
private:
public:
    MyLcd();
    void writeMessage(String message);
};

#endif