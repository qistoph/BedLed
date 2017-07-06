#include <avr/sleep.h>

void button_isr() {
  Serial.println(F("Waken from button."));
}

void deepSleep() {
  Serial.println(F("Going to sleep"));
  attachInterrupt(3, button_isr, LOW);
  attachInterrupt(4, button_isr, LOW);
  cli();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  cli();
  sleep_disable();
  detachInterrupt(3);
  detachInterrupt(4);
  sei();
  Serial.println(F("Awoken"));
}
