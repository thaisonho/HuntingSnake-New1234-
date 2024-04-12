#pragma once
#include <fstream>
#include <iostream>
#include "Snake.h"
#include "Variables.h"

bool isValidName(Player PlayerSnake[], int id, std::string& st);
int LoadGameBoard(Player PlayerSnake[], int id, Player& LoadPlayer);
void LoadGamePlayer(Player PlayerSnake[], int id, Player& LoadPlayer);
void CleanDataFile(void);
void notValid(void);
void refill(void);

void NewGame(Player PlayerSnake[], int& id, string& namePlayer);