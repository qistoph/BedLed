#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <Arduino.h>

// LED_PIN must support PWM,
#define LED_PIN 1
#define BTN1_PIN 3
#define BTN2_PIN 4
#define TX 2 // As used hardcoded by TinyDebugSerial

// Times are in ms
// DEBOUNCE_DELAY: time to allow jitter on a button before accepting its new state
// DIMM_TIMEOUT:   time to wait, after pressing a button, before starting to dimm
// DIMM_STEP_TIME: time between stepping to the next dimm level
#define DEBOUNCE_DELAY 20
#define DIMM_TIMEOUT 1000
#define DIMM_STEP_TIME 500

#if 1 // 1 -> Use TinyDebugSerial, 0 -> Use core Serial (+428 bytes)
  #include <TinyDebugSerial.h>
  extern TinyDebugSerial TDSerial;
  #define MySerial TDSerial
#else
  #define MySerial Serial
#endif

#endif