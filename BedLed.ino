// LED_PIN must support PWM,
// ATtiny85: 0 and 1. Use core from https://github.com/SpenceKonde/ATTinyCore
// ATmega328: 3, 5, 6, 9, 10, and 11
#define LED_PIN 0
#define CAP_SEND_PIN 3
#define CAP_SENS_PIN 4
#define PIN_UNUSED_1 1
#define TX 2 // As used hardcoded by TinyDebugSerial

#define CAP_SAMPLES 5
#define CAP_THRESHOLD 200

#define DIMM_TIMEOUT 1000

#include <TinyDebugSerial.h> // 3860 -> 196

//#include <SoftwareSerial.h> // 4904 -> 1240
//SoftwareSerial Serial(RX, TX); // RX, TX

//#include "FakeSerial.h" // 3664
//FakeSerial Serial;

void setup() {
  pinMode(PIN_UNUSED_1, INPUT_PULLUP);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  capsenseSetup();
  lightSetup();

  digitalWrite(LED_PIN, 1);
  delay(500);
  digitalWrite(LED_PIN, 0);
}

bool wasTouching = false;

void loop() {
  // put your main code here, to run repeatedly:
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

  delay(10); // Wait 10ms before sampling touch again
}
