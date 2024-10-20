#include "LedHandler.h"
#include "Debug.h"

#define RED_LED 9
#define LED_1 6
#define LED_2 7
#define LED_3 8
#define LED_4 10

void LedHandler_init() {
  pinMode(RED_LED, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  DEBUG_PRINTLN(F("LEDs initialized"));
}

void LedHandler_setLed(int ledIndex, int state) {
  int ledPins[] = {LED_1, LED_2, LED_3, LED_4};
  digitalWrite(ledPins[ledIndex], state);
  DEBUG_PRINT(F("LED "));
  DEBUG_PRINT(ledIndex + 1);
  DEBUG_PRINT(F(" set to "));
  DEBUG_PRINTLN(state);
}

void LedHandler_setAllLeds(int state) {
  digitalWrite(LED_1, state);
  digitalWrite(LED_2, state);
  digitalWrite(LED_3, state);
  digitalWrite(LED_4, state);
  DEBUG_PRINT(F("All LEDs set to "));
  DEBUG_PRINTLN(state);
}

void LedHandler_setRedLed(int state) {
  digitalWrite(RED_LED, state);
  DEBUG_PRINT(F("Red LED set to "));
  DEBUG_PRINTLN(state);
}

void LedHandler_fadeRedLed() {
  int brightness = (sin(millis() / 1000.0 * PI) + 1) * 127.5;
  analogWrite(RED_LED, brightness);
}