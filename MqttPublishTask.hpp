#ifndef MqttPublishTask_hpp
#define MqttPublishTask_hpp

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <map>

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
   * \brief Call initially after Serial.begin(...) and WiFi has been connected to set up task. Will send to <baseTopic>/<subTopic>
   * 
   * \param broker The hostname of the MQTT broker
   * \param port The port of the MQTT broker
   * \param clientId The client id of the MQTT publisher
   * \param baseTopic The base MQTT topic without trailing backslash
   * \param subTopicSources The map of subTopics without leading backslash and float value data sources
   * \param interval The time between publishing new values or reconnection attempts
   * \param blocking If this is true and WiFi has been connected, the execution will block until successful connection to broker 
   */
  void setup(const char* broker, int port, const char* clientId, const char* baseTopic, std::map<const char*, float&> subTopicsSources, unsigned long interval, bool blocking = false);

  /**
   * \brief Call once in main Arduino loop
   */
  void loop(void);

private:
  PubSubClient pubSubClient;
  const char* clientId;
  const char* baseTopic;
  std::map<const char*, float&> subTopicsSources;
  bool isConnected;
  unsigned long interval;
  unsigned long millisPublishNext;

  /**
   * \brief Publishes data to the broker
   */
  void publish(void);

  /**
   * \brief Prints MQTT client status changes
   */
  void updateStatus(void);
};

#endif // MqttPublishTask_hpp
