#include <Arduino.h>
#include <SD.h>

#include "SDCardLoggerTask.hpp"

void SDCardLoggerTask::setup(const int spiChipSelect, const char* filename, std::map<const char*, float&> datapointSources, unsigned long interval) {
  this->filename = filename;
  this->datapointSources = datapointSources;
  this->interval = interval;
  this->spiChipSelect = spiChipSelect;
  isSDCardAvailiable = SD.begin(spiChipSelect);
}

void SDCardLoggerTask::loop(void) {
  if(interval == 0){
    /* Logging disabled */
    return;
  }
  unsigned long currentMillis = millis();
  if(currentMillis >= millisStoreNext) {
    if(!isSDCardAvailiable) {
      isSDCardAvailiable = SD.begin(spiChipSelect);
    }
    if(isSDCardAvailiable) {
      bool shouldWriteHeader = false;
      if(!SD.exists(filename)) {
        shouldWriteHeader = true;
      }
      bool hadError = false;
      if(shouldWriteHeader) {
        hadError = !writeHeader();
      }
      if(!hadError) {
        hadError = !writeValues();
      }
      if(hadError) {
        Serial.print("Could not open file \"");
        Serial.print(filename);
        Serial.println("\" on SD card!");
      }
    }
    else {
      Serial.println("Could not initialize SD card");
    }
    millisStoreNext = currentMillis + interval;
  }
}

bool SDCardLoggerTask::writeHeader(void) {
  File file = SD.open(filename, FILE_WRITE);
  if(file) {
    bool isFirstColumn = true;
      for(const auto& datapointSource : datapointSources) {
        if(!isFirstColumn) {
          file.print(",");
        }
        file.print("\"");
        file.print(datapointSource.first);
        file.print("\"");
        isFirstColumn = false;
      }
      file.println();
    file.close();
    return true;
  }
  else {
    return false;
  }
}

bool SDCardLoggerTask::writeValues(void) {
  File file = SD.open(filename, FILE_WRITE);
  if(file) {
    Serial.print("Printing row ");
    String row = "";
    bool isFirstColumn = true;
    for(const auto& datapointSource : datapointSources) {
      if(!isFirstColumn) {
        row = String(row + ",");
      }
      row = String(row + "\"");
      row = String(row + datapointSource.second);
      row = String(row + "\"");
      isFirstColumn = false;
    }
    file.println(row);
    file.close();
    Serial.print(row);
    Serial.print(" to \"");
    Serial.print(filename);
    Serial.println("\"");
    return true;
  }
  else {
    return false;
  }
}
