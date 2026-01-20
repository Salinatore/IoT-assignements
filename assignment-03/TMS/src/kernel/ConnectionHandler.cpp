#include <WiFi.h>
#include <PubSubClient.h>
#include "ConnectionHandler.h"
#include "config.h"

#define MSG_BUFFER_SIZE  50

WiFiClient espClient;
PubSubClient client(espClient);

//set up the wifi an loop until it si connected
void ConnectionHandlerClass::setUpWifi()
{
    Serial.println(String("Connecting to ") + WIFI_NAME);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NAME, WIFI_PW);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
}

//take the server name form the config 
void ConnectionHandlerClass::selectMqttServer()
{
    this->mqtt_server = MQTT_SERVER_NAME;
}

//take the topic form the form
void ConnectionHandlerClass::selectMqttServerTopic()
{
    this->topic = MQTT_TOPIC;
}

//set up the connection to the mqtt server
void ConnectionHandlerClass::setUp()
{
  client.setServer(this->mqtt_server, 1883);
  if(this->isConnected()){
    Serial.println("connected to server");
  }
  else{
    Serial.println("not connected to server");
  }
}

//return it the connection is enstablished
bool ConnectionHandlerClass::isConnected()
{
    return client.connected();
}

//trying to reconnect if the connection is down
void ConnectionHandlerClass::tryReconect()
{
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        
        // Create a random client ID
        String clientId = String("esiot-2025-client-")+String(random(0xffff), HEX);

        // Attempt to connect
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
            client.subscribe(this->topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
    }
  }
}

//publish the message in the setted topic
void ConnectionHandlerClass::publish(char* msg)
{
    client.publish(this->topic, msg);  
}

