#ifndef MqttPublishTask_hpp
#define MqttPublishTask_hpp

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/**
 *  \brief MQTT-Publisher
 */
class MqttPublishTask {
public:
  /**
   * Constructor
   * 
   * \brief wifiClient ESP8266 WifiClient
   */
  MqttPublishTask(WiFiClient& wifiClient);

  /**
   * \brief Call initially after Serial.begin(...) and WiFi has been connected to set up task
   * 
   * \param broker The hostname of the MQTT broker
   * \param port The port of the MQTT broker
   * \param clientId The client id of the MQTT publisher
   * \param baseTopic The base MQTT topic without trailing backslash
   */
  void setup(const char* broker, int port, const char* clientId, const char* baseTopic);

  /**
   * \brief Call once in main Arduino loop
   */
  void loop(void);

private:
  PubSubClient pubSubClient;
  const char* baseTopic;
};

#endif // MqttPublishTask_hpp
