#define RELEASE_OFF 0
#define RELEASE_SAVE 1
#define RELEASE_NONE 2

byte releaseAction = RELEASE_OFF;
unsigned long touchStartedAt = 0;

void onTouch() {
  touchStartedAt = millis();
  releaseAction = RELEASE_OFF;
  if(!lightIsOn) {
    releaseAction = RELEASE_NONE;
    lightOn();
  }
}

void onTouching() {
  if(millis() > (touchStartedAt + DIMM_TIMEOUT)) {
    releaseAction = RELEASE_SAVE;
    lightDimm();

    // Hack: set touchStartedAt so that after DIMM_STEP_TIME the DIMM_TIMEOUT is reached again
    // triggering this if() every DIMM_STEP_TIME ms
    touchStartedAt = millis() - (DIMM_TIMEOUT - DIMM_STEP_TIME);
  }
}

void onTouchRelease() {
  MySerial.print(F("releaseAction: "));
  MySerial.print(releaseAction);

  switch(releaseAction) {
    case RELEASE_OFF:
      MySerial.println(F(" (RELEASE_OFF)"));
      lightOff();
      break;
    case RELEASE_SAVE:
      MySerial.println(F(" (RELEASE_SAVE)"));
      // Save the current dimm level to EEPROM
      lightSaveData();
      break;
    case RELEASE_NONE:
      MySerial.println(F(" (RELEASE_NONE)"));
      break;
    default:
      MySerial.println(F(" (Unknown)"));
      break;
  }
}
