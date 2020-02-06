#include <Arduino.h>

#include "WiFiConnectionTask.hpp"

void WiFiConnectionTask::setup(const char* ssid, const char* password, bool blocking) {
  Serial.print("Connecting to SSID ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  if(blocking) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    isConnected = true;
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println();
  }
}

void WiFiConnectionTask::loop(void) {
  updateStatus();
}

void WiFiConnectionTask::updateStatus(void) {
  if (WiFi.status() == WL_CONNECTED && !isConnected) {
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    isConnected = true;
  }
  else if(WiFi.status() != WL_CONNECTED && isConnected) {
    Serial.println("WiFi disconnected");
    isConnected = false;
  }
}
