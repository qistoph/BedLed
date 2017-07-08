#include <avr/sleep.h>

ISR(PCINT0_vect) {
  //Serial.println(F("PCINT0"));
}

void deepSleep() {
  Serial.print(F("Going to sleep at "));
  Serial.println(millis());

  GIMSK = _BV(PCIE);
  PCMSK = _BV(PCINT3) | _BV(PCINT4);
  
  //attachInterrupt(3, button_isr, LOW);
  //attachInterrupt(4, button_isr, LOW);
  cli();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  cli();
  sleep_disable();
  //detachInterrupt(3);
  //detachInterrupt(4);
  PCMSK = 0;
  sei();
  Serial.print(F("Awoken at "));
  Serial.println(millis());
}
