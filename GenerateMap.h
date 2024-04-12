#pragma once
#include <fstream>
#include "ConsoleWindow.h"
#include "Snake.h"
#include "arts.h"
#include "MapTracking.h"

// Obstacle color
#define _obs_Color TXT_RGB

/*________________________________________________MAP 2___________________________________________________*/
// Initialize obstacle for level 3
void create_obstacle_2(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums);

// Generate level 2
void play_match2(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums);

/*________________________________________________MAP 3___________________________________________________*/
// Initialize obstacle for level 3
void create_obstacle_3(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums);

// Generate level 3
void play_match3(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums);

/*________________________________________________MAP 4___________________________________________________*/
// Initialize obstacle for level 3
void create_obstacle_4(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums, Point const_obs[], int& const_obs_nums);

// Make obstacle move
void move_obs(int x_pos, int y_pos, int height, int width, Point obs[], int obs_nums, bool& up, Point const_obs[], int& const_obs_nums);



/*________________________________________________TELEPORT___________________________________________________*/
// Initialize teleGate position
void create_teleGate();

// Generate teleGate in game interface
void draw_teleGate();


/*________________________________________________SHOW ART___________________________________________________*/
// show obstacle in game interface
void draw_obstacle(Point obs[], int obs_nums);

// show full art game interface
void draw_matchBoard(int x_pos, int y_pos, int height, int width);