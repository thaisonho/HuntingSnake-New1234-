#include "Snake.h"
#include "arts.h"
#include "ProcessPlayer.h"
#include "ScoreList.h"
#include "MapTracking.h"
#include "GenerateMap.h"
#include "Variables.h"



using namespace std;

// Random number generator (rng) generates seed to random number [0, 2^64 - 1]
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

CONSOLE_SCREEN_BUFFER_INFO csbi;
//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
CHAR_INFO ci;

// Declare Global Variables, Structs
Point Snake[MAX_SIZE_SNAKE]; // Snake
Point Food[MAX_SIZE_FOOD]; // Food
Point BonusFood[MAX_SIZE_FOOD - 2]; // Food
Point Center; // Center coordinates of Gate
Point Gate[5]; // Coordinates of Gate
Point* obs = new Point[1000];
Point const_obs[1000];

string str[MAX_SIZE_SNAKE];
string namePlayer;

Player PlayerSnake[50];
Player LoadPlayer;
ScoreList SPlayer[50];

int STATE; // DEAD or ALIVE
int ID_Food = 0; // Current index of food 
int ID_BonusFood = 0; // Current index of Bonus food
int SCORE = 0; // Current score
int highScore = 0; // High score
int TYPE; // Type of Gate (4 types directions = 4 types gates)
int cntGate; // index of coordinates of Gate
int Snake_Size; // Size of snake, initially 6 units and maximum is 22 units
int obs_nums; // Size of obstacles
int const_obs_nums;
int id = 0;

char CHAR_LOCK; // At a moment, there is one direction that snake cannot move to
char MOVING; // At a moment, there are three directions that snake can move

bool GateDraw = false; // Gate exist or not
bool up = false;
bool WIN = false;
atomic<bool> stop_thread(false);

float SPEED; // Current speed (Speed increase with each level)
float addTime; // Control the speed between rows and columns 
// Speed through rows < speed through columns (number of columns > number of rows)

float DeltaSpeed = 0; // Control the speed when move to next level

int lev = 1; // Current level (Maximum is 5 levels)

// EXTERN
char ch[100][100];

// Global variable to store the thread handle
HANDLE g_ThreadHandle;

// Overload input/output operator for Point
istream& operator>>(istream& inDev, Point& p)
{
	inDev >> p.x >> p.y;
	return inDev;
}

ostream& operator << (ostream& outDev, Point p)
{
	outDev << p.x << " " << p.y << "\n";
	return outDev;
}

// Function to set the thread handle
void SetThreadHandle(HANDLE t) {
	g_ThreadHandle = t;
}

// Function to exit the game
void ExitGame() {
	// Clear the console screen
	system("cls");

	// Set STATE to 0 (assuming STATE is a global variable)
	STATE = 0;

	// Terminate the thread using the stored handle
	TerminateThread(g_ThreadHandle, 0);

	// Get the handle of the console window
	HWND consoleWindow = GetConsoleWindow();

	// Post a close message to the console window
	PostMessage(consoleWindow, WM_CLOSE, 0, 0);

	// Exit the process
	ExitProcess(0);
}

void StartGame(bool LoadSnake)
{

	system("cls"); // Clear screen
	if (LoadSnake == false)
		ResetData(); // Intialize original data
	else
		ResetLoadData(); // Intialize Load data

	SCORE = (LoadSnake ? LoadPlayer.score : 0);

	STATE = 1; // ALIVE (Running thread)

	switch (map)
	{
	case 1:
	{
		Extract2DPic(ch, "MAP1");
		break;
	}
	case 2:
	{
		Extract2DPic(ch, "MAP2");
		break;
	}
	case 3:
	{
		Extract2DPic(ch, "MAP3");
		break;
	}
	case 4:
	{
		Extract2DPic(ch, "MAP4");
		break;
	}
	}

	DrawSnake(MSSV); // Draw snake with MSSV of four members in group
}

void LoadGame()  // void LoadGame(int& lev)
{
	switch (lev)
	{
	case 1:
		draw_matchBoard(x_pos, y_pos, HEIGHT_BOARD, WIDTH_BOARD);
		break;
	case 2:
		play_match2(x_pos, y_pos, HEIGHT_BOARD, WIDTH_BOARD, obs, obs_nums);
		break;
	case 3:
		play_match3(x_pos, y_pos, HEIGHT_BOARD, WIDTH_BOARD, obs, obs_nums);
		break;
	case 4:
		create_obstacle_4(x_pos, y_pos, HEIGHT_BOARD, WIDTH_BOARD, obs, obs_nums, const_obs, const_obs_nums);
		draw_matchBoard(x_pos, y_pos, HEIGHT_BOARD, WIDTH_BOARD);
		break;
	}

	thread thread_obj(ThreadFunction); // Create thread for snake
	HANDLE handle_thread_obj = thread_obj.native_handle(); // Take handle of thread

	while (true)
	{
		if (stop_thread) {
			break;
		}

		char temp = toupper(_getch()); // Get character from the keyboard (including 4 arrows, WASD, ...)

		if (STATE == 1)
		{
			ResumeThread(handle_thread_obj);

			if (temp == 'U') // 'U' Key to Pause Game
				PauseGame(handle_thread_obj);
			if ((temp == 'W' || temp == 'S' || temp == 'A' || temp == 'D') && temp != CHAR_LOCK)
			{
				MOVING = temp; // Next movement of snake
			}
		}
	}

	thread_obj.join();
	for (int i = 0; i <= 28; ++i) {
		changeTextColor(BG_RGB_2);
		GotoXY(0, i); cout << "                                                                                                                        ";
	}
	changeTextColor();
	stop_thread = false;
	return;
}

void ProcessPause(HANDLE t) {

	draw_PAUSE(0, 5, PAUSE);

	int x_menu = 25;
	int y_menu = 13;

	bool check = true, isEnter = false;
	int selection = 1;

	draw_rectangle(x_menu, y_menu, 2, 30, { 234, 222, 165 }, "PLAY", TXT_RGB, { 253, 255, 154 });

	draw_rectangle(x_menu, y_menu + 5, 2, 30, { 234, 222, 165 }, "SAVE GAME AND EXIT", TXT_RGB, { 253, 255, 154 });

	unsigned int x_pointer = x_menu, y_pointer = y_menu;
	unsigned int x_prev = x_menu, y_prev = y_menu;


	while (true) {
		GotoXY(x_pointer, y_pointer);
		if (check) {
			GotoXY(x_prev, y_prev);
			draw_rectangle(x_prev, y_prev, 2, 30, TXT_RGB, { 253, 255, 154 });

			x_prev = x_pointer;
			y_prev = y_pointer;

			highlightedBox(x_pointer, y_pointer, 2, 30, { 225, 0, 0 });

			check = false;
		}

		if (_kbhit()) { // if any key is pressed
			check = true;

			char handle;
			handle = toupper(_getch());
			Sleep(50);

			switch (handle)
			{
			case 'S': // if user pressed S to move pointer down
				// move pointer down to the next button
				y_pointer += 5;
				selection++;
				if (selection > 2) selection = 1;
				// if pointer is out of range => set the pointer to the first position
				if (y_pointer > y_menu + 5) y_pointer = y_menu;
				GotoXY(x_pointer, y_pointer);
				break;
			case 'W': // if user pressed W to move pointer up
				// move pointer up to the next button
				y_pointer -= 5;
				selection--;
				if (selection < 1) selection = 2;
				// if pointer is out of range => set the pointer to the last button
				if (y_pointer < y_menu) y_pointer = y_menu + 5;
				GotoXY(x_pointer, y_pointer);

				break;
			case 13: // if user pressed 'Enter'
				//int selection = (y_pointer % y_menu) / (2 + 1);
				if (selection == 1) {
					LoadGame();
					//LoadSnake to continue to play
				}
				if (selection == 2) {
					SaveGame(t);
				}
				break;
			}
		}
	}
}

void PauseGame(HANDLE t)
{
	//pause_game();
	SuspendThread(t);
	ProcessPause(t);
}

void SaveGame(HANDLE t)
{
	if (LoadSnake)
		ExtractInfoPlayer(PlayerSnake, LoadOption, namePlayer);
	else
		ExtractInfoPlayer(PlayerSnake, id, namePlayer);

	outputInfoPlayer(PlayerSnake, id);
	SuspendThread(t);
	system("cls");

	mainMenu();
}

void extractDataFile(Player PlayerSnake[], int i, Player& LoadPlayer)
{
	ifstream INP;
	INP.open("data.txt");

	int id;
	INP >> id;

	for (int i = 1; i <= id; i++)
	{
		INP >> PlayerSnake[i].Name;
		INP >> PlayerSnake[i].level;
		INP >> PlayerSnake[i].map;
		INP >> PlayerSnake[i].ID_FOOD;
		INP >> PlayerSnake[i].ID_BONUSFOOD;
		INP >> PlayerSnake[i].direction;
		INP >> PlayerSnake[i].charLock;
		INP >> PlayerSnake[i].delta_speed;
		INP >> PlayerSnake[i].score;
		INP >> PlayerSnake[i].sizeOfSnake;
		INP >> PlayerSnake[i].speed;
		INP >> PlayerSnake[i].cntGate;
		INP >> PlayerSnake[i].cntObs;

		for (int j = 0; j < PlayerSnake[i].ID_FOOD; j++)
			INP >> PlayerSnake[i].Food[j];

		for (int j = 0; j < PlayerSnake[i].ID_BONUSFOOD; j++)
			INP >> PlayerSnake[i].BonusFood[j];

		for (int j = 0; j < PlayerSnake[i].sizeOfSnake; j++)
			INP >> PlayerSnake[i].Snake[j];

		for (int j = 0; j < PlayerSnake[i].cntObs; j++)
			INP >> PlayerSnake[i].obs[j];

		for (int j = 0; j < PlayerSnake[i].cntGate; j++)
			INP >> PlayerSnake[i].Gate[j];
	}

	INP.close();
	LoadPlayer = PlayerSnake[i];
}

void ExtractInfoPlayer(Player PlayerSnake[], int& id, string& namePlayer)
{
	if (LoadSnake)
	{
		PlayerSnake[id].level = lev;
		PlayerSnake[id].map = map;
		PlayerSnake[id].ID_FOOD = ID_Food;
		PlayerSnake[id].ID_BONUSFOOD = ID_BonusFood;
		PlayerSnake[id].direction = MOVING;
		PlayerSnake[id].charLock = CHAR_LOCK;
		PlayerSnake[id].delta_speed = DeltaSpeed;
		PlayerSnake[id].score = SCORE;
		PlayerSnake[id].sizeOfSnake = Snake_Size;
		PlayerSnake[id].speed = SPEED;
		PlayerSnake[id].cntGate = cntGate;
		PlayerSnake[id].cntObs = obs_nums;
	}
	else
	{
		PlayerSnake[id].Name = namePlayer;
		PlayerSnake[id].level = lev;
		PlayerSnake[id].map = map;
		PlayerSnake[id].ID_FOOD = ID_Food;
		PlayerSnake[id].ID_BONUSFOOD = ID_BonusFood;
		PlayerSnake[id].direction = MOVING;
		PlayerSnake[id].charLock = CHAR_LOCK;
		PlayerSnake[id].delta_speed = DeltaSpeed;
		PlayerSnake[id].score = SCORE;
		PlayerSnake[id].sizeOfSnake = Snake_Size;
		PlayerSnake[id].speed = SPEED;
		PlayerSnake[id].cntGate = cntGate;
		PlayerSnake[id].cntObs = obs_nums;
	}

	for (int i = 0; i < ID_Food; i++)
		PlayerSnake[id].Food[i] = Food[i];

	for (int i = 0; i < ID_BonusFood; i++)
		PlayerSnake[id].BonusFood[i] = BonusFood[i];

	for (int i = 0; i < Snake_Size; i++)
		PlayerSnake[id].Snake[i] = Snake[i];

	for (int i = 0; i < PlayerSnake[i].cntObs; i++)
		PlayerSnake[id].obs[i] = obs[i];

	for (int i = 0; i < PlayerSnake[i].cntGate; i++)
		PlayerSnake[id].Gate[i] = Gate[i];

	highScore = max(highScore, PlayerSnake[id].score);
}

void outputInfoPlayer(Player PlayerSnake[], int id)
{
	ofstream OUTT;
	OUTT.open("data.txt");

	OUTT << id << "\n";

	for (int i = 1; i <= id; i++)
	{
		OUTT << PlayerSnake[i].Name << "\n";
		OUTT << PlayerSnake[i].level << "\n";
		OUTT << PlayerSnake[i].map << "\n";
		OUTT << PlayerSnake[i].ID_FOOD << "\n";
		OUTT << PlayerSnake[i].ID_BONUSFOOD << "\n";
		OUTT << PlayerSnake[i].direction << "\n";
		OUTT << PlayerSnake[i].charLock << "\n";
		OUTT << PlayerSnake[i].delta_speed << "\n";
		OUTT << PlayerSnake[i].score << "\n";
		OUTT << PlayerSnake[i].sizeOfSnake << "\n";
		OUTT << PlayerSnake[i].speed << "\n";
		OUTT << PlayerSnake[i].cntGate << "\n";
		OUTT << PlayerSnake[i].cntObs << "\n";

		for (int j = 0; j < PlayerSnake[i].ID_FOOD; j++)
			OUTT << PlayerSnake[i].Food[j];

		for (int j = 0; j < PlayerSnake[i].ID_BONUSFOOD; j++)
			OUTT << PlayerSnake[i].BonusFood[j];

		for (int j = 0; j < PlayerSnake[i].sizeOfSnake; j++)
			OUTT << PlayerSnake[i].Snake[j];

		for (int j = 0; j < PlayerSnake[i].cntObs; j++)
			OUTT << PlayerSnake[i].obs[j];

		for (int j = 0; j < PlayerSnake[i].cntGate; j++)
			OUTT << PlayerSnake[i].Gate[j];
	}

	OUTT.close();
}

void ProcessDead()
{
	SPlayer[id].score = SCORE;

	STATE = 0; // DEAD
	draw_snakeDEAD(0, 5, snakeDEAD);

}

void StopThread()
{
	stop_thread = true;
}

void ProcessWin()
{
	SPlayer[id].score = SCORE;

	GotoXY(35, 1);
	draw_snakeWIN(0, 5, snakeWIN);
	system("pause");
}

bool ProcessExit()
{
	drawExitScreen(0, 0);

	int x_menu = 65;
	int y_menu = 11;

	bool check = true, isEnter = false;
	int selection = 1;

	draw_rectangle(x_menu, y_menu, 2, 30, { 249, 232, 217 }, "YES", TXT_RGB, { 249, 232, 217 });
	draw_rectangle(x_menu, y_menu + 5, 2, 30, { 0, 0, 0 }, "NO", TXT_RGB, { 249, 232, 217 });

	unsigned int x_pointer = x_menu, y_pointer = y_menu;
	unsigned int x_prev = x_menu, y_prev = y_menu;


	while (true) {
		GotoXY(x_pointer, y_pointer);
		if (check) {
			GotoXY(x_prev, y_prev);
			draw_rectangle(x_prev, y_prev, 2, 30, TXT_RGB, { 249, 232, 217 });

			x_prev = x_pointer;
			y_prev = y_pointer;

			highlightedBox(x_pointer, y_pointer, 2, 30, { 225, 0, 0 });

			check = false;
		}

		if (_kbhit()) { // if any key is pressed
			check = true;

			char handle;
			handle = toupper(_getch());
			Sleep(50);

			switch (handle)
			{
			case 'S':
				y_pointer += 5;
				selection++;
				if (selection > 2) selection = 1;
				if (y_pointer > y_menu + 5) y_pointer = y_menu;
				GotoXY(x_pointer, y_pointer);
				break;
			case 'W':
				y_pointer -= 5;
				selection--;
				if (selection < 1) selection = 2;
				if (y_pointer < y_menu) y_pointer = y_menu + 5;
				GotoXY(x_pointer, y_pointer);
				break;
			case 13:
				if (selection == 1) {
					return true;
				}
				if (selection == 2) {
					return false;
				}
				break;
			}
		}
	}
}

void ResetLoadData()
{
	// Get info of previous Snake from Load Game
	extractDataFile(PlayerSnake, LoadOption, LoadPlayer);

	//Initialize the global variables, structs
	for (int i = 0; i < LoadPlayer.cntGate; i++)
		Gate[i] = LoadPlayer.Gate[i];

	cntGate = 0;

	//map = LoadPlayer.map;
	lev = LoadPlayer.level;

	SCORE = LoadPlayer.score;

	SPEED = LoadPlayer.speed;
	DeltaSpeed = LoadPlayer.delta_speed;

	addTime = 0;

	Snake_Size = LoadPlayer.sizeOfSnake;

	CHAR_LOCK = LoadPlayer.charLock;
	MOVING = LoadPlayer.direction;

	ID_Food = LoadPlayer.ID_FOOD;
	ID_BonusFood = LoadPlayer.ID_BONUSFOOD;

	for (int i = 0; i < ID_Food; i++)
		Food[i] = LoadPlayer.Food[i];

	for (int i = 0; i < ID_BonusFood; i++)
		BonusFood[i] = LoadPlayer.BonusFood[i];

	//Initialize the coordinates of snake
	for (int i = 0; i < Snake_Size; i++)
		Snake[i] = LoadPlayer.Snake[i];

	system("cls");
	obs_nums = 0;
	obs = (Point*)calloc(1000, sizeof(Point));
}


void ResetData()
{
	//Initialize the global variables, structs
	for (int i = 0; i < cntGate; i++)
		Gate[i] = { 0, 0 };

	cntGate = 0;
	SCORE = 0;

	SPEED = 1.0F;
	addTime = 0;
	Snake_Size = 6 + MAX_SIZE_FOOD * (lev - 1);

	CHAR_LOCK = 'A';
	MOVING = 'D';

	ID_Food = 0;
	ID_BonusFood = 0;

	int Init = (10 + Snake_Size) / 2 + 5;

	//Initialize the coordinates of snake
	if (lev == 4)
	{
		Init = 10;
		for (int i = 0; i < Snake_Size; i++)
			Snake[i] = { 8, Init + i };
	}
	else
	{
		for (int i = 0; i < Snake_Size; i++)
			Snake[i] = { Init - i, 8 };
	}

	GenerateFood(); // Create food array
	GenerateBonusFood(); // Create Bonus Food array

	system("cls");
	obs_nums = 0;
	obs = (Point*)calloc(1000, sizeof(Point));
}

void ThreadFunction()
{
	//int leve = getValue(lev);
	while (true)
	{
		if (stop_thread)
		{
			ProcessDead();
			return;
		}

		switch (lev)
		{
		case 4:
			//Sleep(100);
			move_obs(x_pos, y_pos, HEIGHT_BOARD, WIDTH_BOARD, obs, obs_nums, up, const_obs, const_obs_nums);
		}

		int x = Snake[Snake_Size - 1].x, y = Snake[Snake_Size - 1].y;
		if (STATE == 1) // If snake ALIVE -> continue
		{
			//EraseOldSnake(); // Erase old snake and draw new snake
			EraseTrace(ch, x, y, map);
			switch (MOVING)
			{
				// WASD Key   W: Up, S: Down, A: Left, D: Right
				// Addtime set speed different between columns and rows
			case 'W':
			{
				draw_ButtonW();
				MoveUp();
				EraseTrace(ch, x, y, map);
				CHAR_LOCK = 'S';
				addTime = 0.35F;
				//drawMAP2(0, 5, MAP2);
				break;
			}

			case 'S':
			{
				draw_ButtonS();
				MoveDown();
				EraseTrace(ch, x, y, map);
				CHAR_LOCK = 'W';
				addTime = 0.35F;
				break;
			}

			case 'A':
			{
				draw_ButtonA();
				MoveLeft();
				EraseTrace(ch, x, y, map);
				CHAR_LOCK = 'D';
				addTime = 0;
				break;
			}

			case 'D':
			{
				draw_ButtonD();
				MoveRight();
				EraseTrace(ch, x, y, map);
				CHAR_LOCK = 'A';
				addTime = 0;
				break;
			}

			default:
				break;
			}
		}

		for (int i = 0; i < Snake_Size; i++)
		{
			if (TouchObs(Snake[i].x, Snake[i].y))
			{
				ProcessDead();
				return;
			}
		}

		increaseScore(SCORE);
		EraseTrace(ch, x, y, map);
		DrawSnake(MSSV); // After one move, draw new snake because of the change of coordinates
		ProcessGate(); // Process if meet the conditions to create the gate
		Sleep(100 / (SPEED + DeltaSpeed - addTime)); // Sleep function will set the speed of snake
	}
}

void DrawSnake(const string& str) // With str is MSSV
{
	// Draw head of snake
	GotoXY(Snake[0].x, Snake[0].y);
	changeTextColor({ 255, 0, 0 }); // Color of head will be different from body
	cout << str[0];

	for (int i = 1; i < Snake_Size; i++)
	{
		GotoXY(Snake[i].x, Snake[i].y);
		changeTextColor(_userChoiceFG, _userChoiceBG);
		cout << str[i]; // Draw body of snake
	}
}

void DrawFood()
{
	SetConsoleOutputCP(CP_UTF8);

	//GotoXY(Food[ID_Food].x, Food[ID_Food].y);

	int X = Food[ID_Food].x, Y = Food[ID_Food].y;
	RGBCOLOR _bg_color = ExtractColor(ch, X, Y, map);

	GotoXY(X, Y);
	switch (map)
	{
	case 1:	//space
	{
		RGBCOLOR _fg_color = { 128, 128, 128 };
		changeTextColor(_fg_color, _bg_color);
		cout << u8"\u1C6A";
		break;
	}
	case 2:	//hill
	{
		RGBCOLOR _fg_color = { 255, 165, 0 };
		changeTextColor(_fg_color, _bg_color);
		cout << u8"\u2736";
		break;
	}
	case 3:	//sky
	{
		RGBCOLOR _fg_color = { 240, 128, 128 };
		changeTextColor(_fg_color, _bg_color);
		//cout << u8"\u1C6C";
		cout << u8"\u273F";
		break;
	}
	case 4:	//earth
	{
		RGBCOLOR _fg_color = { 255, 215, 0 };
		changeTextColor(_fg_color, _bg_color);
		cout << u8"\u2666";
		break;
	}
	}
	changeTextColor();
}

void DrawBonusFood()
{
	SetConsoleOutputCP(CP_UTF8);

	int X = BonusFood[ID_BonusFood].x, Y = BonusFood[ID_BonusFood].y;
	RGBCOLOR _bg_color = ExtractColor(ch, X, Y, map);
	RGBCOLOR _fg_color = { 220, 20, 60 };
	changeTextColor(_fg_color, _bg_color);

	GotoXY(X, Y);  cout << u8"\u2665";
}

void DrawGate(int x, int y)
{
	SetConsoleOutputCP(CP_UTF8);
	TYPE = Random(1, 4); // Random 4 types of gates

	switch (TYPE)
	{
	case 1:
		DrawGateU1(x, y);
		break;
	case 2:
		DrawGateU2(x, y);
		break;
	case 3:
		DrawGateU3(x, y);
		break;
	case 4:
		DrawGateU4(x, y);
		break;
	}

	GateDraw = true; // Gate exits
}
void DrawGateU1(int x, int y)
{
	SetConsoleOutputCP(CP_UTF8);
	RGBCOLOR _bg_color;
	RGBCOLOR _fg_color;
	for (int i = -1; i <= 1; i++)
	{
		_bg_color = ExtractColor(ch, x + i, y, map);
		_fg_color = { 0, 0, 128 };
		changeTextColor(_fg_color, _bg_color);
		//GotoXY(x + i, y);
		Gate[cntGate++] = { x + i, y }; // Save coordinates of each point from gate to array
		GotoXY(x + i, y); cout << u8"\u058E";
	}

	_bg_color = ExtractColor(ch, x - 1, y - 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	//GotoXY(x - 1, y - 1);
	Gate[cntGate++] = { x - 1, y - 1 };
	GotoXY(x - 1, y - 1); cout << u8"\u058E";

	_bg_color = ExtractColor(ch, x + 1, y - 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	//GotoXY(x + 1, y - 1);
	Gate[cntGate++] = { x + 1, y - 1 };
	GotoXY(x + 1, y - 1); cout << u8"\u058E";
}

void DrawGateU2(int x, int y)
{
	SetConsoleOutputCP(CP_UTF8);
	RGBCOLOR _bg_color;
	RGBCOLOR _fg_color;
	for (int i = -1; i <= 1; i++)
	{
		_bg_color = ExtractColor(ch, x + i, y, map);
		_fg_color = { 0, 0, 128 };
		changeTextColor(_fg_color, _bg_color);
		Gate[cntGate++] = { x + i, y };
		GotoXY(x + i, y); cout << u8"\u058E";
	}
	_bg_color = ExtractColor(ch, x - 1, y + 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	Gate[cntGate++] = { x - 1, y + 1 };
	GotoXY(x - 1, y + 1); cout << u8"\u058E";

	_bg_color = ExtractColor(ch, x + 1, y + 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	Gate[cntGate++] = { x + 1, y + 1 };
	GotoXY(x + 1, y + 1); cout << u8"\u058E";
}

void DrawGateU3(int x, int y)
{
	SetConsoleOutputCP(CP_UTF8);
	RGBCOLOR _bg_color;
	RGBCOLOR _fg_color;
	for (int i = -1; i <= 1; i++)
	{
		_bg_color = ExtractColor(ch, x, y + i, map);
		_fg_color = { 0, 0, 128 };
		changeTextColor(_fg_color, _bg_color);
		Gate[cntGate++] = { x, y + i };
		GotoXY(x, y + i); cout << u8"\u058E";
	}
	_bg_color = ExtractColor(ch, x + 1, y - 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	Gate[cntGate++] = { x + 1, y - 1 };
	GotoXY(x + 1, y - 1); cout << u8"\u058E";

	_bg_color = ExtractColor(ch, x + 1, y + 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	Gate[cntGate++] = { x + 1, y + 1 };
	GotoXY(x + 1, y + 1); cout << u8"\u058E";
}

void DrawGateU4(int x, int y)
{
	SetConsoleOutputCP(CP_UTF8);
	RGBCOLOR _bg_color;
	RGBCOLOR _fg_color;
	for (int i = -1; i <= 1; i++)
	{
		_bg_color = ExtractColor(ch, x, y + i, map);
		_fg_color = { 0, 0, 128 };
		changeTextColor(_fg_color, _bg_color);
		Gate[cntGate++] = { x, y + i };
		GotoXY(x, y + i); cout << u8"\u058E";
	}
	_bg_color = ExtractColor(ch, x - 1, y - 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	Gate[cntGate++] = { x - 1, y - 1 };
	GotoXY(x - 1, y - 1); cout << u8"\u058E";

	_bg_color = ExtractColor(ch, x - 1, y + 1, map);
	_fg_color = { 0, 0, 128 };
	changeTextColor(_fg_color, _bg_color);
	Gate[cntGate++] = { x - 1, y + 1 };
	GotoXY(x - 1, y + 1); cout << u8"\u058E";
}

void ProcessGate() // void ProcessGate(int& LEVEL)
{
	if (GateDraw == false) // If doesn't meet the conditions to create Gate -> continue draw food to eat food 
	{
		DrawFood(); // (4 foods each level to create Gate)
		if (ID_BonusFood < 2)
			DrawBonusFood();
	}
	else // Create Gate already
	{
		if (TouchGate()) // When come into gate and crash it -> DEAD
		{
			AnimationDead();
			StopThread();
			ProcessDead();
			return;
		}

		if (Snake[0].x == Center.x && Snake[0].y == Center.y && Snake_Size > 0) // Snake come into gate
		{
			for (int i = 0; i < Snake_Size - 1; i++)
				Snake[i] = Snake[i + 1];

			Snake_Size--;
		}

		if (Snake_Size == 0) // When all the body of snake come into Gate -> Erase Gate and move to next level
		{
			if (checkMusicEffect) PlaySound(TEXT("sound/gate.wav"), NULL, SND_FILENAME | SND_ASYNC);
			// Erase Gate
			EraseGate();
			GateDraw = false;

			DeltaSpeed += 0.2F; // Increase speed when move to next level
			lev++; // Move to next level

			if (lev == MAX_LEVEL)
			{
				ProcessWin();
				WIN = TRUE;
				return;
			}

			ResetData(); // Intialize original data when move to new level
			LoadGame(); // LoadGame(lev)
		}
	}
}
void EraseGate()
{
	for (int i = 0; i < cntGate; i++)
	{
		GotoXY(Gate[i].x, Gate[i].y);
		cout << " ";
	}
}

void EraseOldSnake()
{
	for (int i = 0; i < Snake_Size; i++)
	{
		GotoXY(Snake[i].x, Snake[i].y);
		text_color(0, 10);
		cout << " ";
		text_color(0, 7);
	}
}



bool isValidFood(int x, int y)
{
	for (int i = 0; i < Snake_Size; i++)
		if (Snake[i].x == x && Snake[i].y == y)
			return false;

	for (int i = 0; i < cntGate; i++)
		if (x == Gate[i].x && y == Gate[i].y)
			return false;

	for (int i = 0; i < obs_nums; i++)
	{
		if (obs[i].x == x && obs[i].y == y)
			return false;
	}

	for (int i = 0; i < const_obs_nums; i++)
	{
		if (const_obs[i].x == x && const_obs[i].y == y)
			return false;
	}

	return true;
}

void GenerateFood()
{
	int x, y;
	for (int i = 0; i < MAX_SIZE_FOOD; i++)
	{
		while (true)
		{
			x = Random(10, WIDTH_GAME - 10);
			y = Random(10, HEIGHT_GAME - 10);

			if (lev == 2 || lev == 3)
				if (abs(y - 11) < 2 || abs(y - 23) < 2)
					continue;

			if (lev == 3)
				if (abs(x - 61) < 2 || abs(x - 20) < 2)
					continue;

			if (isValidFood(x, y) == true)
				break;
		}

		Food[i] = { x, y }; // Random coordinates of food into array
	}
}

void GenerateBonusFood()
{
	int x = -1, y = -1;
	for (int i = 0; i < MAX_SIZE_FOOD - 2; i++)
	{
		while (true)
		{
			x = Random(10, WIDTH_GAME - 10);
			y = Random(10, HEIGHT_GAME - 10);

			if (lev == 2 || lev == 3)
				if (abs(y - 11) < 2 || abs(y - 23) < 2)
					continue;

			if (lev == 3)
				if (abs(x - 61) < 2 || abs(x - 20) < 2)
					continue;

			bool check = false;
			if (isValidFood(x, y) == true)
			{
				for (int j = 0; j < MAX_SIZE_FOOD; j++)
					if (y != Food[j].y && x != Food[j].x)
						check = true;
			}

			if (check == true)
				break;
		}

		BonusFood[i] = { x, y }; // Random coordinates of food into array
	}
}

void EatFood(int type)
{
	SCORE += (type == 1 ? 50 : 100);
	//draw_matchBoard(x_pos, y_pos, HEIGHT_BOARD, WIDTH_BOARD, SCORE, lev, 2, 0, "", 1);

	Snake[Snake_Size] = (type == 1 ? Food[ID_Food] : BonusFood[ID_BonusFood]); // Insert new size of body (Insert food coordinates)

	if (type == 2)
	{
		ID_BonusFood++;
		GenerateBonusFood();
	}
	else
	{
		if (ID_Food == MAX_SIZE_FOOD - 1) // 4 foods can generate Gate
		{
			Snake_Size++;
			GenerateCenterGate();
			DrawGate(Center.x, Center.y); // Draw Gate
		}
		else // Increase size of snake, index of food
		{
			ID_Food++;
			Snake_Size++;
			GenerateFood();
		}
	}
}

void GenerateCenterGate()
{
	int x, y;
	do
	{
		x = Random(8, WIDTH_GAME - 8);
		y = Random(8, HEIGHT_GAME - 8);
	} while (CenterGate(x - 2, y) == false || CenterGate(x, y) == false || CenterGate(x + 2, y) == false
		|| CenterGate(x - 2, y - 2) == false || CenterGate(x, y - 2) == false || CenterGate(x + 2, y - 2) == false
		|| CenterGate(x - 2, y + 2) == false || CenterGate(x, y + 2) == false || CenterGate(x + 2, y + 2) == false);

	// Check 7x7 matrix with center is Center Point Of Gate (avoid gate opposite the bound)
	// It makes sure that there are no Food, Obstacles and Snake in this 7x7 matrix

	Center = { x, y };
}

// 8 directions from point (x, y) 
int dx[] = { -1, 0, 0, 1, -1, -1, 1, 1 };
int dy[] = { 0, -1, 1, 0, -1, 1, -1, 1 };

bool CenterGate(int x, int y)
{
	if (TouchWall(x, y) == true)
		return false;

	/*if (isValidFood(x, y) == true)
		return false;*/

	if (TouchObs(x, y) == true)
		return false;

	for (int dir = 0; dir < 8; dir++) // Check 8 directions from Center Point of Gate (Not food, obstacles, snake, wall)
	{
		int new_x = x + dx[dir];
		int new_y = y + dy[dir];

		if (TouchWall(new_x, new_y) == true)
			return false;

		/*if (isValidFood(new_x, new_y) == true)
			return false;*/

		if (TouchObs(new_x, new_y) == true)
			return false;

		for (int i = 0; i < obs_nums; i++)
			if (abs(obs[i].x - new_x) <= 1 && abs(obs[i].y - new_y) <= 1)
				return false;

		for (int i = 0; i < const_obs_nums; i++)
			if (abs(const_obs[i].x - new_x) <= 1 && abs(const_obs[i].y - new_y) <= 1)
				return false;

		for (int i = 0; i < Snake_Size; i++)
			if (abs(Snake[i].x - new_x) <= 4 && abs(Snake[i].y - new_y) <= 4)
				return false;
	}

	return true;
}

bool TouchWall(int x, int y)
{
	for (int i = 0; i < TELEPOS; i++)
	{
		if (x == TELEGATE[i].in.x && y == TELEGATE[i].in.y)
			return false;

		if (x == TELEGATE[i].out.x && y == TELEGATE[i].out.y)
			return false;
	}

	return !(x > x_pos && x < WIDTH_GAME && y > y_pos && y < HEIGHT_GAME - 1);
}

bool TouchItself()
{
	for (int i = 1; i < Snake_Size; i++)
		if (Snake[i].x == Snake[0].x && Snake[i].y == Snake[0].y)
			return true;

	return false;
}
bool TouchObs(int x, int y)
{
	for (int i = 0; i < obs_nums; i++)
		if (x == obs[i].x && y == obs[i].y)
			return true;

	for (int i = 0; i < const_obs_nums; i++)
		if (x == const_obs[i].x && y == const_obs[i].y)
			return true;

	return false;
}

bool TouchGate()
{
	for (int i = 0; i < cntGate; i++)
	{
		if (Gate[i].x == Center.x && Gate[i].y == Center.y)
		{
			if (Snake[0].x == Gate[i].x && Snake[0].y == Gate[i].y)
			{
				// Check that snake come into Gate with right direction
				if (TYPE == 1) // Gate type 1
					return !(MOVING == 'S');

				if (TYPE == 2) // Gate type 2
					return !(MOVING == 'W');

				if (TYPE == 3) // Gate type 3
					return !(MOVING == 'A');

				if (TYPE == 4) // Gate type 4
					return !(MOVING == 'D');
			}

			for (int j = 1; j < Snake_Size; j++)
			{
				if (Snake[j].x == Gate[i].x && Snake[j].y == Gate[i].y)
					return true;
			}
		}
		else
		{
			for (int j = 0; j < Snake_Size; j++)
			{
				if (Snake[j].x == Gate[i].x && Snake[j].y == Gate[i].y)
					return true;
			}
		}
	}

	return false;
}

bool TouchTeleport(int x, int y)
{
	for (int i = 0; i < TELEPOS; i++)
	{
		if (x == TELEGATE[i].in.x && y == TELEGATE[i].in.y)
			return true;

		if (x == TELEGATE[i].out.x && y == TELEGATE[i].out.y)
			return true;
	}

	return false;
}
void MoveUp()
{
	if (TouchWall(Snake[0].x, Snake[0].y - 1) == true || TouchObs(Snake[0].x, Snake[0].y - 1) == true) // Touch the Wall or Obstacle -> You lose
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}

	if (Snake[0].x == Food[ID_Food].x && Snake[0].y - 1 == Food[ID_Food].y) // If move to coordinates of Food -> Eat Food
	{
		EatFood(1);
		if (checkMusicEffect) if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (Snake[0].x == BonusFood[ID_BonusFood].x && Snake[0].y - 1 == BonusFood[ID_BonusFood].y) // If move to coordinates of Bonus Food -> Eat Bonus Food
	{
		EatFood(2);
		if (checkMusicEffect) if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	for (int i = Snake_Size - 2; i >= 0; i--) // Change the coordinates of Snake
		Snake[i + 1] = Snake[i];

	if (TouchTeleport(Snake[0].x, Snake[0].y - 1) == true)
	{
		for (int i = 0; i < TELEPOS; i++)
		{
			if (Snake[0].x == TELEGATE[i].in.x && Snake[0].y - 1 == TELEGATE[i].in.y)
			{
				Snake[0].y = TELEGATE[i].out.y - 1;
				break;
			}
		}
	}
	else Snake[0].y--; // Move up

	if (TouchItself() || TouchGate()) // Check conditions to make sure that snake still ALIVE
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}
}
void MoveDown()
{
	if (TouchWall(Snake[0].x, Snake[0].y + 1) == true || TouchObs(Snake[0].x, Snake[0].y + 1) == true)
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}

	if (Snake[0].x == Food[ID_Food].x && Snake[0].y + 1 == Food[ID_Food].y)
	{
		EatFood(1);
		if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (Snake[0].x == BonusFood[ID_BonusFood].x && Snake[0].y + 1 == BonusFood[ID_BonusFood].y) // If move to coordinates of Bonus Food -> Eat Bonus Food
	{
		EatFood(2);
		if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	for (int i = Snake_Size - 2; i >= 0; i--)
		Snake[i + 1] = Snake[i];

	if (TouchTeleport(Snake[0].x, Snake[0].y + 1) == true)
	{
		for (int i = 0; i < TELEPOS; i++)
		{
			if (Snake[0].x == TELEGATE[i].out.x && Snake[0].y + 1 == TELEGATE[i].out.y)
			{
				Snake[0].y = TELEGATE[i].in.y + 1;
				break;
			}
		}
	}
	else Snake[0].y++; // Move down

	if (TouchItself() || TouchGate())
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}
}
void MoveRight()
{
	if (TouchWall(Snake[0].x + 1, Snake[0].y) == true || TouchObs(Snake[0].x + 1, Snake[0].y) == true)
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}

	if (Snake[0].x + 1 == Food[ID_Food].x && Snake[0].y == Food[ID_Food].y)
	{
		EatFood(1);
		if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (Snake[0].x + 1 == BonusFood[ID_BonusFood].x && Snake[0].y == BonusFood[ID_BonusFood].y) // If move to coordinates of Bonus Food -> Eat Bonus Food
	{
		EatFood(2);
		if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	for (int i = Snake_Size - 2; i >= 0; i--)
		Snake[i + 1] = Snake[i];

	if (TouchTeleport(Snake[0].x + 1, Snake[0].y) == true)
	{
		for (int i = 0; i < TELEPOS; i++)
		{
			if (Snake[0].x + 1 == TELEGATE[i].out.x && Snake[0].y == TELEGATE[i].out.y)
			{
				Snake[0].x = TELEGATE[i].in.x + 1;
				break;
			}
		}
	}
	else Snake[0].x++; // Move right

	if (TouchItself() || TouchGate())
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}
}
void MoveLeft()
{
	if (TouchWall(Snake[0].x - 1, Snake[0].y) == true || TouchObs(Snake[0].x - 1, Snake[0].y) == true)
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}

	if (Snake[0].x - 1 == Food[ID_Food].x && Snake[0].y == Food[ID_Food].y)
	{
		EatFood(1);
		if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (Snake[0].x - 1 == BonusFood[ID_BonusFood].x && Snake[0].y == BonusFood[ID_BonusFood].y) // If move to coordinates of Bonus Food -> Eat Bonus Food
	{
		EatFood(2);
		if (checkMusicEffect) PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	for (int i = Snake_Size - 2; i >= 0; i--)
		Snake[i + 1] = Snake[i];

	if (TouchTeleport(Snake[0].x - 1, Snake[0].y) == true)
	{
		for (int i = 0; i < TELEPOS; i++)
		{
			if (Snake[0].x - 1 == TELEGATE[i].in.x && Snake[0].y == TELEGATE[i].in.y)
			{
				Snake[0].x = TELEGATE[i].out.x - 1;
				break;
			}
		}
	}
	else Snake[0].x--; // Move left

	if (TouchItself() || TouchGate())
	{
		AnimationDead();
		StopThread();
		ProcessDead();
		return;
	}
}

void increaseScore(int& SCORE)
{
	changeTextColor({ 0, 0, 0 }, { 37, 67, 110 });
	GotoXY(107, 20);
	cout << "SCORE: " << SCORE;
	changeTextColor();
}

void AnimationDead()
{
	for (int i = 1; i <= 3; i++) {
		DrawSnake(MSSV);//draw 
		if (checkMusicEffect) if (checkMusicEffect) PlaySound(TEXT("sound/beep.wav"), NULL, SND_FILENAME | SND_ASYNC);
		fflush(stdout);
		Sleep(450);
		EraseOldSnake();//erase
		fflush(stdout);
		if (i == 3) Sleep(370);
		else Sleep(450);
	}
	DrawSnake(MSSV);
	Sleep(250);
	if (checkMusicEffect) if (checkMusicEffect) PlaySound(TEXT("sound/dead.wav"), NULL, SND_FILENAME | SND_SYNC);
	EraseOldSnake();
}
