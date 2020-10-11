#ifndef SDCardConfigReader_hpp
#define SDCardConfigReader_hpp

#include <Arduino.h>
#include <map>

/**
 *  \brief Overrides default settings based on configuration on SD card
 */
class SDCardConfigReader {
public:
  /**
   * \brief Call initially after Serial.begin(...) to update configs before applying them
   * 
   * \param spiChipSelect Chip select pin for the SD card reader on the SPI
   * \param filename The name of the configuration file on the SD card
   * \param stringConfigs Names and locations of configurations containing string values to be updated
   * \param intConfigs Names and locations of configurations containing integer values to be updated
   */
  void updateConfigs(const int spiChipSelect, const char* filename, std::map<const char*, String&> stringConfigs, std::map<const char*, int&> intConfigs);
  
private:
  /**
   * \brief Apply single line from config file
   * 
   * \param line A single line from the config file
   * \param stringConfigs Names and locations of configurations containing string values to be updated
   * \param intConfigs Names and locations of configurations containing integer values to be updated
   */
	void applyConfig(const char* line, std::map<const char*, String&> stringConfigs, std::map<const char*, int&> intConfigs);
};

#endif // SDCardConfigReader_hpp

