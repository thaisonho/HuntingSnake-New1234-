#pragma once

#include "Snake.h"
#include "Variables.h"

void NewGame(Player PlayerSnake[], int& id, string& namePlayer);

bool isValidName(Player PlayerSnake[], int id, std::string& st);
int LoadGameBoard(Player PlayerSnake[], int id, Player &LoadPlayer);
void LoadGamePlayer(Player PlayerSnake[], int id, Player &LoadPlayer);
void CleanDataFile(void);
void notValid(void);
void refill(void);