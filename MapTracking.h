#include "ConsoleWindow.h"
#include "arts.h"
#include "GameMatch.h"
#include "GenerateMap.h"
#include "graphics.h"
#include "ProcessPlayer.h"
#include "ScoreList.h"
#include "Snake.h"
#include "Variables.h"


//extern int map = 3;


void Extract2DPic(char ch[100][100], std::string pic);

void EraseTrace(char ch[100][100], int& x, int& y, int& map);

RGBCOLOR ExtractColor(char ch[100][100], int x, int y, int& map);