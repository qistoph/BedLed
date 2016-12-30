#include <EEPROM.h>

#define dimmMin 1
#define dimmStep 20
#define dimmMax 255

#define EEPROM_ADDRESS_DIMM 0

byte dimmLevel = dimmMax;

void lightSetup() {
  pinMode(LED_PIN, OUTPUT);
  dimmLevel = EEPROM.read(EEPROM_ADDRESS_DIMM);
  if(dimmLevel < dimmMin) {
    dimmLevel = dimmMin;
  }
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
  dimmLevel = (dimmLevel - dimmStep) % dimmMax;
  if(dimmLevel < dimmMin) {
    dimmLevel = dimmMax;
  }
  Serial.print("dim: ");
  Serial.println(dimmLevel);
  lightOn(); // Turn light 'on' to effectuate dimm
}
