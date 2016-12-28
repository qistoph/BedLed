#include <CapacitiveSensor.h>

CapacitiveSensor capSens = CapacitiveSensor(CAP_SEND_PIN, CAP_SENS_PIN);

void capsenseSetup() {
  //capSens.set_CS_AutocaL_Millis(0xFFFFFFFF);
}

bool capsenseReadTouch() {
  long start = millis();
  long total =  capSens.capacitiveSensor(CAP_SAMPLES);
  
  Serial.print(millis() - start);
  Serial.print("\t");
  Serial.println(total);

  return (total > CAP_THRESHOLD);
}

