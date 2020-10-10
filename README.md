# US_Temp_IoT
Creating a thermometer that uses an ultrasonic transducer that is calibrated to a set distance by a laser time of flight sensor to measure the average temperature of a given stretch of air.  
Ground truth readings are provided by a [BME280 sensor](https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/).  
Readings can be stored on an SD card or broadcast over MQTT.

## Dependencies
* [Arduino for ESP8266](https://github.com/esp8266/Arduino)
* [PubSubClient](https://github.com/knolleary/pubsubclient)
* [SparkFun BME280 Arduino Library](https://github.com/sparkfun/SparkFun_BME280_Arduino_Library)

## Setup
1. Create _config.h_ based on _template-config.h_
2. Update preprocessor directives in the newly created _config.h_ file
3. Compile Sketch and Flash Board

## Recording data
You can use another computer like a Raspberry Pi to record the measurements that are published to the MQTT broker.
1. Install the [Eclipse Paho™ MQTT Python Client](https://github.com/eclipse/paho.mqtt.python)
2. Run `./recorder.py -b MQTT_BROKER -s MQTT_BASE_TOPIC -o measurements.csv`  
(`MQTT_BROKER` and `MQTT_BASE_TOPIC` must your match _config.h_)
3. Use `./recorder.py -h` for a list of all options
