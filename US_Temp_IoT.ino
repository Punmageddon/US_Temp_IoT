#include <Arduino.h>

#include "SerialHeartbeatTask.hpp"
#include "WifiConnectionTask.hpp"
#include "MqttPublishTask.hpp"

SerialHeartbeatTask heartbeatTask;
WiFiConnectionTask wifiConnectionTask;
MqttPublishTask mqttPublishTask;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch started");

  wifiConnectionTask.setup("**********", "**********");
  heartbeatTask.setup(5000);
  mqttPublishTask.setup();
}

void loop() {
  wifiConnectionTask.loop();
  heartbeatTask.loop();
  mqttPublishTask.loop();
}
