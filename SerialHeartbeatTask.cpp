#include <Arduino.h>

#include "SerialHeartbeatTask.hpp"

void SerialHeartbeatTask::setup(unsigned long interval) {
    this->interval = interval;
    millisNextHeartbeat = millis();
}

void SerialHeartbeatTask::loop(void) {
    unsigned long currentMillis = millis();
    if(currentMillis >= millisNextHeartbeat) {
        Serial.println("Beep!");
        millisNextHeartbeat = currentMillis + interval;
    }
}
