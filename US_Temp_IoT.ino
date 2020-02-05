#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "SerialHeartbeatTask.hpp"
#include "WifiConnectionTask.hpp"
#include "MqttPublishTask.hpp"
#include "config.h"

WiFiClient wifiClient;

SerialHeartbeatTask heartbeatTask;
WiFiConnectionTask wifiConnectionTask;
MqttPublishTask mqttPublishTask(wifiClient);

float humidity;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch started");

  wifiConnectionTask.setup(WIFI_SSID, WIFI_PASSWORD);
  heartbeatTask.setup(10000);
  mqttPublishTask.setup(MQTT_BROKER, MQTT_PORT, MQTT_CLIENT_ID, MQTT_BASE_TOPIC, { { "humidity", humidity } }, 1000);
}

void loop() {
  wifiConnectionTask.loop();
  heartbeatTask.loop();
  mqttPublishTask.loop();
}
