#pragma once
#include "ConsoleWindow.h"
#include "Snake.h"
#include <iostream>

#define _obs_Color {0, 0, 0}

using namespace std;


void setTextColor(int color); // change text color inside box

void draw_matchBoard(int x_pos, int y_pos, int height, int width);

//void draw_infoBoard(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, int txtColor, int bg_color, int line_color, std::string text);

void draw_obstacle(Point obs[], int obs_nums); // draw obstacle from obstacle initialized

void draw_teleGate(void);
//void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, int line_color, int bg_color, std::string text, int txtColor)
