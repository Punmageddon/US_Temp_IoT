#ifndef WiFiHttpServer_hpp
#define WiFiHttpServer_hpp

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

class WiFiHttpServer {
public:
  void setup(void);
  void loop(void);
private:
  ESP8266WiFiMulti wifi;
};

#endif // WiFiHttpServer_hpp
