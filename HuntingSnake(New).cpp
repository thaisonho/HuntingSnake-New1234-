#include "Snake.h"
#include "ConsoleWindow.h"
#include "graphics.h"
#include "arts.h"
#include "ProcessPlayer.h"

int main()
{
	CleanDataFile();
	LoadConsole();
	mainMenu();
	return 0;
}