#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"

class MesageServiceClass {
    char* mqtt_server;
    char* topic;

public:
    void setUpWifi(char* wifiName, char* pw);
    void selectMqttServer(char server);
    void selectMqttServerTopic(char topic);
    void setUp();
    bool isConnected();
    void tryReconect();
    void sendMsg(char* msg);
};

#endif