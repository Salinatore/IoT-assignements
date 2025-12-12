#ifndef MYLCD_H
#define MYLCD_H

#include <Arduino.h>

class MyLcd {
private:
public:
    MyLcd();
    void writeModeMessage(String message);
    void writePercMessage(String message);
private:
    void refresh();

    String modeMessage;
    String percMessage;
};

#endif