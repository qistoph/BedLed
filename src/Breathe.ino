void lightBreathe() {
  lightSetDimm((sin(sin(millis()/1000.0))+1)*125);
}
