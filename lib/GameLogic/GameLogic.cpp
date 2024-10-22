#include "GameLogic.h"
#include "Debug.h"
#include <Arduino.h>

int correctAnswer[4];
int playerAnswer[4];
int randomNumber = 0;
unsigned long roundTime = 10000; // Initial round time: 10 seconds

void GameLogic_init() {
  for (int i = 0; i < 4; i++) {
    playerAnswer[i] = 0; // just to be sure
  }
  DEBUG_PRINTLN(F("Game logic initialized"));
}

void GameLogic_generateRandomNumber() {
  randomNumber = random(0, 16); // change to random seed ^
  for (int i = 0; i < 4; i++) {
    correctAnswer[i] = (randomNumber >> (3 - i)) & 1;
  }
  DEBUG_PRINT(F("Generated random number: "));
  DEBUG_PRINTLN(randomNumber);
}

bool GameLogic_checkAnswers() {
  for (int i = 0; i < 4; i++) {
    if (playerAnswer[i] != correctAnswer[i]) {
      //DEBUG_PRINTLN(F("Incorrect answer")); will continously loop
      return false;
    }
  }
  DEBUG_PRINTLN(F("Correct answer"));
  return true;
}

void GameLogic_updateTime(int difficulty) {
  float reductionFactor = 1.0 - (difficulty * 0.1);
  roundTime = max((unsigned long)(roundTime * reductionFactor), 2000UL);
  DEBUG_PRINT(F("Updated round time: "));
  DEBUG_PRINTLN(roundTime);
}

unsigned long GameLogic_getRoundTime() {
  return roundTime;
}

int GameLogic_getRandomNumber() {
  return randomNumber;
}

void GameLogic_resetPlayerAnswer() {
  for (int i = 0; i < 4; i++) {
    playerAnswer[i] = 0;
  }
  DEBUG_PRINTLN(F("Player answer reset"));
}