#pragma once
#include "ConsoleWindow.h"
#include <iostream>
#include <string>
#define rgb(x, y, z) RGB((x), (y), (z))

enum colorCode
{
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_CYAN,
    DARK_RED,
    DARK_MAGENTA,
    DARK_YELLOW,
    LIGHT_GRAY,
    DARK_GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_MAGENTA,
    LIGHT_YELLOW,
    WHITE
};

typedef struct {
    unsigned int red, green, blue;
} RGBCOLOR;

// set background color for the terminal
void setBackgroundColor(COLORREF color);
// reset to normal color
inline void changeTextColor();
// change only foreground color
inline void changeTextColor(RGBCOLOR _fg);
// change both foreground & background color
inline void changeTextColor(RGBCOLOR _fg, RGBCOLOR _bg);

// to draw a rectangle with user-choice color, text, text color
void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color);
void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, std::string text, RGBCOLOR txtColor);
void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, RGBCOLOR _bg_text);
void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, std::string text, RGBCOLOR txtColor, RGBCOLOR _bg_text);
void filled_rec(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color);
void filled_rec(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, std::string text, RGBCOLOR txtColor);


// to higlight the button in rectangle shape, user can choose color base on the need
void highlightedBox(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color);

void draw_level1();

void draw_level2();

void draw_level3();

void draw_level4();

void draw_huntingSnake();

void pause_game();

void draw_ButtonW();

void draw_ButtonA();

void draw_ButtonS();

void draw_ButtonD();

void TextToAsciiArt(int pos_x, int pos_y, std::string text, RGBCOLOR fg_color, RGBCOLOR bg_color);
int AsciiArtTextSize(std::string text);