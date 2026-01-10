#include <WiFi.h>
#include <PubSubClient.h>
#include "MsgService.h"
#define MSG_BUFFER_SIZE  50

WiFiClient espClient;
PubSubClient client(espClient);

void MesageServiceClass::setUpWifi(char* wifiName, char* pw)
{
    delay(10);
    Serial.println(String("Connecting to ") + wifiName);
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiName, pw);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void MesageServiceClass::selectMqttServer(char server)
{
    this->mqtt_server = &server;
}

void MesageServiceClass::selectMqttServerTopic(char topic)
{
    this->topic = &topic;
}

void MesageServiceClass::setUp()
{
  randomSeed(micros());
  client.setServer(this->mqtt_server, 1883);
}

bool MesageServiceClass::isConnected()
{
    return client.connected();
}

void MesageServiceClass::tryReconect()
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

void MesageServiceClass::sendMsg(char* mesage)
{
    char msg[MSG_BUFFER_SIZE];
    snprintf (msg, MSG_BUFFER_SIZE, mesage);
    client.publish(this->topic, msg);  
}
