#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"
#include "ConnectionHandler.h"

class MessageHandlerClass {
    ConnectionHandlerClass* connection;
    void setUp();
    

public:
    MessageHandlerClass(ConnectionHandlerClass* connection);
    bool isConnectionOn();
    void tryReconection();
    void sendMsg(char* msg);
};

#endif