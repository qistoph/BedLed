#include "modes.h"

#include "lightcontrol.h"

void lightBreathe() {
  lightSetDimm((sin(sin(millis()/1000.0))+1)*125);
}

byte strobe_freq_hz = 25;

void lightStrobe() {
  lightSetDimm(
    (millis()/(1000/strobe_freq_hz)) % 2 == 1 ?
    dimmLevelMin :
    dimmLevelMax
  );
}

int horrorState = 0;
void lightHorror() {
  horrorState += random(-70, 70);
  horrorState = max(min(horrorState, 200), 10);
  lightSetDimm(horrorState);
  delay(50);
}