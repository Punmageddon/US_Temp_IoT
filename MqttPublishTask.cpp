#include <Arduino.h>

#include "MqttPublishTask.hpp"

MqttPublishTask::MqttPublishTask() {
  pubSubClient = PubSubClient(wifiClient);
}

void MqttPublishTask::setup(void) {
  // WiFi
  Serial.print("Connecting to SSID ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // MQTT
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
    String message = String("hello mqtt! ") + String(i++);
    Serial.print("Publishing ");
    Serial.print(message);
    Serial.print(" to ");
    Serial.println(topic);
    pubSubClient.publish(topic, message.c_str());
  }
  pubSubClient.loop();
}
