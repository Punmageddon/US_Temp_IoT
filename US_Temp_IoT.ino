#include <Arduino.h>
#include <map>

#include "config.h"
#include "SerialHeartbeatTask.hpp"
#include "BME280Task.hpp"
#include "SDCardLoggerTask.hpp"

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

#ifdef ENABLE_SDCARD
SDCardLoggerTask sdcardLoggerTask;
#endif // ENABLE_SDCARD

float temperatureGroundTruth;
float temperature;
float humidity;

void setup() {
  pinMode(INITIALIZATION_LED, OUTPUT);
  digitalWrite(INITIALIZATION_LED, LOW); // Turn LED on
  delay(500);
  Serial.begin(9600);
  Serial.println("Sketch started");
  Wire.begin(I2C_SDA, I2C_SCL);
  bme280Task.setup(temperatureGroundTruth, humidity, 1000);
  std::map<const char*, float&> datapointNameToValueMapping = {
    { "temperatureGroundTruth", temperatureGroundTruth }, { "temperature", temperature }, { "humidity", humidity }
  };
#ifdef ENABLE_SDCARD
  sdcardLoggerTask.setup(SPI_CHIP_SELECT_SD, FILENAME, datapointNameToValueMapping, 1000);
#endif // ENABLE_SDCARD
#ifdef ENABLE_WIFI_MQTT
  wifiConnectionTask.setup(WIFI_SSID, WIFI_PASSWORD);
  MqttPublishTask::ConnectionSettings mqttConnectionSettings = { MQTT_BROKER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENT_ID };
  mqttPublishTask.setup(mqttConnectionSettings, MQTT_BASE_TOPIC, datapointNameToValueMapping, 1000);
#endif // ENABLE_WIFI_MQTT
  heartbeatTask.setup(10000);
  delay(500);
  digitalWrite(INITIALIZATION_LED, HIGH); // Turn LED off
}

void loop() {
  bme280Task.loop();
#ifdef ENABLE_SDCARD
  sdcardLoggerTask.loop();
#endif // ENABLE_SDCARD
#ifdef ENABLE_WIFI_MQTT
  wifiConnectionTask.loop();
  mqttPublishTask.loop();
#endif // ENABLE_WIFI_MQTT
  heartbeatTask.loop();
}

