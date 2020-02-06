#include <Arduino.h>

#include "MqttPublishTask.hpp"

MqttPublishTask::MqttPublishTask(WiFiClient& wifiClient) {
  pubSubClient = PubSubClient(wifiClient);
}

void MqttPublishTask::setup(const char* broker, int port, const char* clientId, const char* baseTopic, std::map<const char*, float&> subTopicsSources, unsigned long interval) {
  this->baseTopic = baseTopic;
  this->clientId = clientId;
  this->interval = interval;
  this->subTopicsSources = subTopicsSources;
  pubSubClient.setServer(broker, port);
  Serial.print("Connecting to MQTT broker ");
  Serial.print(broker);
  Serial.print(":");
  Serial.println(port);
  while(!pubSubClient.connect(clientId)) {
    delay(500);
    Serial.print(".");
  }
  isConnected = true;
  Serial.println("MQTT broker connected");
}

void MqttPublishTask::loop(void) {
  if (pubSubClient.connected() && !isConnected) {
    Serial.println("MQTT client reconnected");
    isConnected = true;
  }
  else if(!pubSubClient.connected() && isConnected) {
    Serial.println("MQTT client disconnected");
    isConnected = false;
  }
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  unsigned long currentMillis = millis();
  if(currentMillis >= millisPublishNext) {
    if(isConnected || pubSubClient.connect(clientId)) {
      for(const auto& subTopicSource : subTopicsSources) {
        String topic = String(baseTopic) + "/" + String(subTopicSource.first);
        String message = String(subTopicSource.second);
        Serial.print("Publishing ");
        Serial.print(message);
        Serial.print(" to ");
        Serial.println(topic);
        pubSubClient.publish(topic.c_str(), message.c_str());
      }
      millisPublishNext = currentMillis + interval;
    }
  }
  pubSubClient.loop();
}
