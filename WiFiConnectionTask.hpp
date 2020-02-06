#ifndef WiFiConnectionTask_hpp
#define WiFiConnectionTask_hpp

#include <ESP8266WiFi.h>

/**
 *  \brief ESP8266 Wifi with status logging
 */
class WiFiConnectionTask {
public:
  /**
   * \brief Call initially after Serial.begin(...) to set up task
   * 
   * \param ssid The SSID of the access point
   * \param password The password of the access point
   * \param blocking If this is true, the execution will block until WiFi has been connected
   */
  void setup(const char* ssid, const char* password, bool blocking = false);

  /**
   * \brief Call once in main Arduino loop
   */
  void loop(void);
  
private:
  bool isConnected;

  /**
   * \brief Prints WiFi status changes
   */
  void updateStatus(void);
};

#endif // WiFiConnectionTask_hpp
