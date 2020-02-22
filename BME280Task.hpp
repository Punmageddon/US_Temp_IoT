#ifndef BME280Task_hpp
#define BME280Task_hpp

#include "SparkFunBME280.h"

/**
 *  \brief Task that regularly measures temperature and humidity using Bosch BME280 sensor over I2C
 */
class BME280Task {
public:
  /**
   * \brief Call initially after Serial.begin(...) and Wire.begin(...) to set up task
   * 
   * \param temperature The variable to store the measured temperature in
   * \param humidity The variable to store the measured humidity in
   * \param interval The interval between measurements
   */
  void setup(float& temperature, float& humidity, unsigned long interval);
  
  /**
   * \brief Call once in main Arduino loop
   */
  void loop(void);
  
private:
  BME280 bme280;
  bool isSensorAvailable;
  float* temperature;
  float* humidity;
  unsigned long interval;
  unsigned long millisNextMeasurement;
};

#endif // BME280Task_hpp
