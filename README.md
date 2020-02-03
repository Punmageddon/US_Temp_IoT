# US_Temp_IoT
Creating a thermometer that uses an ultrasonic transducer that is calibrated to a set distance by a laser time of flight sensor to measure the average temperature of a given stretch of air.

## Dependencies
* Arduino (ESP8266)
* [PubSubClient](https://github.com/knolleary/pubsubclient) library

## Setup
1. Set `ssid`, `password`, `clientId` and `topic` in _MqttPublishTask.hpp_
2. Flash Board
