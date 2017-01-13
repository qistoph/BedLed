bool turnOffAtRelease = true;
bool lightIsOn = true;
unsigned long touchStartedAt = 0;

void onTouch() {
  touchStartedAt = millis();
  if(!lightIsOn) {
    lightOn();
    lightIsOn = true;
  }
}

void onTouching() {
  if(millis() > (touchStartedAt + DIMM_TIMEOUT)) {
    turnOffAtRelease = false;
    lightDimm();

    // Hack: set touchStartedAt to that after DIMM_STEP_TIME the DIMM_TIMEOUT is reached again
    touchStartedAt = millis() - (DIMM_TIMEOUT - DIMM_STEP_TIME);
  }
}

void onTouchRelease() {
  if(turnOffAtRelease) {
    lightOff();
    lightIsOn = false;
  } else {
    // turnOffAtRelease is false, because we were dimming
    // Save the current dimm level to EEPROM
    lightSaveData();
  }
  turnOffAtRelease = lightIsOn;
}
