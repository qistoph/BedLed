#ifndef __BUTTONS_H
#define __BUTTONS_H

#include <settings.h>

void buttonsSetup();

void updateButtonState(uint8_t pin, uint8_t index);

uint8_t buttonsReadTouch();

#endif