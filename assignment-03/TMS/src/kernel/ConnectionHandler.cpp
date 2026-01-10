#include <WiFi.h>
#include <PubSubClient.h>
#include "ConnectionHandler.h"
#include "config.h"
#define MSG_BUFFER_SIZE  50

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(String("Message arrived on [") + topic + "] len: " + length + " txt: " + String((char*)payload, length) );
}

void ConnectionHandlerClass::setUpWifi()
{
    delay(10);
    Serial.println(String("Connecting to ") + WIFI_NAME);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NAME, WIFI_PW);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void ConnectionHandlerClass::selectMqttServer()
{
    this->mqtt_server = MQTT_SERVER_NAME;
}

void ConnectionHandlerClass::selectMqttServerTopic()
{
    this->topic = MQTT_TOPIC;
}

void ConnectionHandlerClass::setUp()
{
  randomSeed(micros());
  client.setServer(this->mqtt_server, 1883);
  client.setCallback(callback);
}

bool ConnectionHandlerClass::isConnected()
{
    return client.connected();
}

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

void ConnectionHandlerClass::publish(char* msg)
{
    client.publish(this->topic, msg);  
}

