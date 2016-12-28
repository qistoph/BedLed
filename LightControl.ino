#include <EEPROM.h>

#define dimmMin 1
#define dimmMax 255

#define EEPROM_ADDRESS_DIMM 0

byte dimmLevel = dimmMax;

void lightSetup() {
  pinMode(LED_PIN, OUTPUT);
  dimmLevel = EEPROM.read(EEPROM_ADDRESS_DIMM);
}

void lightSaveData() {
  EEPROM.write(EEPROM_ADDRESS_DIMM, dimmLevel);
}

void lightOn() {
  analogWrite(LED_PIN, dimmLevel);
}

void lightOff() {
  analogWrite(LED_PIN, 0);
}

void lightDimm() {
  dimmLevel = (dimmLevel - 1) % dimmMax;
  if(dimmLevel < dimmMin) {
    dimmLevel = dimmMax;
  }
  lightOn(); // Turn light 'on' to effectuate dimm
}
