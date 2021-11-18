#include "touchcontrol.h"
#include "lightcontrol.h"
#include "modes.h"
#include "quake.h"

byte releaseAction = RELEASE_CLICK;
unsigned long touchStartedAt = 0;

#define MODE_NORMAL 0
#define MODE_BRIGHT 1
#define MODE_BREATH 2
#define MODE_STROBE 3
#define MODE_HORROR 4
#define MODE_QUAKE  5
#define NUM_MODES   6

byte mode = MODE_NORMAL;

void onTouch() {
  touchStartedAt = millis();
  releaseAction = RELEASE_CLICK;

  randomSeed(analogRead(0));
}

void onTouching() {
  switch(mode) {
    case MODE_NORMAL:
    case MODE_BRIGHT:
      if(millis() > (touchStartedAt + DIMM_TIMEOUT)) {
        if(!lightIsOn) {
          MySerial.println(F("Dimmed on"));
          lightSetDimm(dimmLevelMin);
          lightOn();
        } else {
          MySerial.println(F("Dimming"));
          lightDimm();
        }
        
        releaseAction = RELEASE_HOLD;

        // Hack: set touchStartedAt so that after DIMM_STEP_TIME the DIMM_TIMEOUT is reached again
        // triggering this if() every DIMM_STEP_TIME ms
        touchStartedAt = millis() - (DIMM_TIMEOUT - DIMM_STEP_TIME);
      }
      break;
    case MODE_STROBE:
      if(millis() > (touchStartedAt + DIMM_TIMEOUT)) {
        if(strobe_freq_hz < 10) {
          strobe_freq_hz = 10;
        } else if(strobe_freq_hz < 20) {
          strobe_freq_hz = 20;
        } else if(strobe_freq_hz < 30) {
          strobe_freq_hz = 30;
        } else if (strobe_freq_hz < 40) {
          strobe_freq_hz = 40;
        } else {
          strobe_freq_hz = 5;
        }
        
        MySerial.print(F("Strobe freq (Hz): "));
        MySerial.println(strobe_freq_hz);

        releaseAction = RELEASE_SKIP;

        touchStartedAt = millis() - (DIMM_TIMEOUT - DIMM_STEP_TIME);
      }
      break;
    case MODE_QUAKE:
      if(millis() > (touchStartedAt + DIMM_TIMEOUT)) {
        uint8_t quake_mode = nextQuake();

        MySerial.print(F("Quak mode: "));
        MySerial.println(quake_mode);

        releaseAction = RELEASE_SKIP;
        touchStartedAt = millis() - (DIMM_TIMEOUT - DIMM_STEP_TIME);
      }
  }
}

void nextMode() {
  mode = (mode + 1) % NUM_MODES;
  MySerial.print(F("Mode: "));
  MySerial.println(mode);

  switch (mode)
  {
    case MODE_BRIGHT:
        lightSetDimm(dimmLevelMax);
      break;
  }
}

unsigned long lastClick = 0;

void onTouchRelease() {
  MySerial.print(F("releaseAction: "));
  MySerial.print(releaseAction);

  switch(releaseAction) {
    case RELEASE_CLICK:
      MySerial.println(F(" (RELEASE_CLICK)"));

      if (lightIsOn) {
        if (millis() - lastClick < DOUBLE_CLICK_TIME) {
          nextMode();
        } else {
          lightOff();
          mode = MODE_NORMAL;
        }
      } else {
        lightOn();
      }

      lastClick = millis();
      break;
    case RELEASE_HOLD:
      MySerial.println(F(" (RELEASE_HOLD)"));
      // Save the current dimm level to EEPROM
      lightSaveData();
      break;
    case RELEASE_SKIP:
      MySerial.println(F(" (RELEASE_SKIP)"));
      break;
    default:
      MySerial.println(F(" (Unknown)"));
      break;
  }
}

void easterEgg() {
  releaseAction = RELEASE_SKIP;
  mode = MODE_BREATH;
}

void touchLoop() {
  switch(mode) {
    case MODE_BREATH:
      lightBreathe();
      break;
    case MODE_STROBE:
      lightStrobe();
      break;
    case MODE_HORROR:
      lightHorror();
      break;
    case MODE_QUAKE:
      lightQuake();
      break;
  }
}
