#include "GenerateMap.h"
#include "Snake.h"
#include "arts.h";
#include "MapTracking.h"
#include "Variables.h"

// Declare telegate variables
PAIR TELEGATE[100];
int TELEPOS = 0;

/*-----------------------------------------PREPARE AND SHOW GAME INTERFACE------------------------------------------*/

/*________________________________________________MAP 2___________________________________________________*/
// Initialize obstacle for level 3
void create_obstacle_2(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums) {
	int dist = 10; // distance from topside to obs and obs to bottomside
	for (int i = x_pos + dist; i <= width - dist; i++) {

		// above obs
		obs[obs_nums].x = i;
		obs[obs_nums].y = y_pos + 6;
		obs_nums++;

		// under obs
		obs[obs_nums].x = i;
		obs[obs_nums].y = height - 1;
		obs_nums++;
	}
}

// Generate level 2
void play_match2(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums) {

	// prepare obstacle for level 2
	create_obstacle_2(x_pos, y_pos, height, width, obs, obs_nums);

	// draw match board
	draw_matchBoard(x_pos, y_pos, height, width);

	// draw obstacle for level 2
	draw_obstacle(obs, obs_nums);
}

/*________________________________________________MAP 3___________________________________________________*/
// Initialize obstacle for level 3
void create_obstacle_3(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums) {

	int dist = 10;
	for (int i = x_pos + dist; i <= width - dist; i++) {
		obs[obs_nums].x = i;
		obs[obs_nums].y = y_pos + 6;
		obs_nums++;

		obs[obs_nums].x = i;
		obs[obs_nums].y = height - 1;
		obs_nums++;
	}

	// add two obs in leftside and rightside
	for (int j = y_pos + 10; j <= height - 5; j++) {

		// leftside
		obs[obs_nums].x = x_pos + 2 * dist; // vat can dung cach 2 bien mot gia tri la 2*dist
		obs[obs_nums].y = j;
		obs_nums++;
		// rightside
		obs[obs_nums].x = width - 2 * dist;
		obs[obs_nums].y = j;
		obs_nums++;
	}

}

// Generate level 3
void play_match3(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums) {
	// prepare obstacle for level 3
	create_obstacle_3(x_pos, y_pos, height, width, obs, obs_nums);

	// draw matchboard
	draw_matchBoard(x_pos, y_pos, height, width);

	// draw obstacle for level 2
	draw_obstacle(obs, obs_nums);
}

/*________________________________________________MAP 4___________________________________________________*/
// Initialize obstacle for level 3
void create_obstacle_4(int x_pos, int y_pos, int height, int width, Point obs[], int& obs_nums, Point const_obs[], int& const_obs_nums)
{
	int obs_length = 10; // Khai bao do dai 2 thanh vat can di chuyen la 10 don vi
	// Khoi tao gia tri toa do thanh vat can di chuyen tu tren xuong
	for (int i = y_pos + 1; i <= y_pos + obs_length; i++)
	{
		obs[obs_nums].x = x_pos + 20;
		obs[obs_nums].y = i;
		obs_nums++;
	}
	// leftside
	for (int i = y_pos + height - obs_length - 1; i < y_pos + height - 1; i++)
	{
		obs[obs_nums].x = width - 20;
		obs[obs_nums].y = i;
		obs_nums++;
	}
	// rightside
	for (int i = x_pos + width / 2 - 10; i <= x_pos + width / 2 + 10; i++) {
		const_obs[const_obs_nums].x = i;
		const_obs[const_obs_nums].y = 16;
		const_obs_nums++;
	}

}

// Make obstacle move
void move_obs(int x_pos, int y_pos, int height, int width, Point obs[], int obs_nums, bool& up, Point const_obs[], int& const_obs_nums) {
	int x, y;
	draw_obstacle(const_obs, const_obs_nums); // Thanh thanh vat can co dinh nam ngang vao trong matchboard
	if (up)
	{
		// erase and redraw continuosly
		GotoXY(obs[obs_nums / 2 - 1].x, obs[obs_nums / 2 - 1].y);
		x = obs[obs_nums / 2 - 1].x, y = obs[obs_nums / 2 - 1].y;
		EraseTrace(ch, x, y, map);
		for (int i = 0; i < obs_nums / 2; i++)
		{
			obs[i].y--;
		}
		GotoXY(obs[0].x, obs[0].y);
		changeTextColor({ 0, 0, 0 }); cout << u8"\u2588"; // char(219)

		GotoXY(obs[obs_nums / 2].x, obs[obs_nums / 2].y);
		x = obs[obs_nums / 2].x, y = obs[obs_nums / 2].y;
		EraseTrace(ch, x, y, map);
		for (int i = obs_nums / 2; i < obs_nums; i++)
		{
			obs[i].y++;
		}
		GotoXY(obs[obs_nums - 1].x, obs[obs_nums - 1].y);
		changeTextColor({ 0, 0, 0 }); cout << u8"\u2588";

		if (obs[0].y == y_pos + 1)
		{
			up = false;
		}

	}
	else
	{
		GotoXY(obs[0].x, obs[0].y);
		x = obs[0].x, y = obs[0].y;
		EraseTrace(ch, x, y, map);
		for (int i = 0; i < obs_nums / 2; i++)
		{
			obs[i].y++;
		}

		GotoXY(obs[obs_nums / 2 - 1].x, obs[obs_nums / 2 - 1].y);
		changeTextColor({ 0, 0, 0 }); cout << u8"\u2588";

		GotoXY(obs[obs_nums - 1].x, obs[obs_nums - 1].y);
		x = obs[obs_nums - 1].x, y = obs[obs_nums - 1].y;
		EraseTrace(ch, x, y, map);
		for (int i = obs_nums / 2; i < obs_nums; i++)
		{
			obs[i].y--;
		}
		GotoXY(obs[obs_nums / 2].x, obs[obs_nums / 2].y);
		changeTextColor({ 0, 0, 0 }); cout << u8"\u2588";


		if (obs[obs_nums / 2 - 1].y == y_pos + height - 2)
		{
			up = true;
		}
	}
}



/*________________________________________________TELEPORT___________________________________________________*/
// Initialize teleGate position
void create_teleGate() {


	int distROW = 4;
	int distCOL = 2;

	for (int i = 40; i < 40 + distROW; i++) {


		TELEGATE[TELEPOS].in.x = i;
		TELEGATE[TELEPOS].in.y = 5;

		TELEGATE[TELEPOS].out.x = i;
		TELEGATE[TELEPOS].out.y = 28;
		TELEPOS++;
	}

	for (int i = 15; i < 15 + distCOL; i++) {


		TELEGATE[TELEPOS].in.x = 0;
		TELEGATE[TELEPOS].in.y = i;

		TELEGATE[TELEPOS].out.x = 81;
		TELEGATE[TELEPOS].out.y = i;
		TELEPOS++;
	}

}

// Generate teleGate in game interface
void draw_teleGate()
{
	SetConsoleOutputCP(CP_UTF8);

	changeTextColor({ 255, 0, 0 });
	for (int i = 0; i < TELEPOS; i++) {
		GotoXY(TELEGATE[i].in.x, TELEGATE[i].in.y);
		cout << u8"\u2588";
		GotoXY(TELEGATE[i].out.x, TELEGATE[i].out.y);
		cout << u8"\u2588";
	}
	changeTextColor();
}


/*________________________________________________SHOW ART___________________________________________________*/
// show obstacle in game interface
void draw_obstacle(Point obs[], int obs_nums) {
	changeTextColor(_obs_Color);
	for (int i = 0; i < obs_nums; i++) {
		GotoXY(obs[i].x, obs[i].y);
		cout << u8"\u2588";
	}
	changeTextColor();
}

// show full art game interface
void draw_matchBoard(int x_pos, int y_pos, int height, int width) {

	switch (map)
	{
	case 1:
	{
		drawMAP1(x_pos, y_pos, MAP1);
		break;
	}
	case 2:
	{
		drawMAP2(x_pos, y_pos, MAP2);
		break;
	}
	case 3:
	{
		drawMAP3(x_pos, y_pos, MAP3);
		break;
	}
	case 4:
	{
		drawMAP4(x_pos, y_pos, MAP4);
		break;
	}
	}

	draw_INFOR(width + 5, y_pos, infor_Board);

	switch (lev)
	{
	case 1:
		draw_level1();
		break;
	case 2:
		draw_level2();
		break;
	case 3:
		draw_level3();
		break;
	case 4:
		draw_level4();
		break;
	}
	create_teleGate();
	draw_teleGate();
}

