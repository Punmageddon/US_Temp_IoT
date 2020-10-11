#include <Arduino.h>

#include "MqttPublishTask.hpp"

MqttPublishTask::MqttPublishTask(WiFiClient& wifiClient) {
  pubSubClient = PubSubClient(wifiClient);
}

void MqttPublishTask::setup(const ConnectionSettings connectionSettings, const char* baseTopic, std::map<const char*, float&> subTopicsSources, unsigned long interval, bool blocking) {
  this->connectionSettings = connectionSettings;
  this->baseTopic = baseTopic;
  this->interval = interval;
  this->subTopicsSources = subTopicsSources;
  pubSubClient.setServer(connectionSettings.broker, connectionSettings.port);
  Serial.print("Connecting to MQTT broker ");
  Serial.print(connectionSettings.broker);
  Serial.print(":");
  Serial.print(connectionSettings.port);
  isConnected = false;
  if(interval == 0){ /* MQTT disabled */ }
  else if (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.println("Postponed connecting MQTT client, because WiFi is not connected");
  }
  else {
    do {
      isConnected = connect();
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

bool MqttPublishTask::connect(void) {
  return pubSubClient.connect(
    connectionSettings.clientId,
    connectionSettings.user[0] != '\0' ? connectionSettings.user : NULL,
    connectionSettings.password[0] != '\0' ? connectionSettings.password : NULL
  );
}

void MqttPublishTask::loop(void) {
  if(interval == 0){
    /* MQTT disabled */
    return;
  }
  updateStatus();
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  unsigned long currentMillis = millis();
  if(currentMillis >= millisPublishNext) {
    if(isConnected) {
      publish();
    }
    else if(pubSubClient.connect(connectionSettings.clientId, connectionSettings.user, connectionSettings.password)) {
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
    String message(subTopicSource.second);
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
