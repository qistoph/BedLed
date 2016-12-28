// LED_PIN must support PWM,
// ATtiny85: 0 and 1
// ATmega328: 3, 5, 6, 9, 10, and 11
#define LED_PIN 0
#define CAP_SEND_PIN 1
#define CAP_SENS_PIN 2
#define PIN_UNUSED_1 3
#define TX 4

#define CAP_SAMPLES 30
#define CAP_THRESHOLD 200

#define DIMM_TIMEOUT 1000

#include <TinyDebugSerial.h> // 3860 -> 196

//#include <SoftwareSerial.h> // 4904 -> 1240
//SoftwareSerial Serial(RX, TX); // RX, TX

//#include "FakeSerial.h" // 3664
//FakeSerial Serial;

void setup() {
  pinMode(PIN_UNUSED_1, INPUT_PULLUP);
  //pinMode(PIN_UNUSED_2, INPUT_PULLUP);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  capsenseSetup();
  lightSetup();
}

bool wasTouching = false;

void loop() {
  // put your main code here, to run repeatedly:
  bool stableTouch = capsenseReadTouch();

  if(stableTouch != wasTouching) {
    // stage change
    if(stableTouch) {
      // Release -> Touch
      Serial.print("Touch");
      onTouch();
    } else {
      // Touch -> Release
      Serial.print("Release");
      onTouchRelease();
    }
    wasTouching = stableTouch;
  } else if(stableTouch) {
    // Still touching
    onTouching();
  }

  delay(10); // Wait 10ms before sampling touch again
}
