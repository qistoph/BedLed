#include <CapacitiveSensor.h>

CapacitiveSensor capSens = CapacitiveSensor(CAP_SEND_PIN, CAP_SENS_PIN);

void capsenseSetup() {
  //capSens.set_CS_AutocaL_Millis(0xFFFFFFFF);
}

#define HISTORY 5
long lastReadings[HISTORY];
int lastIndex = 0;

bool capsenseReadTouch() {
  long start = millis();
  long reading =  capSens.capacitiveSensor(CAP_SAMPLES);

  Serial.print(millis() - start);
  Serial.print("\t");
  Serial.print(reading);
  Serial.print("\t");

  lastReadings[lastIndex] = reading;
  lastIndex = (lastIndex + 1) % HISTORY;
  
  Serial.print(lastIndex);
  Serial.print("\t");

  long total = 0;
  for(int i=0; i<HISTORY; ++i) {
    total += lastReadings[i];
  }
  Serial.println(total);

  return (total > CAP_THRESHOLD);
}

