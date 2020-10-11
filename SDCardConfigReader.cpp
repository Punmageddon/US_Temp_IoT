#include "SDCardConfigReader.hpp"

#include <SD.h>

void SDCardConfigReader::updateConfigs(const int spiChipSelect, const char* filename, std::map<const char*, String&> stringConfigs, std::map<const char*, int&> intConfigs){
  if(SD.begin(spiChipSelect)) {
    if(SD.exists(filename)) {
      Serial.println("Applying configs from SD card");
      File file = SD.open(filename, FILE_READ);
      String line;
      while(file.available()){
        char nextChar = file.read();
        switch (nextChar)
        {
        case '\r':
          /* Ignore return carriage */
          break;
        case '\n':
          applyConfig(line.c_str(), stringConfigs, intConfigs);
          line = String();
          break;
        case ';':
          // Rest of the line is comment
          while(file.available() && file.read() != '\n');
          applyConfig(line.c_str(), stringConfigs, intConfigs);
          break;
        default:
          line += nextChar;
          break;
        }
      }
      applyConfig(line.c_str(), stringConfigs, intConfigs);
    }
  }
  else {
    Serial.println("Could not initialize SD card");
  }
}

void SDCardConfigReader::applyConfig(const char* line, std::map<const char*, String&> stringConfigs, std::map<const char*, int&> intConfigs){
    String buffer(line);
    if(buffer.length() == 0){
      return;
    }
    String value;
    int separatorIndex = buffer.indexOf('=');
    if(separatorIndex != -1){
      buffer.setCharAt(separatorIndex, '\0');
      value = String(buffer.c_str() + separatorIndex + 1);
    }
    else {
      Serial.print("Invalid line in config (Doesn't match format \"key=value\"): ");
      Serial.println(line);
      return;
    }
    String key(buffer.c_str());
    for(const auto& stringConfig : stringConfigs) {
      if(key.compareTo(String(stringConfig.first)) == 0) {
        stringConfig.second = *new String(value); // Config will remain in heap as zombie memory, but it's not a big deal :D
        return;
      }
    }
    for(const auto& intConfig : intConfigs) {
      if(key.compareTo(String(intConfig.first)) == 0) {
        intConfig.second = value.toInt();
        String trimmedValue(value);
        trimmedValue.trim();
        if(String(intConfig.second).compareTo(trimmedValue) != 0) {
          Serial.print("Invalid line in config (Expected an integer): ");
          Serial.println(line);
        }
        return;
      }
    }
    Serial.print("Invalid line in config (Unknown key): ");
    Serial.println(line);
}