#include <Arduino.h>
#include <map>

#include "config.h"
#include "SDCardConfigReader.hpp"
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

// Shared memory
float temperatureGroundTruth;
float temperature;
float humidity;

// Configuration
int bme280Interval = BME280_INTERVAL;
int sdcardInterval = SDCARD_INTERVAL;
String wifiSsid(WIFI_SSID);
String wifiPassword(WIFI_PASSWORD);
String filenameReadings(FILENAME_READINGS);
String mqttBroker(MQTT_BROKER);
int mqttPort = MQTT_PORT;
String mqttUser(MQTT_USER);
String mqttPassword(MQTT_PASSWORD);
String mqttClientId(MQTT_CLIENT_ID);
String mqttBaseTopic(MQTT_BASE_TOPIC);
int mqttInterval = MQTT_INTERVAL;

void setup() {
  pinMode(INITIALIZATION_LED, OUTPUT);
  digitalWrite(INITIALIZATION_LED, LOW); // Turn LED on
  delay(500);
  Serial.begin(9600);
  Serial.println("Sketch started");
#ifdef ENABLE_SDCARD
  SDCardConfigReader().updateConfigs(SPI_CHIP_SELECT_SD, FILENAME_CONFIG, {
    { "WIFI_SSID", wifiSsid },
    { "WIFI_PASSWORD", wifiPassword },
    { "FILENAME_READINGS", filenameReadings },
    { "MQTT_BROKER", mqttBroker },
    { "MQTT_USER", mqttUser },
    { "MQTT_PASSWORD", mqttPassword },
    { "MQTT_CLIENT_ID", mqttClientId },
    { "MQTT_BASE_TOPIC", mqttBaseTopic }
  }, {
    { "BME280_INTERVAL", bme280Interval },
    { "SDCARD_INTERVAL", sdcardInterval },
    { "MQTT_PORT", mqttPort },
    { "MQTT_INTERVAL", mqttInterval }
  });
#endif // ENABLE_SDCARD
  Wire.begin(I2C_SDA, I2C_SCL);
  bme280Task.setup(temperatureGroundTruth, humidity, bme280Interval);
  std::map<const char*, float&> datapointNameToValueMapping = {
    { "temperatureGroundTruth", temperatureGroundTruth }, { "temperature", temperature }, { "humidity", humidity }
  };
#ifdef ENABLE_SDCARD
  sdcardLoggerTask.setup(SPI_CHIP_SELECT_SD, filenameReadings.c_str(), datapointNameToValueMapping, sdcardInterval);
#endif // ENABLE_SDCARD
#ifdef ENABLE_WIFI_MQTT
  wifiConnectionTask.setup(wifiSsid.c_str(), wifiPassword.c_str());
  MqttPublishTask::ConnectionSettings mqttConnectionSettings = { mqttBroker.c_str(), mqttPort, mqttUser.c_str(), mqttPassword.c_str(), mqttClientId.c_str() };
  mqttPublishTask.setup(mqttConnectionSettings, mqttBaseTopic.c_str(), datapointNameToValueMapping, mqttInterval);
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

