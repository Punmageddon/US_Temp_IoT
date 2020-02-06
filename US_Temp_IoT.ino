#include <Arduino.h>

#include "config.h"
#include "SerialHeartbeatTask.hpp"

#ifdef ENABLE_WIFI_MQTT
#include <ESP8266WiFi.h>

#include "WifiConnectionTask.hpp"
#include "MqttPublishTask.hpp"

WiFiClient wifiClient;

WiFiConnectionTask wifiConnectionTask;
MqttPublishTask mqttPublishTask(wifiClient);
#endif // ENABLE_WIFI_MQTT
SerialHeartbeatTask heartbeatTask;

float temperatureGroundTruth;
float temperature;
float humidity;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch started");
#ifdef ENABLE_WIFI_MQTT
  wifiConnectionTask.setup(WIFI_SSID, WIFI_PASSWORD);
  mqttPublishTask.setup(MQTT_BROKER, MQTT_PORT, MQTT_CLIENT_ID, MQTT_BASE_TOPIC, {
    { "temperatureGroundTruth", temperatureGroundTruth }, { "temperature", temperature }, { "humidity", humidity }
  }, 1000);
#endif // ENABLE_WIFI_MQTT
  heartbeatTask.setup(10000);
}

void loop() {
#ifdef ENABLE_WIFI_MQTT
  wifiConnectionTask.loop();
  mqttPublishTask.loop();
#endif // ENABLE_WIFI_MQTT
  heartbeatTask.loop();
}
