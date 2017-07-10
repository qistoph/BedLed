#define MDT 150
#define MDH (3*MDT)
#define MLS MDH
#define MWS (7*MDT)

void easterEgg() {
  lightOff();
  wsp();
  dah();dit();dah();dit();dah(); // Starting signal
  wsp();
  dah();dit();dah();dit(); // C
  lsp();
  dit();dit();dit();dah();dah(); // 3
  lsp();
  dah();dah(); // M
  wsp();

  lightOn();
}

void dit() {
  lightOn();
  delay(MDT);
  lightOff();
  delay(MDT);
}

void dah() {
  lightOn();
  delay(MDH);
  lightOff();
  delay(MDT);
}

void lsp() {
  delay(MLS);
}

void wsp() {
  delay(MWS);
}
