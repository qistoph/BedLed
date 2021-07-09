#include "touchcontrol.h"
#include "lightcontrol.h"
#include "modes.h"

byte releaseAction = RELEASE_CLICK;
unsigned long touchStartedAt = 0;
bool easterEggActive = 0;

void onTouch() {
  touchStartedAt = millis();
  easterEggActive = 0;
  releaseAction = RELEASE_CLICK;
}

void onTouching() {
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
}

void onTouchRelease() {
  MySerial.print(F("releaseAction: "));
  MySerial.print(releaseAction);

  switch(releaseAction) {
    case RELEASE_CLICK:
      MySerial.println(F(" (RELEASE_CLICK)"));
      if (lightIsOn) {
        lightOff();
      } else {
        lightOn();
      }
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
  easterEggActive = 1;
}

void touchLoop() {
  if(easterEggActive) {
    lightBreathe();
  }
}
