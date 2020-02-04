#ifndef MqttPublishTask_hpp
#define MqttPublishTask_hpp

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/**
 *  \brief MQTT-Publisher
 */
class MqttPublishTask {
public:
  MqttPublishTask();
  /**
   * \brief Call initially after Serial.begin(...) to set up task
   */
  void setup(void);

  /**
   * \brief Call once in main Arduino loop
   */
  void loop(void);
private:
  PubSubClient pubSubClient;
  char* broker = "broker.hivemq.com";
  char* clientId = "**********";
  int port = 1883;
  char* topic = "**********";
};

#endif // MqttPublishTask_hpp
