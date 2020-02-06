#include <Arduino.h>

#include "MqttPublishTask.hpp"

MqttPublishTask::MqttPublishTask(WiFiClient& wifiClient) {
  pubSubClient = PubSubClient(wifiClient);
}

void MqttPublishTask::setup(const char* broker, int port, const char* clientId, const char* baseTopic, std::map<const char*, float&> subTopicsSources, unsigned long interval, bool blocking) {
  this->baseTopic = baseTopic;
  this->clientId = clientId;
  this->interval = interval;
  this->subTopicsSources = subTopicsSources;
  pubSubClient.setServer(broker, port);
  Serial.print("Connecting to MQTT broker ");
  Serial.print(broker);
  Serial.print(":");
  Serial.print(port);
  isConnected = false;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.println("Postponed connecting MQTT client, because WiFi is not connected");
  }
  else {
    do {
      isConnected = pubSubClient.connect(clientId);
      if(!isConnected && blocking) {
        delay(500);
        Serial.print(".");
      }
    } while(!isConnected && blocking);
    Serial.println();
    if(isConnected) {
      Serial.println("MQTT client connected");
    }
  }
}

void MqttPublishTask::loop(void) {
  updateStatus();
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  unsigned long currentMillis = millis();
  if(currentMillis >= millisPublishNext) {
    if(isConnected) {
      publish();
    }
    else if(pubSubClient.connect(clientId)) {
      updateStatus();
      publish();
    }
    millisPublishNext = currentMillis + interval;
  }
  pubSubClient.loop();
}

void MqttPublishTask::publish(void) {
  for(const auto& subTopicSource : subTopicsSources) {
    String topic = String(baseTopic) + "/" + String(subTopicSource.first);
    String message = String(subTopicSource.second);
    Serial.print("Publishing ");
    Serial.print(message);
    Serial.print(" to ");
    Serial.println(topic);
    pubSubClient.publish(topic.c_str(), message.c_str());
  }
}

void MqttPublishTask::updateStatus(void) {
  if (pubSubClient.connected() && !isConnected) {
    Serial.println("MQTT client connected");
    isConnected = true;
  }
  else if(!pubSubClient.connected() && isConnected) {
    Serial.println("MQTT client disconnected");
    isConnected = false;
  }
}
