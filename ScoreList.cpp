#include "ScoreList.h"

void swap(ScoreList& a, ScoreList& b)
{
	ScoreList temp = a;
	a = b;
	b = temp;
}

void SortScore(ScoreList S[], int id)
{
	for (int i = 1; i < id; i++)
	{
		for (int j = i + 1; j <= id; j++)
			if (S[i].score < S[j].score)
				swap(S[i], S[j]);
	}
}

void ProcessHighScore()
{
	system("cls");
	GotoXY(50, 15);

	cout << "YOU'VE SET UP A NEW HIGH SCORE\n";
}

void ScoreBoard(ScoreList S[], Player PlayerSnake[], int id)
{
	SetConsoleOutputCP(CP_UTF8);
	system("cls");

	drawStage();

	int x = 10, y = y_pos - 2;
	int xmax = WIDTH_GAME + 15;
	int ymax = HEIGHT_BOARD;

	changeTextColor({ 247, 183, 135 }, { 82, 120, 83 });
	GotoXY((120 - 58) / 2, 1); cout << u8" ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄ ";
	GotoXY((120 - 58) / 2, 2); cout << u8" █░▄▄▀█▀▄▀█░█████▄██░▄▄█▀███▀█░▄▄█░▄▀▄░█░▄▄█░▄▄▀█▄░▄█░▄▄█ ";
	GotoXY((120 - 58) / 2, 3); cout << u8" █░▀▀░█░█▀█░▄▄░██░▄█░▄▄██░▀░██░▄▄█░█▄█░█░▄▄█░██░██░██▄▄▀█ ";
	GotoXY((120 - 58) / 2, 4); cout << u8" █░██░██▄██▄██▄█▄▄▄█▄▄▄███▄███▄▄▄█▄███▄█▄▄▄█▄██▄██▄██▄▄▄█ ";
	GotoXY((120 - 58) / 2, 5); cout << u8" ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀ ";
	changeTextColor();

	drawMedal((120 - 59) / 2 + 1, (30 - 13) / 2, 1);
	drawMedal((120 - 59) / 2 + 24, (30 - 13) / 2, 2);
	drawMedal((120 - 59) / 2 + 49, (30 - 13) / 2, 3);

	ifstream INP;
	INP.open("data.txt");
	INP >> id;
	INP.close();

	for (int i = 1; i <= id; i++)
	{
		string name = "";
		int score = 0;
		int level = 1;
		extractDataFile(PlayerSnake, i, LoadPlayer);

		S[i].name = (LoadPlayer.Name != "") ? LoadPlayer.Name : "EMPTY";
		S[i].score = (LoadPlayer.Name != "") ? LoadPlayer.score : -1;
	}

	SortScore(S, id);
	TextToAsciiArt((120 - 59 + 9 - AsciiArtTextSize(S[1].name)) / 2 + 1, 19, S[1].name, { 247, 183, 135 }, { 82, 120, 83 });
	TextToAsciiArt((120 - 59 + 9 - AsciiArtTextSize(to_string(S[0].score))) / 2 + 1, 23, to_string(S[0].score), { 247, 183, 135 }, { 82, 120, 83 });
	TextToAsciiArt((120 - 59 + 9 - AsciiArtTextSize(S[2].name)) / 2 + 24, 19, S[2].name, { 247, 183, 135 }, { 82, 120, 83 });
	TextToAsciiArt((120 - 59 + 9 - AsciiArtTextSize(to_string(S[2].score))) / 2 + 24, 23, to_string(S[2].score), { 247, 183, 135 }, { 82, 120, 83 });
	TextToAsciiArt((120 - 59 + 9 - AsciiArtTextSize(S[3].name)) / 2 + 49, 19, S[3].name, { 247, 183, 135 }, { 82, 120, 83 });
	TextToAsciiArt((120 - 59 + 9 - AsciiArtTextSize(to_string(S[3].score))) / 2 + 49, 23, to_string(S[3].score), { 247, 183, 135 }, { 82, 120, 83 });

	changeTextColor(TXT_RGB);
	GotoXY((120 - 33) / 2 + 1, 28);
	system("pause");
	changeTextColor();
}