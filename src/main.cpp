#include <Arduino.h>
#include <avr/sleep.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "ButtonHandler.h"
#include "LedHandler.h"
#include "GameLogic.h"
#include "Debug.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

enum GameState {
  INITIAL_STATE,
  SLEEP_MODE,
  ROUND_START,
  PLAYER_INPUT,
  SUCCESS_STATE,
  GAME_OVER
};

GameState currentState = INITIAL_STATE;
unsigned long stateStartTime = 0;
int difficulty = 1;
int score = 0;
const int potPin = A0;

void setState(GameState newState);
void handleSleepMode();

void setup() {
  Serial.begin(9600); // move to debug later
  randomSeed(analogRead(0)); 
  
  ButtonHandler_init();
  LedHandler_init();
  GameLogic_init();
  
  lcd.init();
  lcd.backlight();
  
  pinMode(potPin, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(2), []{ setState(INITIAL_STATE); }, FALLING);
  
  setState(INITIAL_STATE);
  DEBUG_PRINTLN(F("Game initialized"));
}

void loop() {
  switch (currentState) {
    case INITIAL_STATE:
      difficulty = map(analogRead(potPin), 0, 1023, 1, 4);
      LedHandler_fadeRedLed();
      
      if (millis() - stateStartTime > 10000) {
        setState(SLEEP_MODE);
      }
      if (ButtonHandler_isPressed(0)) {
        DEBUG_PRINTLN(F("Button B1 pressed"));
        // setState(ROUND_START);
      }
      break;

    case SLEEP_MODE:
      handleSleepMode();
      break;

    case ROUND_START:
      GameLogic_generateRandomNumber();
      setState(PLAYER_INPUT);
      break;

    case PLAYER_INPUT:
      ButtonHandler_checkButtons();
      if (GameLogic_checkAnswers()) {
        setState(SUCCESS_STATE);
      } else if (millis() - stateStartTime > GameLogic_getRoundTime()) {
        setState(GAME_OVER);
      }
      break;

    case SUCCESS_STATE:
      score++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Good job!");
      lcd.setCursor(0, 1);
      lcd.print("Score: ");
      lcd.print(score);
      
      GameLogic_updateTime(difficulty);
      
      if (millis() - stateStartTime > 5000) {
        setState(ROUND_START);
      }
      break;

    case GAME_OVER:
      LedHandler_setRedLed(HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Game over");
      lcd.setCursor(0, 1);
      lcd.print("Score: ");
      lcd.print(score);
      
      if (millis() - stateStartTime > 5000) {
        setState(INITIAL_STATE);
      }
      break;
  }
}

void setState(GameState newState) {
  currentState = newState;
  stateStartTime = millis();
  
  DEBUG_PRINT(F("State changed to: "));
  DEBUG_PRINTLN(newState);
  
  switch (newState) {
    case INITIAL_STATE:
      score = 0;
      LedHandler_setAllLeds(LOW);
      
      // display on lcd
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Welcome to GMB!");
      lcd.setCursor(0, 1);
      lcd.print("Press B1 to Start");


      break;
    case SLEEP_MODE:
      // Sleep mode is handled in the handleSleepMode function
      break;
    case ROUND_START:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("New round");
      lcd.setCursor(0, 1);
      lcd.print("Number: ");
      lcd.print(GameLogic_getRandomNumber());
      break;
    case PLAYER_INPUT:
      // Start timer is implicit in setting stateStartTime
      break;
    case SUCCESS_STATE:
      // Actions are handled in the main loop
      break;
    case GAME_OVER:
      // Actions are handled in the main loop
      break;
  }
}

void handleSleepMode() {
  DEBUG_PRINTLN(F("Entering sleep mode"));
  lcd.noBacklight();
  LedHandler_setAllLeds(LOW);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  
  // Execution resumes here after wake-up
  sleep_disable();
  lcd.backlight();
  DEBUG_PRINTLN(F("Waking up from sleep mode"));
}