// NOTE!!!
// Set #undef USE_SOFTWARE_SERIAL in ATTinyCore\hardware\avr\1.1.2\variants\tinyX5\pins_arduino.h

// LED_PIN must support PWM,
// ATtiny85: 0 and 1. Use core from https://github.com/SpenceKonde/ATTinyCore
// ATmega328: 3, 5, 6, 9, 10, and 11

//TODO:
// Check datasheet page 99 for required frequency

#define LED_PIN 1
#define CAP_SEND_PIN 3
#define CAP_SENS_PIN 4
#define PIN_UNUSED_1 0
#define TX 2 // As used hardcoded by TinyDebugSerial

#define CAP_SAMPLES 5
#define CAP_THRESHOLD 200

#define DIMM_TIMEOUT 1000
#define DIMM_STEP_TIME 500

#include <TinyDebugSerial.h> // 3860 -> 196

//#include <SoftwareSerial.h> // 4904 -> 1240
//SoftwareSerial Serial(RX, TX); // RX, TX

//#include "FakeSerial.h" // 3664
//FakeSerial Serial;

void setup() {
  pinMode(PIN_UNUSED_1, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  capsenseSetup();
  lightSetup();

  digitalWrite(LED_PIN, 1);
  delay(500);
  digitalWrite(LED_PIN, 0);
}

bool wasTouching = false;
unsigned long loopStart = 0;
unsigned long now = 0;

void loop() {
  loopStart = millis();

  bool stableTouch = capsenseReadTouch();

  if(stableTouch != wasTouching) {
    // state change
    if(stableTouch) {
      // Release -> Touch
      Serial.println("Touch");
      onTouch();
    } else {
      // Touch -> Release
      Serial.println("Release");
      onTouchRelease();
    }
    wasTouching = stableTouch;
  } else if(stableTouch) {
    // Still touching
    onTouching();
  }

  now = millis();
  if(now - loopStart < 50) {
    // If handling took less than 50ms
    // Delay additional time to have constant time loop
    delay( 50 - (now - loopStart) );
  }
}
