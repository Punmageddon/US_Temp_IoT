#ifndef SDCardLoggerTask_hpp
#define SDCardLoggerTask_hpp

#include <map>

/**
 *  \brief Task that regularly writes sensor values to a csv file on the SD card
 */
class SDCardLoggerTask {
public:
  /**
   * \brief Call initially after Serial.begin(...) to set up task
   * 
   * \param spiChipSelect Chip select pin for the SD card reader on the SPI
   * \param filename The name of the csv file on the SDCard that the readings will be stored in
   * \param datapointSources The map of value names and float value data sources (csv columns)
   * \param interval The interval between measurements (should match measurement interval)
   */
  // TODO Add pointer to rtc timestamp to include as first column
  void setup(const int spiChipSelect, const char* filename, std::map<const char*, float&> datapointSources, unsigned long interval);
  
  /**
   * \brief Call once in main Arduino loop (after measurements have been taken)
   */
  void loop(void);
  
private:
  const char* filename;
  std::map<const char*, float&> datapointSources;
  unsigned long interval;
  unsigned long millisStoreNext;
  bool isSDCardAvailiable;
  int spiChipSelect;

  /**
   * \brief Write the column names (csv header) to the SD card
   * \return True, if the data was successfully written
   */
  bool writeHeader(void);

  /**
   * \brief Write the next set row of values to to the SD card
   * \return True, if the data was successfully written
   */
  bool writeValues(void);
};

#endif // SDCardLoggerTask_hpp

