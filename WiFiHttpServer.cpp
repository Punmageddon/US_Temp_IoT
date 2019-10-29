#include "WiFiHttpServer.hpp"

#include <ESP8266HTTPClient.h>
#include "WiFiCredentials.hpp" // Must define char* WIFI_SSID and WIFI_PASSWORD

// TODO Replace test code

#define HTTPS_PORT 443
#define URL "http://example.com" // NOTE test code

void WiFiHttpServer::setup(void) {
  wdt_disable();
  WiFi.mode(WIFI_STA);
  wifi.addAP(WIFI_SSID, WIFI_PASSWORD);
}

void WiFiHttpServer::loop(void) {
  // NOTE test code begin (http client -> http server)
  if(wifi.run() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  BearSSL::WiFiClientSecure client;

  HTTPClient https;
  if(!https.begin(client, URL)) {
    Serial.println(String("Could not connect to ") + URL);
    return;
  }
  int error = https.POST("some payload");
  Serial.println(https.errorToString(error));

  delay(5 * 1000);
  // NOTE test code end
}
