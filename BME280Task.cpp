#include <Arduino.h>

#include "BME280Task.hpp"

void BME280Task::setup(float& temperature, float& humidity, unsigned long interval){
  this->temperature = &temperature;
  this->humidity = &humidity;
  this->interval = interval;
  bme280.setI2CAddress(0x76);
  isSensorAvailable = bme280.beginI2C();
}

void BME280Task::loop(void) {
  unsigned long currentMillis = millis();
  if(currentMillis >= millisNextMeasurement) {
    millisNextMeasurement = currentMillis + interval;
    if(!isSensorAvailable) {
      isSensorAvailable = bme280.beginI2C();
    }
    if(isSensorAvailable) {
      *temperature = bme280.readTempC();
      *humidity = bme280.readFloatHumidity();
      Serial.print("Reading from BME: ");
      Serial.print(*temperature);
      Serial.print("°C, ");
      Serial.print(*humidity);
      Serial.println("%");
    }
    else {
      Serial.println("Could not connect BME280");
    }
  }
}
