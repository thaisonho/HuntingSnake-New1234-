#include "GameMatch.h"
#include "snake.h"
#include "graphics.h"
#include "arts.h"
#include "Variables.h"
#include "GenerateMap.h"

// change text inside color
void setTextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//	draw_matchBoard
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


void draw_obstacle(Point obs[], int obs_nums) {
	changeTextColor(_obs_Color);
	for (int i = 0; i < obs_nums; i++) {
		GotoXY(obs[i].x, obs[i].y);
		cout << u8"\u2588";
	}
	changeTextColor();
}

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


