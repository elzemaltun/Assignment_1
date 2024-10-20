#ifndef LEDHANDLER_H
#define LEDHANDLER_H

#include <Arduino.h>

void LedHandler_init();
void LedHandler_setLed(int ledIndex, int state);
void LedHandler_setAllLeds(int state);
void LedHandler_setRedLed(int state);
void LedHandler_fadeRedLed();

#endif