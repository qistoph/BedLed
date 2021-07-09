// NOTE!!!
// Uses:
// ATTinyCore (https://github.com/SpenceKonde/ATTinyCore)
// TinyDebugSerial (https://github.com/qistoph/TinyDebugSerial) + modified to use PB2 on F_CPU<=8MHz

// Fuse/Core config:
// Timer 1 Clock: 64MHz
// Clock: 8MHz internal

//TODO:
// Check datasheet page 99 for required frequency

#include <Arduino.h>
#include "settings.h"
#include "buttons.h"
#include "lightcontrol.h"
#include "touchcontrol.h"
#include "sleep.h"

#ifndef SIGRD // used in boot.h by boot_signature_byte_get_short(addr)
#define SIGRD RSIG
#endif

TinyDebugSerial TDSerial;

bool deepSleepEnabled = true;

void setup() {
  // put your setup code here, to run once:
  OSCCAL = 0x58;
  MySerial.begin(9600);

  Serial.println(F("Running setup"));
  buttonsSetup();
  lightSetup();

  // Setup interrupts
  *digitalPinToPCICR(BTN1_PIN) |= _BV(digitalPinToPCICRbit(BTN1_PIN)); // Enable PIN interrupts in general and btn1 pin
  *digitalPinToPCICR(BTN2_PIN) |= _BV(digitalPinToPCICRbit(BTN2_PIN)); //TODO: remove? (because same register in ATTiny)

  *digitalPinToPCMSK(BTN1_PIN) |= _BV(digitalPinToPCMSKbit(BTN1_PIN)); // Enable interrupt on button 1
  *digitalPinToPCMSK(BTN2_PIN) |= _BV(digitalPinToPCMSKbit(BTN2_PIN)); // Enable interrupt on button 2

  MySerial.println(F("Blink LED"));
  lightOn();
  delay(500);
  lightOff();
}

uint8_t wasTouching = false;
unsigned long loopStart = 0;
unsigned long sleepAt = 0;
unsigned long eeStart = 0;
bool startEe = false;

extern bool lightIsOn;

// Interrupt handler, called for button and RF interrupts
ISR(PCINT0_vect) {
}

void loop() {
  loopStart = millis();

  uint8_t stableTouch = buttonsReadTouch();

  if(stableTouch != wasTouching) {
    //Serial.print("stableTouch: ");
    //Serial.println(stableTouch);

    // state change
    if(stableTouch && !wasTouching) {
      // Release -> Touch
      MySerial.println(F("Touch"));
      onTouch();
    } else if(!stableTouch && wasTouching) {
      // Touch -> Release
      MySerial.println(F("Release"));
      onTouchRelease();
    }
    wasTouching = stableTouch;
  } else if(stableTouch) {
    // Still touching
    onTouching();
  }

  if(stableTouch == 3) {
    if(eeStart == 0) {
      eeStart = millis() + 2000;
      startEe = true;
    } else if(millis() > eeStart && startEe) {
      MySerial.println(F("EASTER EGG MODE!"));
      easterEgg();
      startEe = false;
    }
  } else {
    eeStart = 0;
  }

  touchLoop();

//  Serial.print(F("lightIsOn: "));
//  Serial.println(lightIsOn);
//  Serial.print(F("stableTouch: "));
//  Serial.println(stableTouch);
//  Serial.print(F("deepSleepEnabled: "));
//  Serial.println(deepSleepEnabled);
  if(!lightIsOn && !stableTouch && millis() > sleepAt && deepSleepEnabled) {
    deepSleep();
    sleepAt = millis() + 100; // Allow at least 100ms to detect button presses before sleeping again
  }
}
