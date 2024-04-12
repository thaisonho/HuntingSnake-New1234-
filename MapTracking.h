#pragma once
#include "ConsoleWindow.h"
#include "arts.h"
#include "graphics.h"
#include "GenerateMap.h"
#include "ProcessPlayer.h"
#include "ScoreList.h"
#include "Snake.h"
#include "Variables.h"



/*--------------Extract and get the color ai particular point from txt file----------------*/
void Extract2DPic(char chr[100][100], const string fileName);

RGBCOLOR ExtractColor(char chr[100][100], int x, int y, int& MAPP);


/*------------------------MAP TRACKING------------------------------*/
void EraseTrace(char chr[100][100], int& x, int& y, int& MAPP);
