#include "Snake.h"
#include "ConsoleWindow.h"
#include "graphics.h"
#include "arts.h"
#include "ProcessPlayer.h"

int main()
{
	//SetConsoleOutputCP(CP_UTF8);
	CleanDataFile();
	LoadConsole();
	mainMenu();
	return 0;
}