#ifndef GAMELOGIC_H
#define GAMELOGIC_H

void GameLogic_init();
void GameLogic_generateRandomNumber();
bool GameLogic_checkAnswers();
void GameLogic_updateTime(int difficulty);
unsigned long GameLogic_getRoundTime();
int GameLogic_getRandomNumber();

#endif