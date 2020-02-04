#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "SerialHeartbeatTask.hpp"
#include "WifiConnectionTask.hpp"
#include "MqttPublishTask.hpp"

// WiFi
const char* ssid;
const char* password;
// MQTT
const char* broker;
int port;
const char* clientId;
const char* baseTopic;

WiFiClient wifiClient;

SerialHeartbeatTask heartbeatTask;
WiFiConnectionTask wifiConnectionTask;
MqttPublishTask mqttPublishTask(wifiClient);

void setup() {
#include "config.h"

  Serial.begin(9600);
  Serial.println("Sketch started");

  wifiConnectionTask.setup(ssid, password);
  heartbeatTask.setup(5000);
  mqttPublishTask.setup(broker, port, clientId, baseTopic);
}

void loop() {
  wifiConnectionTask.loop();
  heartbeatTask.loop();
  mqttPublishTask.loop();
}
