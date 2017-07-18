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

  MySerial.print(F("Save: "));
  MySerial.println(address, DEC);
  for(size_t i = 0; i<len; ++i) {
    //MySerial.print(address + i, DEC);
    //MySerial.print(F(", "));
    //MySerial.println(*(data + i));
    EEPROM.write(address + i, *(data + i));
  }
}


