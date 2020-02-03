#include <Arduino.h>

#include "SerialHeartbeatTask.hpp"

SerialHeartbeatTask heartbeat;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch started");

  heartbeat.setup(5000);
}

void loop() {
  heartbeat.loop();
}
