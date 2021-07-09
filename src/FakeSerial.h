#ifndef FAKE_SERIAL_H
#define FAKE_SERIAL_H

#include <Stream.h>

class FakeSerial : public Stream {
  public:
    FakeSerial() {};

    void begin(int) {};

    virtual size_t write(uint8_t) {};
    virtual int available() {};
    virtual int read() {};
    virtual void flush() {};
    virtual int peek() {};
};

#endif
