#include <WiFi.h>
#include <PubSubClient.h>
#include "MesageHandler.h"

#define MSG_BUFFER_SIZE 50

MessageHandlerClass::MessageHandlerClass(ConnectionHandlerClass* connection)
{
    this->connection = connection;
    this->setUp();
}

void MessageHandlerClass::setUp()
{
    this->connection->setUpWifi();
    this->connection->selectMqttServer();
    this->connection->selectMqttServerTopic();
    this->connection->setUp();
}

bool MessageHandlerClass::isConnectionOn()
{
    return this->connection->isConnected();
}

void MessageHandlerClass::tryReconection()
{
    this->connection->tryReconect();
}

void MessageHandlerClass::sendMsg(char *msg)
{
    char mesage[MSG_BUFFER_SIZE];
    snprintf (mesage, MSG_BUFFER_SIZE, msg);
    this->connection->publish(mesage); 
}
