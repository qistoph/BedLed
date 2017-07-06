#include <EEPROM.h>

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
  //TODO: move setup to lightSetup
  // LSM   - 0 - Low Speed Mode
  // [6:3] - R
  // PCKE  - 0 - Disable PCK (PLL as clock), wait for PLL Lock first
  // PLLE  - 1 - Enable PLL
  // PLOC  - R
  PLLCSR = _BV(PLLE); // Start PLL, No Low Speed Mode
  delayMicroseconds(100);
  while (!(PLLCSR & (1<<PLOCK))); //Wait for PLL lock
  PLLCSR |= _BV(PCKE); // Use PLL as clock source

  OCR1C = dimmMax - 1;
  OCR1A = dimmLevel;

  // CTC1       -    1 - Reset timer after compare match with OCR1C (p.89)
  // PWM1A      -    1 - Enable PWM mode (p.89)
  // COM1A[1:0] -   01 - OC1A cleared on compare match, set when TCNT1 = $00. (!OC1A) set on compare match, cleared when TCNT1 = $00. (p.86)
  // CS1[3:0]   - 0010 - PCK/2 (p.88)
  TCCR1 = _BV(CTC1) | _BV(PWM1A) | 0 | _BV(COM1A0) | 0 | 0 | _BV(CS11) | 0;
}

void lightOff() {
  TCCR1 = 0; // Disable timer 1
}

void lightDimm() {
  if(dimmLevel >= dimmMax) {
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
