#pragma once
#include <windows.h>
#include <iostream>
#include <conio.h>
#include "Snake.h"
#include "graphics.h"

// macro for console background color & text color
#define BG_COLOR colorCode::LIGHT_CYAN
#define TXT_COLOR colorCode::LIGHT_YELLOW
#define BG_RGB rgb(249, 232, 217)
#define BG_RGB_2 {249, 232, 217}
#define TXT_RGB {82, 120, 83}
const long nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
const long nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

// coudn't use numeric_limits<T>::max() so manually set it here
constexpr long long STREAM_SIZE = 9223372036854775807LL;


struct axis //store terminal's axis
{
	int x;
	int y;
};


axis getTermSize(); // get terminal size

void text_color(int background_color, int text_color);

void GotoXY(int x, int y);

void DisableResizeWindow();
void DisableCtrButton(bool Close, bool Min, bool Max);
void ShowScrollbar(BOOL Show);
void SetConsolePosition(int x, int y);
void LockConsolePosition();
void SetWindowSize(SHORT width, SHORT height);
void SetScreenBufferSize(SHORT width, SHORT height);
void MaximizeConsoleWindow();
void ShowConsoleCursor(bool show); // show/unshow the cursor of the terminal
void LoadConsole();
unsigned int inputUnsignedNumber(int pos_x, int pos_y);

void changeColorSubScreen();
void settingScreen();
void mainMenu(); // main menu of the program