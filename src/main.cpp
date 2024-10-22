/*
Assignment No 1 
Group 14

Group Members: 
1. Keith Barnes
2. Sehen Elzem Altun
3. Mary Anne Selirio
*/
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
void wakeUp();

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(1)); // use an open pin to seed the random number
  
  ButtonHandler_init();
  LedHandler_init();
  GameLogic_init();
  
  lcd.init();
  lcd.backlight();
  
  pinMode(potPin, INPUT); // initializing the potentiometer
  
  // attaching interrupts to hardware interrupt pins
  attachInterrupt(digitalPinToInterrupt(2), wakeUp, FALLING); 
  attachInterrupt(digitalPinToInterrupt(3), wakeUp, FALLING);
  
  DEBUG_PRINTLN(F("Game initialized"));

  setState(INITIAL_STATE); // goto initial state
}

void loop() {
  switch (currentState) {
    case INITIAL_STATE:
      difficulty = map(analogRead(potPin), 0, 1023, 1, 4);
      LedHandler_fadeRedLed();

      ButtonHandler_checkButtons();
      
      lcd.setCursor(0, 1);
      lcd.print(F("Difficulty: "));
      lcd.print(difficulty);
      
      if (millis() - stateStartTime > 10000) {
        DEBUG_PRINTLN(F("GOING TO SLEEP"));
        setState(SLEEP_MODE);
      }
      if (ButtonHandler_isPressed(0)) {
        DEBUG_PRINTLN(F("NEXT ROUND"));
        setState(ROUND_START);
      }
      break;

    case SLEEP_MODE:
      handleSleepMode();
      break;

    case ROUND_START:
      //GameLogic_generateRandomNumber(); // added in set state
      //GameLogic_resetPlayerAnswer();
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
      // display the message for 3 secs
      if (millis() - stateStartTime > 3000) {
        setState(ROUND_START);
      }
      break;

    case GAME_OVER:
      // red led has to be on for 1 sec
      if (millis() - stateStartTime > 1000) {
        LedHandler_setRedLed(LOW);
      }
      // display the message for 3 secs
      if (millis() - stateStartTime > 3000) {
        setState(INITIAL_STATE);
      }
      break;
  }
}

void setState(GameState newState) {
  currentState = newState;
  stateStartTime = millis();
  
  DEBUG_PRINT(F("State changed to: "));
  DEBUG_PRINT(static_cast<int>(newState));
  DEBUG_PRINT(F(" : "));
  switch (newState) {
    case INITIAL_STATE:
      DEBUG_PRINTLN(F("INITIAL"));

      score = 0; // reset score
      LedHandler_setAllLeds(LOW);
      LedHandler_setRedLed(LOW);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Welcome to GMB!"));
      delay(2000); // keep this message for 2 secs
      //lcd.setCursor(0, 1);
      //lcd.print(F("Press B1 to Start"));

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Press B1 to Start"));

      break;
    case SLEEP_MODE:
      DEBUG_PRINTLN(F("SLEEP_MODE"));
      break;
    case ROUND_START:
      DEBUG_PRINTLN(F("ROUND_START")); 

      LedHandler_setRedLed(LOW);

      ButtonHandler_reset(); // Reset all button inputs and player answers
      //GameLogic_resetPlayerAnswer(); // reset answer just to be sure

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("New round"));
      lcd.setCursor(0, 1);
      lcd.print(F("Number: "));

      GameLogic_generateRandomNumber(); // generate a random number & compute correct answer

      lcd.print(GameLogic_getRandomNumber()); // returns the random number
      break;
    case PLAYER_INPUT:
      DEBUG_PRINTLN(F("PLAYER_INPUT"));
      break;
    case SUCCESS_STATE:
      DEBUG_PRINTLN(F("SUCCESS_STATE"));

      score++; // increase score

      // update lcd
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Good job!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Score: "));
      lcd.print(score);
      
      GameLogic_updateTime(difficulty); // update the time acc to diff
      break;
    case GAME_OVER:
      DEBUG_PRINTLN(F("GAME_OVER"));

      LedHandler_setRedLed(HIGH);

      // display the score 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F("Game over"));
      lcd.setCursor(0, 1);
      lcd.print(F("Score: "));
      lcd.print(score);
      break;
  }
}

void handleSleepMode() {
  DEBUG_PRINTLN(F("Entering sleep mode"));
  lcd.noBacklight();
  lcd.clear();

  LedHandler_setAllLeds(LOW);
  LedHandler_setRedLed(LOW);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  
  // Execution resumes here after wake-up
  sleep_disable();
  lcd.backlight();
  DEBUG_PRINTLN(F("Waking up from sleep mode"));
  setState(INITIAL_STATE);
}

void wakeUp() {
  // This function will be called when an interrupt occurs
  // We don't need to do anything here, as the main loop will handle the state change
}