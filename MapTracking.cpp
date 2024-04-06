#include "MapTracking.h"



void Extract2DPic(char chr[100][100], const string fileName)
{
	ifstream INP2(fileName + ".txt");
	if (!INP2.is_open()) {
		cout << "FILE ERROR !!" << endl;
		return;
	}
	for (int i = y_pos; i <= HEIGHT_BOARD + y_pos; i++)
		for (int j = x_pos; j <= WIDTH_BOARD; j++)
			INP2 >> chr[i][j];

	INP2.close();
}


void EraseTrace(char chr[100][100], int& x, int& y, int& MAPP)
{
	//ofstream OUT3("A.txt");
	//OUT3 << x << " " << y << " " << ch[y][x] << "\n";

	//SetConsoleOutputCP(CP_UTF8);

	GotoXY(x, y);

	if (MAPP == 1) {
		SetConsoleOutputCP(CP_UTF8);
		switch (chr[y][x])
		{
		case '.':
			changeTextColor({ 254, 195, 255 });
			break;
		case '+':
			changeTextColor({ 60, 91, 113 });
			break;
		case '-':
			changeTextColor({ 213, 174, 152 });
			break;
		case '*':
			changeTextColor({ 147, 92 ,61 });
			break;
		case '#':
			changeTextColor({ 39, 60, 74 });
			break;
		case 'o':
			changeTextColor({ 98, 141, 172 });
			break;
		case 'L':
			changeTextColor({ 62, 37, 15 });
			break;
		case '0':
			changeTextColor({ 124, 160, 186 });
			break;
		case 'T':
			changeTextColor({ 87, 133, 85 });
			break;
		case 'B':
			changeTextColor({ 0, 0, 0 });
			break;
		default:
			text_color(BG_COLOR, colorCode::WHITE);
			break;
		}

		if (chr[y][x] > 32) cout << u8"\u2588";
		else cout << chr[y][x];
		changeTextColor();
	}
	else if (MAPP == 2) {
		SetConsoleOutputCP(CP_UTF8);
		switch (chr[y][x])
		{
		case '2':
			changeTextColor({ 38, 61, 96 });
			break;
		case '1':
			changeTextColor({ 95, 83, 84 });
			break;
		case 'X':
			changeTextColor({ 48, 73, 47 });
			break;
		case 'B':
			changeTextColor({ 0, 0, 0 });
			break;
		case 'T':
			changeTextColor({ 177, 204, 232 });
			break;
		default:
			text_color(BG_COLOR, colorCode::WHITE);
			break;
		}
		if (chr[y][x] > 32) cout << u8"\u2588";
		else cout << chr[y][x];
		changeTextColor();
	}
	else if (MAPP == 3) {
		SetConsoleOutputCP(CP_UTF8);
		switch (chr[y][x])
		{
		case '.':
			changeTextColor({ 192, 225, 246 });
			break;
		case '+':
			changeTextColor({ 89, 54, 21 });
			break;
		case '-':
			changeTextColor({ 128, 210, 79 });
			break;
		case '*':
			changeTextColor({ 205, 123, 49 });
			break;
		case '#':
			changeTextColor({ 142, 85, 34 });
			break;
		case 'T':
			changeTextColor({ 173, 104, 41 });
			break;
		case 'L':
			changeTextColor({ 62, 37, 15 });
			break;
		case '1':
			changeTextColor({ 39, 74, 19 });
			break;
		case '2':
			changeTextColor({ 73, 137, 35 });
			break;
		case 'M':
			changeTextColor({ 170, 214, 243 });
			break;
		case 'I':
			changeTextColor({ 245, 245, 245 });
			break;
		case 'B':
			changeTextColor({ 0, 0, 0 });
			break;
		default:
			text_color(BG_COLOR, colorCode::WHITE);
			break;
		}
		if (chr[y][x] > 32) cout << u8"\u2588";
		else cout << chr[y][x];
		changeTextColor();
	}
	else if (MAPP == 4) {
		SetConsoleOutputCP(CP_UTF8);
		switch (chr[y][x])
		{
		case '+':
			changeTextColor({ 176, 110, 84 });
			break;
		case '-':
			changeTextColor({ 211, 140, 116 });
			break;
		case '*':
			changeTextColor({ 123, 69, 58 });
			break;
		case '.':
			changeTextColor({ 251, 213, 179 });
			break;
		case 'o':
			changeTextColor({ 123, 69, 58 });
			break;
		case 'W':
			changeTextColor({ 245, 245, 245 });
			break;
		case 'P':
			changeTextColor({ 210, 179, 158 });
			break;
		case 'T':
			changeTextColor({ 196, 117, 78 });
			break;
		case 'B':
			changeTextColor({ 0, 0, 0 });
			break;
		default:
			text_color(BG_COLOR, colorCode::WHITE);
			break;
		}
		if (chr[y][x] > 32) cout << u8"\u2588";
		else cout << chr[y][x];
		changeTextColor();
	}
}

RGBCOLOR ExtractColor(char chr[100][100], int x, int y, int& MAPP) {
	if (MAPP == 1) {
		switch (chr[y][x])
		{
		case '.':
			return { 254, 195, 255 };
		case '+':
			return { 60, 91, 113 };
		case '-':
			return { 213, 174, 152 };
		case '*':
			return { 147, 92 ,61 };
		case '#':
			return { 39, 60, 74 };
		case 'o':
			return { 98, 141, 172 };
		case 'L':
			return { 62, 37, 15 };
		case '0':
			return { 124, 160, 186 };
		case 'T':
			return { 87, 133, 85 };
		case 'B':
			return { 0, 0, 0 };
		}
		return { 245, 245, 245 };
	}
	else if (MAPP == 2) {
		switch (chr[y][x])
		{
		case '2':
			return { 38, 61, 96 };
		case '1':
			return { 95, 83, 84 };
		case 'X':
			return { 48, 73, 47 };
		case 'B':
			return { 0, 0, 0 };
		case 'T':
			return { 177, 204, 232 };
		}
		return { 245, 245, 245 };
	}
	else if (MAPP == 3) {
		switch (chr[y][x])
		{
		case '.':
			return { 192, 225, 246 };
		case '+':
			return { 89, 54, 21 };
		case '-':
			return { 128, 210, 79 };
		case '*':
			return { 205, 123, 49 };
		case '#':
			return { 142, 85, 34 };
		case 'T':
			return { 173, 104, 41 };
		case 'L':
			return { 62, 37, 15 };
		case '1':
			return { 39, 74, 19 };
		case '2':
			return { 73, 137, 35 };
		case 'M':
			return { 170, 214, 243 };
		case 'I':
			return { 245, 245, 245 };
		case 'B':
			return { 0, 0, 0 };
		}
		return { 245, 245, 245 };
	}
	else if (MAPP == 4) {
		switch (chr[y][x])
		{
		case '+':
			return { 176, 110, 84 };
		case '-':
			return { 211, 140, 116 };
		case '*':
			return { 123, 69, 58 };
		case '.':
			return { 251, 213, 179 };
		case 'o':
			return { 123, 69, 58 };
		case 'W':
			return { 245, 245, 245 };
		case 'P':
			return { 210, 179, 158 };
		case 'T':
			return { 196, 117, 78 };
		case 'B':
			return { 0, 0, 0 };
		}
		return { 245, 245, 245 };
	}
}