#include <Arduino.h>

#include "config.h"
#include "SerialHeartbeatTask.hpp"
#include "BME280Task.hpp"

#ifdef ENABLE_WIFI_MQTT
#include <ESP8266WiFi.h>
#include <Wire.h>

#include "WifiConnectionTask.hpp"
#include "MqttPublishTask.hpp"

WiFiClient wifiClient;

WiFiConnectionTask wifiConnectionTask;
MqttPublishTask mqttPublishTask(wifiClient);
#endif // ENABLE_WIFI_MQTT
SerialHeartbeatTask heartbeatTask;
BME280Task bme280Task;

float temperatureGroundTruth;
float temperature;
float humidity;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch started");
  Wire.begin(I2C_SDA, I2C_SCL);
  bme280Task.setup(temperatureGroundTruth, humidity, 1000);
#ifdef ENABLE_WIFI_MQTT
  wifiConnectionTask.setup(WIFI_SSID, WIFI_PASSWORD);
  MqttPublishTask::ConnectionSettings mqttConnectionSettings = { MQTT_BROKER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENT_ID };
  mqttPublishTask.setup(mqttConnectionSettings, MQTT_BASE_TOPIC, {
    { "temperatureGroundTruth", temperatureGroundTruth }, { "temperature", temperature }, { "humidity", humidity }
  }, 1000);
#endif // ENABLE_WIFI_MQTT
  heartbeatTask.setup(10000);
}

void loop() {
#ifdef ENABLE_WIFI_MQTT
  bme280Task.loop();
  wifiConnectionTask.loop();
  mqttPublishTask.loop();
#endif // ENABLE_WIFI_MQTT
  heartbeatTask.loop();
}
