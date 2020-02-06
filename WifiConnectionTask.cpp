#include <Arduino.h>

#include "WiFiConnectionTask.hpp"

void WiFiConnectionTask::setup(const char* ssid, const char* password) {
  Serial.print("Connecting to SSID ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  isConnected = true;
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiConnectionTask::loop(void) {
  if (WiFi.status() == WL_CONNECTED && !isConnected) {
    Serial.println("WiFi reconnected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    isConnected = true;
  }
  else if(WiFi.status() != WL_CONNECTED && isConnected) {
    Serial.println("WiFi disconnected");
    isConnected = false;
  }
}
