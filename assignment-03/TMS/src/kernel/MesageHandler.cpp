#include <WiFi.h>
#include <PubSubClient.h>
#include "MesageHandler.h"

#define MSG_BUFFER_SIZE 50

MessageHandlerClass::MessageHandlerClass(ConnectionHandlerClass* connection)
{
    this->connection = connection;
    this->setUp();
}

//all the conection set up in need
void MessageHandlerClass::setUp()
{
    this->connection->setUpWifi();
    this->connection->selectMqttServer();
    this->connection->selectMqttServerTopic();
    this->connection->setUp();
}

//return if the connection is on
bool MessageHandlerClass::isConnectionOn()
{
    return this->connection->isConnected();
}

//try to reconnect the connection
void MessageHandlerClass::tryReconection()
{
    this->connection->tryReconect();
}

//send the message to the connection resizing to buffer size
void MessageHandlerClass::sendMsg(char *msg)
{
    char mesage[MSG_BUFFER_SIZE];
    snprintf (mesage, MSG_BUFFER_SIZE, msg);
    this->connection->publish(mesage); 
}
