#include <Arduino.h>

#include "MqttPublishTask.hpp"

MqttPublishTask::MqttPublishTask(WiFiClient& wifiClient) {
  pubSubClient = PubSubClient(wifiClient);
}

void MqttPublishTask::setup(const char* broker, int port, const char* clientId, const char* baseTopic) {
  this->baseTopic = baseTopic;

  pubSubClient.setServer(broker, port);
  Serial.print("Connecting to MQTT broker ");
  Serial.print(broker);
  Serial.print(":");
  Serial.println(port);
  while(!pubSubClient.connect(clientId)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("MQTT broker connected");
}

void MqttPublishTask::loop(void) {
  static int i = 0;
  if(millis() % 5000 == 0) {
    String message = String("hello MQTT! ") + String(i++);
    Serial.print("Publishing ");
    Serial.print(message);
    Serial.print(" to ");
    Serial.println(baseTopic);
    pubSubClient.publish(baseTopic, message.c_str());
  }
  pubSubClient.loop();
}
