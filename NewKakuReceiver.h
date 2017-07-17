#ifndef _NEWKAKURECEIVER_H_
#define _NEWKAKURECEIVER_H_

struct NewRemoteCode {
  unsigned int period;    // Detected duration in microseconds of 1T in the received signal
  unsigned long address;  // Address of received code. [0..2^26-1]
  boolean groupBit;   // Group bit set or not
  unsigned short switchType;  // 0: swich off; 1: switch on; 2: set dim level
  unsigned short unit;    // Unit code of received code [0..15]
  unsigned short dimLevel;  // Dim level [0..15] iff switchType == 2
};

struct NewRemoteCode NewKaku;

#endif
