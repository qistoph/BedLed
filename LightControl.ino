#include "Storage.h"

uint16_t dimmLevels[] = {8, 16, 32, 64, 128, 255, 0};
int dimmLevel = 8;

void lightSetup() {
  pinMode(LED_PIN, OUTPUT);
  eepromLoad(STORAGE_DIMM_ADDRESS, &dimmLevel, sizeof(dimmLevel));
  if(dimmLevel < dimmLevels[0]) {
    dimmLevel = dimmLevels[0];
  }
}

void lightSaveData() {
  eepromSave(STORAGE_DIMM_ADDRESS, &dimmLevel, sizeof(dimmLevel));
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

  OCR1C = 255 - 1;
  OCR1A = dimmLevel;

  // CTC1       -    1 - Reset timer after compare match with OCR1C (p.89)
  // PWM1A      -    1 - Enable PWM mode (p.89)
  // COM1A[1:0] -   01 - OC1A cleared on compare match, set when TCNT1 = $00. (!OC1A) set on compare match, cleared when TCNT1 = $00. (p.86)
  // CS1[3:0]   - 0010 - PCK/2 (p.88)
  TCCR1 = _BV(CTC1) | _BV(PWM1A) | 0 | _BV(COM1A0) | 0 | 0 | _BV(CS11) | 0;
  lightIsOn = true;
  MySerial.println(F("On"));
}

void lightOff() {
  TCCR1 = 0; // Disable timer 1
  lightIsOn = false;
  MySerial.println(F("Off"));
}

void lightDimm() {
  int n = 0;
  for(;;++n) {
    if(dimmLevels[n] == 0) {
      // No match
      dimmLevel = dimmLevels[0];
      break;
    } else if(dimmLevel < dimmLevels[n]) {
      dimmLevel = dimmLevels[n];
      break;
    }
  }
  
  MySerial.print(F("dim: "));
  MySerial.println(dimmLevel);
  lightOn(); // Turn light 'on' to effectuate dimm
}

void lightSetDimm(int dimm) {
  dimmLevel = dimm;
  lightOn(); // Turn light 'on' to effectuate dimm
}

void kakuDimm(unsigned short kakuDim) {
  kakuDim <<= 4;
  if(kakuDim == 0) {
    lightOff();
  } else {
    if(kakuDim > 240) {
      //Highest possible 4-bit value: full on
      kakuDim = 255;
    }
    dimmLevel = kakuDim;
    lightOn();
  }
}

void lightBlink(uint8_t count) {
  for(;count;) {
    lightOn();
    delay(250);
    lightOff();
    if(--count) {
      delay(250);
    }
  }
}
