#ifndef WiFiConnectionTask_hpp
#define WiFiConnectionTask_hpp

#include <ESP8266WiFi.h>

/**
 *  \brief ESP8266 Wifi with status logging to Serial
 */
class WiFiConnectionTask {
public:
  /**
   * \brief Call initially after Serial.begin(...) to set up task
   * 
   * \param ssid The SSID of the access point
   * \param password The password of the access point
   */
  void setup(const char* ssid, const char* password);

  /**
   * \brief Call once in main Arduino loop
   */
  void loop(void);
  
private:
  bool isConnected;
};

#endif // WiFiConnectionTask_hpp
