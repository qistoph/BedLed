#ifndef __TOUCHCONTROL_H
#define __TOUCHCONTROL_H

#include <Arduino.h>

#define RELEASE_CLICK 0
#define RELEASE_HOLD 2
#define RELEASE_SKIP 3

void onTouch();

void onTouching();

void onTouchRelease();

void easterEgg();

void touchLoop();

#endif