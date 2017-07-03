// NOTE!!!
// Set #undef USE_SOFTWARE_SERIAL in ATTinyCore\hardware\avr\1.1.2\variants\tinyX5\pins_arduino.h

// 60+190+60 = 310

// Fuse/Core config:
// Timer 1 Clock: 64MHz
// Clock: 8MHz internal

// LED_PIN must support PWM,
// ATtiny85: 0 and 1. Use core from https://github.com/SpenceKonde/ATTinyCore
// ATmega328: 3, 5, 6, 9, 10, and 11

//TODO:
// Check datasheet page 99 for required frequency

#define LED_PIN 1
#define BTN1_PIN 3
#define BTN2_PIN 4
#define PIN_UNUSED_1 0
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
#define dimmMin 1
#define dimmStep 20
#define dimmMax 255

// End of configuration

#include <TinyDebugSerial.h> // 2886 -> 210

//#include <SoftwareSerial.h> // 4028 -> 1352
//#define RX 0
//SoftwareSerial Serial(RX, TX); // RX, TX

//#include "FakeSerial.h" // 2676
//FakeSerial Serial;

void setup() {
  pinMode(PIN_UNUSED_1, INPUT_PULLUP);
  
  // put your setup code here, to run once:
  OSCCAL = 0x6A;
  Serial.begin(9600);
  Serial.println("PWM frequency, register check - pre setup:");
  Serial.println("TODO: Fuses..."); // http://www.engbedded.com/fusecalc/
  Serial.print("PLLCSR: "); Serial.println(PLLCSR, HEX);
  Serial.print("TCCR1: "); Serial.println(TCCR1, HEX);
  Serial.print("OCR1C: "); Serial.println(OCR1C, HEX);
  Serial.print("OCR1A: "); Serial.println(OCR1A, HEX);

  Serial.println("Running setup");
  buttonsSetup();
  lightSetup();

  Serial.println("PWM frequency, register check - setup/lightOn:");
  Serial.print("PLLCSR: "); Serial.println(PLLCSR, HEX);
  Serial.print("TCCR1: "); Serial.println(TCCR1, HEX);
  Serial.print("OCR1C: "); Serial.println(OCR1C, HEX);
  Serial.print("OCR1A: "); Serial.println(OCR1A, HEX);

  Serial.println("Blink LED");
  lightOn();
  Serial.println("PWM frequency, register check - lightOn/lightOff:");
  Serial.print("PLLCSR: "); Serial.println(PLLCSR, HEX);
  Serial.print("TCCR1: "); Serial.println(TCCR1, HEX);
  Serial.print("OCR1C: "); Serial.println(OCR1C, HEX);
  Serial.print("OCR1A: "); Serial.println(OCR1A, HEX);

  delay(500);
  lightOff();

  Serial.println("PWM frequency, register check - post lightOff:");
  Serial.print("PLLCSR: "); Serial.println(PLLCSR, HEX);
  Serial.print("TCCR1: "); Serial.println(TCCR1, HEX);
  Serial.print("OCR1C: "); Serial.println(OCR1C, HEX);
  Serial.print("OCR1A: "); Serial.println(OCR1A, HEX);
}

uint8_t wasTouching = false;
unsigned long loopStart = 0;
unsigned long now = 0;

void loop() {
  loopStart = millis();

  uint8_t stableTouch = buttonsReadTouch();

  if(stableTouch != wasTouching) {
    Serial.print("stableTouch: ");
    Serial.println(stableTouch);

    // state change
    if(stableTouch == 3) {
      Serial.println("EASTER EGG MODE!");
      easterEgg();
    } else if(stableTouch && !wasTouching) {
      // Release -> Touch
      Serial.println("Touch");
      onTouch();
    } else if(!stableTouch && wasTouching) {
      // Touch -> Release
      Serial.println("Release");
      onTouchRelease();
    }
    wasTouching = stableTouch;
  } else if(stableTouch) {
    // Still touching
    onTouching();
  }
}
