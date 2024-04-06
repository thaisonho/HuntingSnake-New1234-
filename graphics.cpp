#include "graphics.h"

void setBackgroundColor(COLORREF color)
{
	CONSOLE_SCREEN_BUFFER_INFOEX sbInfoEx;
	sbInfoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	//sbInfoEx.dwSize.X = 120; // Set console width to 120 columns
	//sbInfoEx.dwSize.Y = 29;

	HANDLE consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(consoleOut, &sbInfoEx);

	sbInfoEx.dwSize.X = 120; // Set console width to 120 columns
	sbInfoEx.dwSize.Y = 29;

	COLORREF storedBG = sbInfoEx.ColorTable[0];
	//-- set BG color
	sbInfoEx.ColorTable[0] = color;
	SetConsoleScreenBufferInfoEx(consoleOut, &sbInfoEx);
}

inline void changeTextColor()
{
	std::cout << "\x1b[0m";
}

inline void changeTextColor(RGBCOLOR _fg)
{
	printf("\x1b[38;2;%u;%u;%um", _fg.red, _fg.green, _fg.blue);
}

inline void changeTextColor(RGBCOLOR _fg, RGBCOLOR _bg)
{
	printf("\x1b[38;2;%u;%u;%u;48;2;%u;%u;%um", _fg.red, _fg.green, _fg.blue, _bg.red, _bg.green, _bg.blue);
}

void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color)
{
	SetConsoleOutputCP(CP_UTF8);
	changeTextColor(color);
	for (int ix = x_pos + 1; ix < x_pos + width; ++ix) {
		GotoXY(ix, y_pos);
		std::cout << u8"\u2550"; //char(205)
		GotoXY(ix, y_pos + height);
		std::cout << u8"\u2550"; // char(205);
	}
	GotoXY(x_pos, y_pos);
	for (int iy = y_pos + 1; iy < y_pos + height; ++iy) {
		GotoXY(x_pos, iy);
		std::cout << u8"\u2551"; // char(186);
		GotoXY(x_pos + width, iy);
		std::cout << u8"\u2551"; // char(186);
	}
	// draw 4 corners
	GotoXY(x_pos, y_pos);
	std::cout << u8"\u2554"; // char(201);
	GotoXY(x_pos + width, y_pos);
	std::cout << u8"\u2557"; // char(187);
	GotoXY(x_pos, y_pos + height);
	std::cout << u8"\u255A"; // char(200);
	GotoXY(x_pos + width, y_pos + height);
	std::cout << u8"\u255D"; // char(188);
	changeTextColor(color);
}

void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, std::string text, RGBCOLOR txtColor)
{
	SetConsoleOutputCP(CP_UTF8);
	draw_rectangle(x_pos, y_pos, height, width, color);

	changeTextColor(txtColor);
	GotoXY(x_pos + (width - text.size()) / 2, y_pos + height / 2);
	std::cout << text;
	changeTextColor();
}

void filled_rec(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color)
{
	SetConsoleOutputCP(CP_UTF8);
	changeTextColor(color);
	for (int iy = y_pos; iy <= y_pos + height; ++iy) {
		for (int ix = x_pos; ix <= x_pos + width; ++ix) {
			GotoXY(ix, iy);
			std::cout << u8"\u2588"; // char(219);
		}
	}
	changeTextColor();
}

void filled_rec(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, std::string text, RGBCOLOR txtColor)
{
	filled_rec(x_pos, y_pos, height, width, color);

	changeTextColor(txtColor, color);
	GotoXY(x_pos + (width - text.size()) / 2, y_pos + height / 2);
	std::cout << text;
	GotoXY(x_pos + width + 1, y_pos);
	changeTextColor();
}

void highlightedBox(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color)
{
	SetConsoleOutputCP(CP_UTF8);
	changeTextColor(color);
	for (int ix = x_pos; ix <= x_pos + width; ++ix) {
		GotoXY(ix, y_pos);
		std::cout << u8"\u2588"; // char(219);
		GotoXY(ix, y_pos + height);
		std::cout << u8"\u2588"; // char(219);
	}
	GotoXY(x_pos, y_pos);
	for (int iy = y_pos; iy <= y_pos + height; ++iy) {
		GotoXY(x_pos, iy);
		std::cout << u8"\u2588"; // char(219);
		GotoXY(x_pos + width, iy);
		std::cout << u8"\u2588"; // char(219);
	}
	changeTextColor();
}

void draw_level1() {
	SetConsoleOutputCP(CP_UTF8);
	text_color(colorCode::DARK_YELLOW, colorCode::LIGHT_GRAY);
	GotoXY(92, 9); std::cout << u8"█░░░█▀▀░█░█░█▀▀░█░░░▀█░";
	GotoXY(92, 10); std::cout << u8"█░░░█▀▀░▀▄▀░█▀▀░█░░░░█░";
	GotoXY(92, 11); std::cout << u8"▀▀▀░▀▀▀░░▀░░▀▀▀░▀▀▀░▀▀▀";
}

void draw_level2() {
	SetConsoleOutputCP(CP_UTF8);
	text_color(colorCode::DARK_YELLOW, colorCode::LIGHT_GRAY);
	GotoXY(92, 9); std::cout << u8"█░░░█▀▀░█░█░█▀▀░█░░░▀▀▄";
	GotoXY(92, 10); std::cout << u8"█░░░█▀▀░▀▄▀░█▀▀░█░░░▄▀░";
	GotoXY(92, 11); std::cout << u8"▀▀▀░▀▀▀░░▀░░▀▀▀░▀▀▀░▀▀▀";
}

void draw_level3() {
	SetConsoleOutputCP(CP_UTF8);
	text_color(colorCode::DARK_YELLOW, colorCode::LIGHT_GRAY);
	GotoXY(92, 9); std::cout << u8"█░░░█▀▀░█░█░█▀▀░█░░░▀▀█";
	GotoXY(92, 10); std::cout << u8"█░░░█▀▀░▀▄▀░█▀▀░█░░░░▀▄";
	GotoXY(92, 11); std::cout << u8"▀▀▀░▀▀▀░░▀░░▀▀▀░▀▀▀░▀▀░";
}

void draw_level4() {
	SetConsoleOutputCP(CP_UTF8);
	text_color(colorCode::DARK_YELLOW, colorCode::LIGHT_GRAY);
	GotoXY(92, 9); std::cout << u8"█░░░█▀▀░█░█░█▀▀░█░░░█░█";
	GotoXY(92, 10); std::cout << u8"█░░░█▀▀░▀▄▀░█▀▀░█░░░░▀█";
	GotoXY(92, 11); std::cout << u8"▀▀▀░▀▀▀░░▀░░▀▀▀░▀▀▀░░░▀";
}

void draw_huntingSnake() {
	SetConsoleOutputCP(CP_UTF8);
	text_color(colorCode::BLACK, colorCode::LIGHT_BLUE);
	GotoXY(32, 1); std::cout << u8"░█░█░█░█░█▀█░▀█▀░▀█▀░█▀█░█▀▀░░░█▀▀░█▀█░█▀█░█░█░█▀▀\n";
	GotoXY(32, 2); std::cout << u8"░█▀█░█░█░█░█░░█░░░█░░█░█░█░█░░░▀▀█░█░█░█▀█░█▀▄░█▀▀\n";
	GotoXY(32, 3); std::cout << u8"░▀░▀░▀▀▀░▀░▀░░▀░░▀▀▀░▀░▀░▀▀▀░░░▀▀▀░▀░▀░▀░▀░▀░▀░▀▀▀\n";
}

void draw_ButtonW() {
	filled_rec(94, 20, 1, 3, { 63, 199, 212 });
	GotoXY(95, 21);
	changeTextColor({ 0, 0, 0 }, { 63, 199, 212 });
	cout << "W" << u8"\u2191";

	filled_rec(88, 23, 1, 3, { 208, 75, 81 });
	GotoXY(89, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "A" << u8"\u2190";

	filled_rec(94, 23, 1, 3, { 208, 75, 81 });
	GotoXY(95, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "S" << u8"\u2193";

	filled_rec(100, 23, 1, 3, { 208, 75, 81 });
	GotoXY(101, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "D" << u8"\u2192";
}

void draw_ButtonA() {
	filled_rec(94, 20, 1, 3, { 208, 75, 81 });
	GotoXY(95, 21);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "W" << u8"\u2191";

	filled_rec(88, 23, 1, 3, { 63, 199, 212 });
	GotoXY(89, 24);
	changeTextColor({ 0, 0, 0 }, { 63, 199, 212 });
	cout << "A" << u8"\u2190";

	filled_rec(94, 23, 1, 3, { 208, 75, 81 });
	GotoXY(95, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "S" << u8"\u2193";

	filled_rec(100, 23, 1, 3, { 208, 75, 81 });
	GotoXY(101, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "D" << u8"\u2192";
}

void draw_ButtonS() {

	filled_rec(94, 20, 1, 3, { 208, 75, 81 });
	GotoXY(95, 21);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "W" << u8"\u2191";


	filled_rec(88, 23, 1, 3, { 208, 75, 81 });
	GotoXY(89, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "A" << u8"\u2190";


	filled_rec(94, 23, 1, 3, { 63, 199, 212 });
	GotoXY(95, 24);
	changeTextColor({ 0, 0, 0 }, { 63, 199, 212 });
	cout << "S" << u8"\u2193";


	filled_rec(100, 23, 1, 3, { 208, 75, 81 });
	GotoXY(101, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "D" << u8"\u2192";
}

void draw_ButtonD() {

	filled_rec(94, 20, 1, 3, { 208, 75, 81 });
	GotoXY(95, 21);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "W" << u8"\u2191";


	filled_rec(88, 23, 1, 3, { 208, 75, 81 });
	GotoXY(89, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "A" << u8"\u2190";


	filled_rec(94, 23, 1, 3, { 208, 75, 81 });
	GotoXY(95, 24);
	changeTextColor({ 0, 0, 0 }, { 208, 75, 81 });
	cout << "S" << u8"\u2193";


	filled_rec(100, 23, 1, 3, { 63, 199, 212 });
	GotoXY(101, 24);
	changeTextColor({ 0, 0, 0 }, { 63, 199, 212 });
	cout << "D" << u8"\u2192";
}

void TextToAsciiArt(int pos_x, int pos_y, std::string text, RGBCOLOR fg_color, RGBCOLOR bg_color)
{
	SetConsoleOutputCP(CP_UTF8);
	changeTextColor(fg_color, bg_color);
	int	origin_y = pos_y;

	for (const char i : text) {
		switch (i)
		{
		case 'a': case 'A':
			GotoXY(pos_x, pos_y++); cout << u8"▄▀▄ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▀█ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'b':case 'B':
			GotoXY(pos_x, pos_y++); cout << u8"██▄ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▄█ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'c': case 'C':
			GotoXY(pos_x, pos_y++); cout << u8"▄▀▀ ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄▄ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'd': case 'D':
			GotoXY(pos_x, pos_y++); cout << u8"█▀▄ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▄▀ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'e': case 'E':
			GotoXY(pos_x, pos_y++); cout << u8"██▀ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▄▄ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'f': case 'F':
			GotoXY(pos_x, pos_y++); cout << u8"█▀ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▀ ";
			pos_y = origin_y;
			pos_x += 3;
			break;
		case 'g': case 'G':
			GotoXY(pos_x, pos_y++); cout << u8"▄▀  ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄█ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'h': case 'H':
			GotoXY(pos_x, pos_y++); cout << u8"█▄█ ";
			GotoXY(pos_x, pos_y++); cout << u8"█ █ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'i': case 'I':
			GotoXY(pos_x, pos_y++); cout << u8"█ ";
			GotoXY(pos_x, pos_y++); cout << u8"█ ";
			pos_y = origin_y;
			pos_x += 2;
			break;
		case 'j': case 'J':
			GotoXY(pos_x, pos_y++); cout << u8"  █ ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄█ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'k': case 'K':
			GotoXY(pos_x, pos_y++); cout << u8"█▄▀ ";
			GotoXY(pos_x, pos_y++); cout << u8"█ █ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'l': case 'L':
			GotoXY(pos_x, pos_y++); cout << u8"█   ";
			GotoXY(pos_x, pos_y++); cout << u8"█▄▄ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'm': case 'M':
			GotoXY(pos_x, pos_y++); cout << u8"█▄ ▄█ ";
			GotoXY(pos_x, pos_y++); cout << u8"█ ▀ █ ";
			pos_y = origin_y;
			pos_x += 6;
			break;
		case 'n': case 'N':
			GotoXY(pos_x, pos_y++); cout << u8"█▄ █ ";
			GotoXY(pos_x, pos_y++); cout << u8"█ ▀█ ";
			pos_y = origin_y;
			pos_x += 5;
			break;
		case 'o': case 'O':
			GotoXY(pos_x, pos_y++); cout << u8"▄▀▄ ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄▀ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'p': case 'P':
			GotoXY(pos_x, pos_y++); cout << u8"█▀▄ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▀  ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'q': case 'Q':
			GotoXY(pos_x, pos_y++); cout << u8"▄▀▄ ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄█ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'r': case 'R':
			GotoXY(pos_x, pos_y++); cout << u8"█▀▄ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▀▄ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 's': case 'S':
			GotoXY(pos_x, pos_y++); cout << u8"▄▀▀ ";
			GotoXY(pos_x, pos_y++); cout << u8"▄██ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 't': case 'T':
			GotoXY(pos_x, pos_y++); cout << u8"▀█▀ ";
			GotoXY(pos_x, pos_y++); cout << u8" █  ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'u': case 'U':
			GotoXY(pos_x, pos_y++); cout << u8"█ █ ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄█ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'v': case 'V':
			GotoXY(pos_x, pos_y++); cout << u8"█ █ ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄▀ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'w': case 'W':
			GotoXY(pos_x, pos_y++); cout << u8"█   █ ";
			GotoXY(pos_x, pos_y++); cout << u8"▀▄▀▄▀ ";
			pos_y = origin_y;
			pos_x += 6;
			break;
		case 'x': case 'X':
			GotoXY(pos_x, pos_y++); cout << u8"▀▄▀ ";
			GotoXY(pos_x, pos_y++); cout << u8"█ █ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'y': case 'Y':
			GotoXY(pos_x, pos_y++); cout << u8"▀▄▀ ";
			GotoXY(pos_x, pos_y++); cout << u8" █  ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case 'z': case 'Z':
			GotoXY(pos_x, pos_y++); cout << u8"▀█▀ ";
			GotoXY(pos_x, pos_y++); cout << u8"█▄▄ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		case ' ':
			pos_x += 2;
			pos_y = origin_y;
			break;
		case '.':
			GotoXY(pos_x, y_pos + 1); cout << u8"▄ ";
			pos_x += 2;
			pos_y = origin_y;
			break;
		case ',':
			GotoXY(pos_x, y_pos + 1); cout << u8"█ ";
			pos_x += 2;
			pos_y = origin_y;
			break;
		case '?':
			GotoXY(pos_x, pos_y++); cout << u8"▄▀▄ ";
			GotoXY(pos_x, pos_y++); cout << u8" ▄▀ ";
			pos_y = origin_y;
			pos_x += 4;
			break;
		}
	}
	changeTextColor();
}
int AsciiArtTextSize(std::string text) {
	int res = 0;
	for (const char i : text) {
		switch (i)
		{
		case 'a': case 'A':	case 'b': case 'B':	case 'c': case 'C':	case 'd': case 'D': case 'e': case 'E': case 'g': case 'G': case 'h': case 'H': 
		case 'j': case 'J':	case 'k': case 'K': case 'l': case 'L':	case 'o': case 'O': case 'p': case 'P':	case 'q': case 'Q':	case 'r': case 'R':	
		case 's': case 'S':	case 't': case 'T':	case 'u': case 'U': case 'v': case 'V':	case 'x': case 'X': case 'y': case 'Y':	case 'z': case 'Z': 
		case '?':
			res += 4;
			break;
		case 'i': case 'I': case ' ': case '.': case ', ':
			res += 2;
			break;
		case 'f': case 'F':
			res += 3;
			break;
		case 'm': case 'M':case 'w': case 'W':
			res += 6;
			break;
		case 'n':case 'N':
			res += 5;
			break;
		default:
			break;
		}
	}
	return res;
}
void pause_game() {
	//system("cls");
	//draw_matchBoard(10, 20, 10, 20, scor, le, 2, 0, "", 1);
}


void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, RGBCOLOR _bg_text)
{
	SetConsoleOutputCP(CP_UTF8);
	changeTextColor(color, _bg_text);
	for (int ix = x_pos + 1; ix < x_pos + width; ++ix) {
		GotoXY(ix, y_pos);
		std::cout << u8"\u2550"; //char(205)
		GotoXY(ix, y_pos + height);
		std::cout << u8"\u2550"; // char(205);
	}
	GotoXY(x_pos, y_pos);
	for (int iy = y_pos + 1; iy < y_pos + height; ++iy) {
		GotoXY(x_pos, iy);
		std::cout << u8"\u2551"; // char(186);
		GotoXY(x_pos + width, iy);
		std::cout << u8"\u2551"; // char(186);
	}
	// draw 4 corners
	GotoXY(x_pos, y_pos);
	std::cout << u8"\u2554"; // char(201);
	GotoXY(x_pos + width, y_pos);
	std::cout << u8"\u2557"; // char(187);
	GotoXY(x_pos, y_pos + height);
	std::cout << u8"\u255A"; // char(200);
	GotoXY(x_pos + width, y_pos + height);
	std::cout << u8"\u255D"; // char(188);
	changeTextColor();
}


void draw_rectangle(unsigned int x_pos, unsigned int y_pos, unsigned int height, unsigned int width, RGBCOLOR color, std::string text, RGBCOLOR txtColor, RGBCOLOR _bg_text)
{
	SetConsoleOutputCP(CP_UTF8);
	draw_rectangle(x_pos, y_pos, height, width, color, _bg_text);

	changeTextColor(txtColor, _bg_text);
	GotoXY(x_pos + (width - text.size()) / 2, y_pos + height / 2);
	std::cout << text;
	changeTextColor();
}




