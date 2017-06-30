void buttonsSetup() {
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
}

unsigned long lastDebounceTime[2] = {0, 0};
int buttonState[2] = {HIGH, HIGH};
int lastButtonState[2] = { HIGH, HIGH }; // Using internal pull-up, so default high

void updateButtonState(uint8_t pin, uint8_t index) {
  int reading = digitalRead(pin);
  if (reading != lastButtonState[index]) {
    lastDebounceTime[index] = millis();
  }

  if ((millis() - lastDebounceTime[index]) > DEBOUNCE_DELAY) {
    buttonState[index] = reading;
  }
}

uint8_t buttonsReadTouch() {
  updateButtonState(BTN1_PIN, 0);
  updateButtonState(BTN2_PIN, 1);

  // Using internal pull-up, so LOW means touch
  // 0 - Non pressed
  // 1 - BTN1 pressed
  // 2 - BTN2 pressed
  // 3 - BTN1 and BTN2 pressed
  return ((buttonState[0] == LOW) ? 1 : 0) | ((buttonState[1] == LOW) ? 2 : 0);
}
