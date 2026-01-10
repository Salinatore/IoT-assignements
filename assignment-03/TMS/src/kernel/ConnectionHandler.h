#ifndef __CONNECTION__
#define __CONNECTION__

#include "Arduino.h"

class ConnectionHandlerClass {
    const char* mqtt_server;
    const char* topic;

public:
    void setUpWifi();
    void selectMqttServer();
    void selectMqttServerTopic();
    void setUp();
    bool isConnected();
    void tryReconect();
    void publish(char* msg);
};

#endif