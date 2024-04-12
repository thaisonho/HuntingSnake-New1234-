#pragma once
#include <iostream>
#include <random>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <string>
#include <thread>
#include <assert.h>
#include <fstream>
#include "ConsoleWindow.h"
#include "graphics.h"

#pragma comment(lib, "winmm.lib")
// Statements to random number in range [l, r] 
#define Random(l, r) uniform_int_distribution<long long> (l, r) (rng)

using namespace std;

// CONSTANTS
const int MAX_SIZE_FOOD = 4;
const int MAX_SIZE_SNAKE = 30;
const int MAX_SPEED = 5;
const int MAX_SIZE_OBS = 10;
const int MAX_LEVEL = 5;

const int WIDTH_BOARD = 81, HEIGHT_BOARD = 24;  // width board = width --------- // height board = height
const int x_pos = 0;
const int y_pos = 5;


//char ch[100][100];

const int WIDTH_GAME = WIDTH_BOARD + x_pos, HEIGHT_GAME = HEIGHT_BOARD + y_pos;

const string MSSV = "23127148231271522312744223127469";

// Declare struct Point2D
struct Point
{
	int x;
	int y;
};

struct PAIR
{
	Point in;
	Point out;
};

struct Player
{
	string Name;

	char direction;
	char charLock;
	int score;
	int level;
	int map;
	int ID_FOOD;
	int ID_BONUSFOOD;
	float speed;
	float delta_speed;
	int sizeOfSnake;
	int cntGate;
	int cntObs;

	Point Snake[50];
	Point obs[50];
	Point Gate[50];
	Point Food[50];
	Point BonusFood[50];
};

void StartGame(bool LoadSnake);// Function StartGame
void LoadGame();// Function LoadGame (movement of snake using keyboard)
void PauseGame(HANDLE t);
void ExitGame();
void SaveGame(HANDLE t);
void StopThread();
void ProcessPause(HANDLE t); // Function handle when pause game
void ProcessDead(void);// Function handle when snake DEAD
void ProcessWin(void); // Function handle when snake win
bool ProcessExit();
void ResetData(void);// Function Intialize original data
void ResetLoadData(void);

void outputInfoPlayer(Player PlayerSnake[], int id);
void extractDataFile(Player PlayerSnake[], int i, Player& LoadPlayer);
void ExtractInfoPlayer(Player PlayerSnake[], int& id, string& namePlayer); // Save information of player into struct Player
void ThreadFunction(void);// Subfunction for thread

void DrawSnake(const string& s);// Function Draw Snake
void DrawFood(void);// Function Draw Food
void DrawBonusFood(void);// Function Draw Food
void DrawGate(int x, int y);// Function Draw Gate
void DrawGateU1(int x, int y); // Functio Draw Gate Type 1
void DrawGateU2(int x, int y); // Functio Draw Gate Type 2
void DrawGateU3(int x, int y); // Functio Draw Gate Type 3
void DrawGateU4(int x, int y); // Functio Draw Gate Type 4

void ProcessGate(); // Function Process Gate
void EraseGate(); //Function Erase Gate
void EraseOldSnake(void); //Function Erase Old Snake (Old coordinates)

bool isValidFood(int x, int y);// Function check if can create food
void GenerateFood(void); // Function Generate Food
void GenerateBonusFood(void); // Function Generate Food
void EatFood(int type); // Function Eat Food

void GenerateCenterGate(void); // Function Generate Center Point of Gate

bool CenterGate(int x, int y); // Function check if can create Center Point of Gate
bool TouchGate(void); // Function check if snake touches Gate
bool TouchObs(int x, int y); // Function check if snake touches Obstacles
bool TouchWall(int x, int y); // Function check if snake touches wall
bool TouchItself(void); // Function check if head of snake touches the body
bool TouchTeleport(int x, int y); // Function check if touch teleport

void MoveUp(void); // Fucntion Move Up
void MoveDown(void); // Function Move Down
void MoveRight(void); // Function Move Right
void MoveLeft(void); // Function Move Left

void increaseScore(int& SCORE);
void AnimationDead();


