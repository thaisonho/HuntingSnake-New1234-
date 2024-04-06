#include "ProcessPlayer.h"
#include "ConsoleWindow.h"
#include "graphics.h"
#include "arts.h"
#include <fstream>
#include <iostream>

using namespace std;

int selectionLoad = 1;
string st[100];

void NewGame(Player PlayerSnake[], int& id, string& namePlayer)
{
	system("cls");

	draw_User_Name(0, 0, User_Name);

	changeTextColor({ 245, 245, 245 }, { 88, 98, 88 });

	GotoXY(51, 12); cout << "NAME PLAYER: ";

	getline(cin, namePlayer); changeTextColor();

	while (true)
	{
		if (isValidName(PlayerSnake, id, namePlayer) == true && namePlayer.size() <= 10 && namePlayer != "")
		{
			if (checkMusicEffect) PlaySound(TEXT("sound/click.wav"), NULL, SND_FILENAME | SND_ASYNC);
			break;
		}
			
		notValid();
		Sleep(1500);
		refill();

		changeTextColor({ 245, 245, 245 }, { 88, 98, 88 });
		getline(cin, namePlayer);
		changeTextColor();
	}

	id++;
	PlayerSnake[id].Name = namePlayer;
}

bool isValidName(Player PlayerSnake[], int id, string &st)
{

	for (int i = 1; i <= id; i++)
	{
		if (st == PlayerSnake[i].Name)
			return false;
	}

	return true;
}

int LoadGameBoard(Player PlayerSnake[], int id, Player &LoadPlayer)
{
	bool LoadSnake = false;
	SetConsoleCP(CP_UTF8);
	system("cls");

	draw_rectangle((120 - 100) / 2, 3, 22, 100, TXT_RGB);
	int x = 10, y = y_pos - 2;
	int xmax = WIDTH_GAME + 15;
	int ymax = HEIGHT_BOARD;

	GotoXY((120 - 100) / 2, 27);				cout << "Press <Enter> to continue playing";
	GotoXY((120 - 100) / 2 + (100 - 29), 27);	cout << "Press <Esc> to return to menu";

	int bannerPosX = (120 - 49) / 2, bannerPosY = 2;
	changeTextColor({ 238, 114, 20 });
	GotoXY(bannerPosX, bannerPosY++);	cout << u8"░▒█▀▀▀█░█▀▀▄░▄░░░▄░█▀▀░█▀▄░░░▒█▀▀█░█▀▀▄░█▀▄▀█░█▀▀\n";
	GotoXY(bannerPosX, bannerPosY++);	cout << u8"░░▀▀▀▄▄░█▄▄█░░█▄█░░█▀▀░█░█░░░▒█░▄▄░█▄▄█░█░▀░█░█▀▀\n";
	GotoXY(bannerPosX, bannerPosY++);	cout << u8"░▒█▄▄▄█░▀░░▀░░░▀░░░▀▀▀░▀▀░░░░▒█▄▄▀░▀░░▀░▀░░▒▀░▀▀▀\n";
	changeTextColor();

	int distance = (xmax - x) / 3;

	int x1 = x + 13;
	int y1 = y + 3;
	int y2 = y1, y3 = y1;

	changeTextColor(TXT_RGB);
	GotoXY(x1, y1++);	cout << u8"░█▄░█▒▄▀▄░█▄▒▄█▒██▀";
	GotoXY(x1, y1++);	cout << u8"░█▒▀█░█▀█░█▒▀▒█░█▄▄";

	int x2 = x1 + distance;
	GotoXY(x2, y2++);	cout << u8"░▄▀▀░▄▀▀░▄▀▄▒█▀▄▒██▀";
	GotoXY(x2, y2++);	cout << u8"▒▄██░▀▄▄░▀▄▀░█▀▄░█▄▄";

	int x3 = x2 + distance;
	GotoXY(x3, y3++);	cout << u8"░█▒░▒██▀░█▒█▒██▀░█▒░";
	GotoXY(x3, y3++);	cout << u8"▒█▄▄░█▄▄░▀▄▀░█▄▄▒█▄▄";
	changeTextColor();

	ifstream INP;
	INP.open("data.txt");
	INP >> id;
	INP.close();

	string name = "", prev_name = "";
	int score = 0, prev_score = 0, level = 1, prev_level = 1;
	int x_menu = (120 - 100) / 2 + 5,
		y_menu = 10,
		rec_width = 90,
		rec_height = 0;

	for (int i = 1; i <= 7 || i <= id; ++i) {
		GotoXY(x_menu + 8, y_menu + 2 * (i - 1));
		changeTextColor(TXT_RGB);
		if (i > id) {
			cout << "<EMPTY>";
		}
		else {
			extractDataFile(PlayerSnake, i, LoadPlayer);

			cout << LoadPlayer.Name;
			GotoXY(x_menu + 37, y_menu + 2 * (i - 1));	cout << LoadPlayer.score;
			GotoXY(x_menu + 65, y_menu + 2 * (i - 1));	cout << LoadPlayer.level;
		}
		changeTextColor();
	}

	// define variables use for navigate through the menu
	int x_pointer = x_menu, y_pointer = y_menu;
	int x_prev = x_menu, y_prev = y_menu;
	bool check = true, isEnter = false;
	selectionLoad = 1;

	while (isEnter == false) {
		GotoXY(x_pointer, y_pointer);

		// highlitght the current option that the user choose, only if any key is pressed
		if (check) {
			//if the user navigate to the next button, the x_prev and y_prev variables will store the previous axis
			// then turn off the hightlight of the box by set the first paraameter of highlightedBox() to false
			GotoXY(x_prev, y_prev);
			filled_rec(x_prev, y_prev, 0, rec_width, BG_RGB_2);
			changeTextColor(TXT_RGB);
			GotoXY(x_prev + 8, y_prev);		cout << prev_name;
			if (prev_name != "<EMPTY>") {
				GotoXY(x_prev + 37, y_prev);	cout << prev_score;
				GotoXY(x_prev + 65, y_prev);	cout << prev_level;
			}

			// change the x_prev & y_prev to the current position
			x_prev = x_pointer;
			y_prev = y_pointer;	

			// hightlight the current button
			filled_rec(x_pointer, y_pointer, 0, rec_width, { 247, 183, 135 });
			GotoXY(x_pointer, y_pointer);
			//changeTextColor(TXT_RGB, { 247, 183, 135 });

			extractDataFile(PlayerSnake, (y_pointer - y_menu) / 2 + 1, LoadPlayer);
			name = LoadPlayer.Name;
			score = LoadPlayer.score;
			level = LoadPlayer.level;

			prev_name = name;
			prev_level = level;
			prev_score = score;

			changeTextColor(TXT_RGB, { 247, 183, 135 });
			GotoXY(x_pointer + 14, y_pointer);
			if (name == "") {
				name = "<EMPTY>";
				prev_name = name;
				cout << name;
			}
			else {
				cout << name;
				GotoXY(x_prev + 37 + 8, y_prev);	cout << score;
				GotoXY(x_prev + 65 + 8, y_prev);	cout << level;
			};

			st[(y_pointer / y_menu)] = name;
			changeTextColor();

			// set to false to mark that now there's no button pressed
			check = false;
		}

		// navigate through the menu
		if (_kbhit()) { // if any key is pressed
			if (checkMusicEffect) PlaySound(TEXT("sound/move.wav"), NULL, SND_FILENAME | SND_ASYNC);
			check = true;

			// variable to receive user input
			char handle;
			handle = toupper(_getch());
			Sleep(50);

			switch (handle)
			{
			case 'S': case 'P': // if user pressed S or 'Arrow Down'
				// move pointer down to the next button
				y_pointer += 2;
				// if pointer is out of range => set the pointer to the first position
				if (y_pointer > y_menu + 13) y_pointer = y_menu;
				GotoXY(x_pointer, y_pointer);
				break;
			case 'W': case 'H': // if user pressed W or 'Arrow Up'
				// move pointer up to the next button
				y_pointer -= 2;
				// if pointer is out of range => set the pointer to the last button
				if (y_pointer < y_menu) y_pointer = y_menu + 12;
				GotoXY(x_pointer, y_pointer);
				break;
			case 13: // if user pressed 'Enter'
				selectionLoad = (y_pointer / y_menu);

				if (st[selectionLoad] != "<EMPTY>")
					isEnter = true;
				else
					isEnter = false;

				LoadSnake = true;
				break;
			case 27: // if ESC hitted
				selectionLoad = -1;
				isEnter = true;
				break;
			}
		}
	}
	switch (selectionLoad)
	{
	case -1:
		return -1;
		break;
	}

	system("cls");
	return 0;
}

void LoadGamePlayer(Player PlayerSnake[], int id, Player &LoadPlayer)
{
	system("cls");
	int res = LoadGameBoard(PlayerSnake, id, LoadPlayer);
	if (res == -1) return;

	thread thread_obj(ThreadFunction); // Create thread for snake
	HANDLE handle_thread_obj = thread_obj.native_handle(); // Take handle of thread
	SuspendThread(handle_thread_obj);

	StartGame(1);
	LoadGame();
}

void CleanDataFile()
{
	ofstream OUTT("data.txt");
	OUTT << "";

	OUTT.close();
}

void notValid() {
	if (checkMusicEffect) PlaySound(TEXT("sound/wrong.wav"), NULL, SND_FILENAME | SND_ASYNC);
	GotoXY(33, 14);
	changeTextColor({ 245, 245, 245 }, { 88, 98, 88 }); cout << "This name is not valid. Please enter the name again!!!\n"; changeTextColor();
}

void refill() {
	GotoXY(33, 14);
	changeTextColor({ 88, 98, 88 }, { 88, 98, 88 }); cout << "                                                      "; changeTextColor();
	GotoXY(64, 12);
	changeTextColor({ 88, 98, 88 }, { 88, 98, 88 }); cout << "                        "; changeTextColor();
	GotoXY(64, 12);
}
