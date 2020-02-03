#include <Arduino.h>

#include "SerialHeartbeatTask.hpp"
#include "MqttPublishTask.hpp"

SerialHeartbeatTask heartbeat;
MqttPublishTask mqttPublishTask;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch started");

  heartbeat.setup(5000);
  mqttPublishTask.setup();
}

void loop() {
  heartbeat.loop();
  mqttPublishTask.loop();
}
