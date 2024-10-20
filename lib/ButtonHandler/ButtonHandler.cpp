#include "ButtonHandler.h"
#include "LedHandler.h"
#include "Debug.h"
#include <Arduino.h>

#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define BUTTON_4 5

const unsigned long debounceDelay = 50;
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
int buttonState[4] = {HIGH, HIGH, HIGH, HIGH};
int lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};

extern int playerAnswer[4];

void ButtonHandler_init() {
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);
  DEBUG_PRINTLN(F("Buttons initialized"));
}

void ButtonHandler_checkButtons() {
  int buttonPins[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

  for (int i = 0; i < 4; i++) {
    int reading = digitalRead(buttonPins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;

        if (buttonState[i] == LOW) {
          playerAnswer[i] = !playerAnswer[i];
          LedHandler_setLed(i, playerAnswer[i]);
          DEBUG_PRINT(F("Button "));
          DEBUG_PRINT(i + 1);
          DEBUG_PRINT(F(" pressed. Player answer: "));
          DEBUG_PRINTLN(playerAnswer[i]);
        }
      }
    }

    lastButtonState[i] = reading;
  }
}

bool ButtonHandler_isPressed(int buttonIndex) {
  return buttonState[buttonIndex] == LOW;
}