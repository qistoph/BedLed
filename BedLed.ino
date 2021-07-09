// NOTE!!!
// Uses:
// ATTinyCore (https://github.com/SpenceKonde/ATTinyCore)
// TinyDebugSerial (https://github.com/qistoph/TinyDebugSerial) + modified to use PB2 on F_CPU<=8MHz

// Fuse/Core config:
// Timer 1 Clock: 64MHz
// Clock: 8MHz internal

//TODO:
// Check datasheet page 99 for required frequency

// LED_PIN must support PWM,
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

// End of configuration

#if 1 // 1 -> Use TinyDebugSerial, 0 -> Use core Serial (+428 bytes)
  #include <TinyDebugSerial.h>
  TinyDebugSerial TDSerial;
  #define MySerial TDSerial
#else
  #define MySerial Serial
#endif

#include "NewKakuReceiver.h"

#ifndef SIGRD // used in boot.h by boot_signature_byte_get_short(addr)
#define SIGRD RSIG
#endif

bool deepSleepEnabled = true;

bool lightIsOn = false;

volatile uint8_t *_receivePortRegister;

void setup() {
  // put your setup code here, to run once:
  OSCCAL = 0x6F;
  MySerial.begin(9600);

  Serial.println(F("Running setup"));
  buttonsSetup();
  lightSetup();
  kakuSetup();

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
      MySerial.println(F("KAKU learning"));
      kakuLearningMode();
    }
  }

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

  if(NewKaku.address) {
    kakuDumpNewKaku();

    if(kakuEepromCheck(NewKaku.address, NewKaku.unit)) {
      if(NewKaku.switchType == 1) {
        lightOn();
      } else if(NewKaku.switchType == 0) {
        lightOff();
      } else if(NewKaku.switchType == 2) {
        kakuDimm(NewKaku.dimLevel);
      }
    }

    NewKaku.address = 0;
  }

  touchLoop();

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

