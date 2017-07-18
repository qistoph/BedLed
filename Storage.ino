#include "Storage.h"

#include <EEPROM.h>

void eepromLoad(unsigned int address, void *value, size_t len) {
  uint8_t *data = value;
  for(size_t i = 0; i<len; ++i) {
    *(data + i) = EEPROM.read(address + i);
  }
}

void eepromSave(unsigned int address, void *value, size_t len) {
  uint8_t *data = value;

  Serial.print(F("Save: "));
  Serial.println(address, DEC);
  for(size_t i = 0; i<len; ++i) {
    //Serial.print(address + i, DEC);
    //Serial.print(F(", "));
    //Serial.println(*(data + i));
    EEPROM.write(address + i, *(data + i));
  }
}


