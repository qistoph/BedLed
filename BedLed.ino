// NOTE!!!
// Set #undef USE_SOFTWARE_SERIAL in ATTinyCore\hardware\avr\1.1.2\variants\tinyX5\pins_arduino.h

// 60+190+60 = 310

// Fuse/Core config:
// Timer 1 Clock: 64MHz
// Clock: 1MHz internal

// LED_PIN must support PWM,
// ATtiny85: 0 and 1. Use core from https://github.com/SpenceKonde/ATTinyCore
// ATmega328: 3, 5, 6, 9, 10, and 11

//TODO:
// Check datasheet page 99 for required frequency

#define LED_PIN 1
#define BTN1_PIN 3
#define BTN2_PIN 4
#define RF_PIN 0
#define TX 2 // As used hardcoded by TinyDebugSerial

// Times are in ms
// DEBOUNCE_DELAY: time to allow jitter on a button before accepting its new state
// DIMM_TIMEOUT:   time to wait, after pressing a button, before starting to dimm
// DIMM_STEP_TIME: time between stepping to the next dimm level
#define DEBOUNCE_DELAY 20
#define DIMM_TIMEOUT 1000
#define DIMM_STEP_TIME 500

// Dimm settings
// dimmMin: minimum dimm level (as written to OCR1A
// dimmStep: increase per dimm step
// dimmMax: value at which the dimm level is reset to dimmMin
#define dimmMin 8
#define dimmStep 20
#define dimmMax 255

// End of configuration

#include <TinyDebugSerial.h> // 2886 -> 210

//#include <SoftwareSerial.h> // 4028 -> 1352
//#define RX 0
//SoftwareSerial Serial(RX, TX); // RX, TX

//#include "FakeSerial.h" // 2676
//FakeSerial Serial;

#include "NewKakuReceiver.h"

#ifndef SIGRD // used in boot.h by boot_signature_byte_get_short(addr)
#define SIGRD RSIG
#endif

bool deepSleepEnabled = false;

bool lightIsOn = false;

volatile uint8_t *_receivePortRegister;

void setup() {
  // put your setup code here, to run once:
  OSCCAL = 0x6F;
  Serial.begin(9600);

  //Serial.println(F("Running setup"));
  buttonsSetup();
  lightSetup();
  kakuSetup();

  deepSleepEnabled = true;
  //Serial.println(F("Enabling deep sleep"));

  // Setup interrupts
  *digitalPinToPCICR(RF_PIN) |= _BV(digitalPinToPCICRbit(RF_PIN)); // Enable PIN interrupts in general for RF (and buttons, because same register in ATTiny)
  *digitalPinToPCICR(BTN1_PIN) |= _BV(digitalPinToPCICRbit(BTN1_PIN)); //TODO: remove?
  *digitalPinToPCICR(BTN2_PIN) |= _BV(digitalPinToPCICRbit(BTN2_PIN)); //TODO: remove?

  *digitalPinToPCMSK(RF_PIN) |= _BV(digitalPinToPCMSKbit(RF_PIN)); // Enable interrupt on RF
  *digitalPinToPCMSK(BTN1_PIN) |= _BV(digitalPinToPCMSKbit(BTN1_PIN)); // Enable interrupt on button 1
  *digitalPinToPCMSK(BTN2_PIN) |= _BV(digitalPinToPCMSKbit(BTN2_PIN)); // Enable interrupt on button 2

  _receivePortRegister = portInputRegister(digitalPinToPort(RF_PIN));

  while(millis() < 200) { // Allow 0.2 seconds to enter kaku learning mode
    if(buttonsReadTouch()) {
      // If a button is pressed during start-up, enter KAKU-learning mode
      Serial.println(F("KAKU learning"));
      kakuLearningMode();
    }
  }

  Serial.println(F("Blink LED"));
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
byte volatile RFState =  0;
ISR(PCINT0_vect) {
  byte portstate= *_receivePortRegister & _BV(RF_PIN);
  if (RFState != portstate)
  {
    RFState = portstate;
    NewKaku_interruptHandler();
  }
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
      Serial.println(F("Touch"));
      onTouch();
    } else if(!stableTouch && wasTouching) {
      // Touch -> Release
      Serial.println(F("Release"));
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
      //Serial.println("EASTER EGG MODE!");
      easterEgg();
      startEe = false;
    }
  } else {
    eeStart = 0;
  }

  if(NewKaku.address) {
    kakuDumpNewKaku();

    if(kakuEepromCheck(NewKaku.address, NewKaku.unit)) {
      if(NewKaku.switchType == 1) {
        lightOn();
      } else if(NewKaku.switchType == 0) {
        lightOff();
      }
    }

    NewKaku.address = 0;
  }

//  Serial.print(F("lightIsOn: "));
//  Serial.println(lightIsOn);
//  Serial.print(F("stableTouch: "));
//  Serial.println(stableTouch);
//  Serial.print(F("deepSleepEnabled: "));
//  Serial.println(deepSleepEnabled);
  if(!lightIsOn && millis() > sleepAt && deepSleepEnabled) {
    deepSleep();
    sleepAt = millis() + 100; // Allow at least 100ms to detect button presses before sleeping again
  }
}
