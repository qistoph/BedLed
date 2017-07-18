bool turnOffAtRelease = true;
bool lightIsOn = false;
unsigned long touchStartedAt = 0;

void onTouch() {
  touchStartedAt = millis();
  turnOffAtRelease = true;
  if(!lightIsOn) {
    turnOffAtRelease = false;
    lightOn();
    lightIsOn = true;
  }
}

void onTouching() {
  if(millis() > (touchStartedAt + DIMM_TIMEOUT)) {
    turnOffAtRelease = false;
    lightDimm();

    // Hack: set touchStartedAt so that after DIMM_STEP_TIME the DIMM_TIMEOUT is reached again
    // triggering this if() every DIMM_STEP_TIME ms
    touchStartedAt = millis() - (DIMM_TIMEOUT - DIMM_STEP_TIME);
  }
}

void onTouchRelease() {
  //Serial.print(F("turnOffAtRelease: "));
  //Serial.println(turnOffAtRelease);
  if(turnOffAtRelease) {
    lightOff();
    lightIsOn = false;
  } else {
    // turnOffAtRelease is false, because we were dimming
    // Save the current dimm level to EEPROM
    lightSaveData();
  }
}
