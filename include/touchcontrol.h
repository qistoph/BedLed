#ifndef __TOUCHCONTROL_H
#define __TOUCHCONTROL_H

#include <Arduino.h>

#define RELEASE_OFF 0
#define RELEASE_SAVE 1
#define RELEASE_NONE 2

void onTouch();

void onTouching();

void onTouchRelease();

void easterEgg();

void touchLoop();

#endif