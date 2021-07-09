#ifndef __LIGHTCONTROL_H
#define __LIGHTCONTROL_H

#include "settings.h"
#include "storage.h"

extern bool lightIsOn;
extern byte dimmLevelMin;
extern byte dimmLevelMax;

void lightSetup();

void lightSaveData();

void lightOn();

void lightOff();

void lightDimm();

void lightSetDimm(int dimm);

void lightBlink(uint8_t count);

#endif