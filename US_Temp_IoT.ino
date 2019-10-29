#include "WiFiHttpServer.hpp"

WiFiHttpServer server;

void setup() {
  Serial.begin(9600);
  Serial.println("Sketch started");

  server.setup();
}


void loop() {
  server.loop();
}
