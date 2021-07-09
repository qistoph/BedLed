#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "sleep.h"

void deepSleep() {
  //MySerial.print(F("Going to sleep at "));
  //MySerial.println(millis());

  cli();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  cli();
  sleep_disable();
  sei();
  //MySerial.print(F("Awoken at "));
  //MySerial.println(millis());
}
