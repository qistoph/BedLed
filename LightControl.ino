#include <EEPROM.h>

#define dimmMin 1
#define dimmStep 20
#define dimmMax 255

#define EEPROM_ADDRESS_DIMM 0

int dimmLevel = dimmMax;

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
  //analogWrite(LED_PIN, dimmLevel);

  // LSM   - 1 - Low Speed Mode
  // [6:3] - R
  // PCKE  - 0 - Disable PCK (PLL as clock), wait for PLL Lock first
  // PLLE  - 1 - Enable PLL
  // PLOC  - R
  PLLCSR = _BV(LSM) | _BV(PLLE); // Start PLL, NO Low Speed Mode
  while (!(PLLCSR & (1<<PLOCK))); //Wait for PLL lock
  PLLCSR |= _BV(PCKE); // Use PLL as clock source

  // CTC1       -    1 - Reset timer after compare match with OCR1C
  // PWM1A      -    1 - Enable PWM mode
  // COM1A[1:0] -   01 - OC1A cleared on compare match, set on TCNT1 = $00, !OC1A set on compare, cleared on TCNT1 = $00
  // CS1[3:0]   - 0100 - PCK/8
  TCCR1 = _BV(CTC1) | _BV(PWM1A) | 0 | _BV(COM1A0) | 0 | _BV(CS12) | 0 | 0;
  OCR1C = dimmMax - 1;
  OCR1A = dimmLevel;
}

void lightOff() {
  //analogWrite(LED_PIN, 0);
  TCCR1 = 0; // Disable timer 1
}

void lightDimm() {
  if(dimmLevel == dimmMax) {
    dimmLevel = 1;
  } else {
    dimmLevel = dimmLevel * 2;
  }
  
  if(dimmLevel > dimmMax) {
    dimmLevel = dimmMax;
  }

  Serial.print("dim: ");
  Serial.println(dimmLevel);
  lightOn(); // Turn light 'on' to effectuate dimm
}
