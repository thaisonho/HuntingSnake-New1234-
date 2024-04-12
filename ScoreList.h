#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include "ProcessPlayer.h"
#include "arts.h"
#include "Snake.h"
#include "ConsoleWindow.h"

using namespace std;

struct ScoreList
{
	int size;
	int score;

	string name;
};

void swap(ScoreList& a, ScoreList& b);
void SortScore(ScoreList S[], int id);
void ProcessHighScore();
void ScoreBoard(ScoreList S[], Player PlayerSnake[], int id);