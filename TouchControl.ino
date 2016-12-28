bool turnOffAtRelease = true;
bool lightIsOn = true;
unsigned long touchStartedAt = 0;

void onTouch() {
  touchStartedAt = millis();
  if(!lightIsOn) {
    lightOn();
  }
}

void onTouching() {
  if(millis() > (touchStartedAt + DIMM_TIMEOUT)) {
    turnOffAtRelease = false;
    lightDimm();
  }
}

void onTouchRelease() {
  if(turnOffAtRelease) {
    lightOff();
  } else {
    // turnOffAtRelease is false, because we were dimming
    // Save the current dimm level to EEPROM
    
  }
  turnOffAtRelease = lightIsOn;
}
