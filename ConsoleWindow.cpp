#include "ConsoleWindow.h"
#include "GenerateMap.h"
#include "snake.h"
#include "arts.h"
#include "ProcessPlayer.h"
#include "ScoreList.h"
#include "Variables.h"

ScoreList S1[50];
Player PlayerSnake1[50];
Player LoadPlayer1;

bool checkMusicEffect = true;
int map = 1;

/*========== SNAKE COLOR ==========*/

RGBCOLOR
DEFAULT1_FG = { 0, 255, 0 },
DEFAULT2_FG = { 249, 232, 217 }, DEFAULT2_BG = { 82, 120, 83 },
DEFAULT3_FG = { 249, 232, 217 }, DEFAULT3_BG = { 238, 114, 20 },
DEFAULT4_FG = { 82, 120, 83 }, DEFAULT4_BG = { 247, 183, 135 };

RGBCOLOR _userChoiceFG = DEFAULT1_FG, _userChoiceBG = BG_RGB_2;

/*=================================*/

axis getTermSize()
{
    axis res;

    // get terminal's size
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    // calculate the size of terminal
    res.x = info.srWindow.Right - info.srWindow.Left + 1;
    res.y = info.srWindow.Bottom - info.srWindow.Top + 1;

    return res;
}

void text_color(int background_color, int text_color)
{
    HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
    int color_code = background_color * 16 + text_color;
    SetConsoleTextAttribute(color, color_code);
}

void GotoXY(int x, int y)
{
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void LoadConsole()
{
    ios_base::sync_with_stdio(0);
    cin.tie(NULL); cout.tie(NULL);
    SetWindowSize(120, 30);
    SetScreenBufferSize(120, 30);

    DisableResizeWindow();
    DisableCtrButton(0, 0, 0);
    ShowScrollbar(0);
    SetConsolePosition(200, 200);
    LockConsolePosition();

    // turn off cursor blinking
    ShowConsoleCursor(false);

    setBackgroundColor(BG_RGB);
    changeTextColor(TXT_RGB);
}

void DisableResizeWindow()
{
    //get handle window console
    HWND hWnd = GetConsoleWindow();
    // don't allow to change size console through drag the edges or corners
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void DisableCtrButton(bool Close, bool Min, bool Max)
{
    //get handle window console
    HWND hWnd = GetConsoleWindow();
    //get menu of console
    HMENU hMenu = GetSystemMenu(hWnd, false);

    //1 - off, 0 - on
    if (Close == 1)
    {
        //delete close button
        DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
    }
    if (Min == 1)
    {
        //delete min button
        DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
    }
    if (Max == 1)
    {
        //delete max button
        DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
    }
}

void ShowScrollbar(BOOL Show)
{
    HWND hWnd = GetConsoleWindow();
    //SB_BOTH to hide both horizontal & vertical scrollbars
    //0 - off, 1 - on
    ShowScrollBar(hWnd, SB_BOTH, Show);
}

void SetConsolePosition(int x, int y)
{
    HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, 0, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void LockConsolePosition() {
    HWND consoleWindow = GetConsoleWindow();
    //get style window console
    LONG_PTR windowStyle = GetWindowLongPtr(consoleWindow, GWL_STYLE);

    // delete flag WS_CAPTION out of the window
    windowStyle &= ~WS_CAPTION;

    SetWindowLongPtr(consoleWindow, GWL_STYLE, windowStyle);

    // set size window
    SetWindowPos(consoleWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
}

void MaximizeConsoleWindow()
{
    HWND consoleWindow = GetConsoleWindow();
    //set console at maximize size
    ShowWindow(consoleWindow, SW_MAXIMIZE);
}

void SetWindowSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = width;
    WindowSize.Bottom = height;

    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}

void SetScreenBufferSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD NewSize;
    NewSize.X = width;
    NewSize.Y = height;

    SetConsoleScreenBufferSize(hStdout, NewSize);
}

void ShowConsoleCursor(bool show)
{
    // using Win32 API to make the cursor disappear
    // Reference: https://stackoverflow.com/questions/18028808/remove-blinking-underscore-on-console-cmd-prompt
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = show; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void mainMenu() {
    int x_menu = (getTermSize().x - 20) / 2 + 22,
        y_menu = (getTermSize().y - 4) / 2;


    // define variables use for navigate through the menu
MENU:
    bool check = true, isEnter = false;
    static int selection = 1, prev_selection = 6, next_selection = 2;

    drawMenuSnake(0, 0);
    Sleep(500);
    drawMenu(x_menu, y_menu, selection, TXT_RGB);
    drawMenu(x_menu, y_menu - 6, prev_selection, FADED_TXT_RGB);
    drawMenu(x_menu, y_menu + 6, next_selection, FADED_TXT_RGB);

    while (isEnter == false) {
        GotoXY(x_menu, y_menu);

        // navigate through the menu
        if (_kbhit()) { // if any key is pressed
            if (checkMusicEffect) PlaySound(TEXT("sound/move.wav"), NULL, SND_FILENAME | SND_ASYNC);
            // variable to receive user input
            char handle;
            handle = toupper(_getch());
            Sleep(50);

            switch (handle)
            {
            case 'S': case 'P': // if user pressed S or 'Arrow Down'
                deleteArtMenu(x_menu, y_menu - 6, prev_selection);
                deleteArtMenu(x_menu, y_menu + 6, next_selection);
                deleteArtMenu(x_menu, y_menu, selection);

                ++selection;
                if (selection > 6) selection = 1;
                prev_selection = selection - 1;
                next_selection = selection + 1;
                if (prev_selection == 0)   prev_selection = 6;
                if (next_selection == 7)   next_selection = 1;

                drawMenu(x_menu, y_menu - 6, prev_selection, FADED_TXT_RGB);
                drawMenu(x_menu, y_menu + 6, next_selection, FADED_TXT_RGB);
                drawMenu(x_menu, y_menu, selection, TXT_RGB);
                break;
            case 'W': case 'H': // if user pressed W or 'Arrow Up'
                deleteArtMenu(x_menu, y_menu - 6, prev_selection);
                deleteArtMenu(x_menu, y_menu + 6, next_selection);
                deleteArtMenu(x_menu, y_menu, selection);

                --selection;
                if (selection < 1) selection = 6;
                prev_selection = selection - 1;
                next_selection = selection + 1;
                if (prev_selection == 0)   prev_selection = 6;
                if (next_selection == 7)   next_selection = 1;

                drawMenu(x_menu, y_menu - 6, prev_selection, FADED_TXT_RGB);
                drawMenu(x_menu, y_menu + 6, next_selection, FADED_TXT_RGB);
                drawMenu(x_menu, y_menu, selection, TXT_RGB);
                break;
            case 13: // if user pressed 'Enter'
                isEnter = true;
                break;
            }
        }
    }
    switch (selection)
    {
    case 1:
        if (checkMusicEffect) PlaySound(TEXT("sound/click.wav"), NULL, SND_FILENAME | SND_ASYNC);
        NewGame(PlayerSnake1, id, namePlayer);
        StartGame(0);
        LoadGame();
        goto MENU;
        break;
    case 2:
        if (checkMusicEffect) PlaySound(TEXT("sound/click.wav"), NULL, SND_FILENAME | SND_ASYNC);
        LoadGamePlayer(PlayerSnake1, id, LoadPlayer1);
        system("cls");
        goto MENU;
        break;
    case 3:
        if (checkMusicEffect) PlaySound(TEXT("sound/click.wav"), NULL, SND_FILENAME | SND_ASYNC);
        ScoreBoard(S1, PlayerSnake1, id);
        system("cls");
        goto MENU;
        break;
    case 4:
        if (checkMusicEffect) PlaySound(TEXT("sound/click.wav"), NULL, SND_FILENAME | SND_ASYNC);
        settingScreen();
        system("cls");
        goto MENU;
        break;
    case 6:
        /*ExitGame(handle_thread_obj);*/
        break;
    }
    GotoXY(0, 0);
}

void settingScreen()
{
    system("cls");
    bool check = true;
    int x_menu = 4,
        y_menu = (getTermSize().y - 15) / 2;

    // define variables use for navigate through the menu
    bool isEnter = false;
    static int y_pointer = y_menu;
    int settingSelection = 1;
    static int optionMAP = 1;

    changeTextColor({ 238, 114, 20 });
    GotoXY((120 - 33) / 2, 2); cout << u8"░▒█▀▀▀█░█▀▀░▀█▀░▀█▀░░▀░░█▀▀▄░█▀▀▀";
    GotoXY((120 - 33) / 2, 3); cout << u8"░░▀▀▀▄▄░█▀▀░░█░░░█░░░█▀░█░▒█░█░▀▄";
    GotoXY((120 - 33) / 2, 4); cout << u8"░▒█▄▄▄█░▀▀▀░░▀░░░▀░░▀▀▀░▀░░▀░▀▀▀▀";
    changeTextColor(TXT_RGB, { 247, 183, 135 });
    GotoXY(x_menu, 28);        cout << "Press <ESC> to go back to menu";
    changeTextColor();

    highlightedBox(55, y_menu, 20, 61, FADED_TXT_RGB);
    highlightedBox(55, y_menu, 20, 62, FADED_TXT_RGB);
    GotoXY(55, y_menu); for (size_t i = 0; i <= 62; ++i) cout << " ";
    highlightedBox(55, y_menu, 19, 60, TXT_RGB);
    highlightedBox(54, y_menu, 19, 60, TXT_RGB);
    drawSoundOn(x_menu, y_menu, checkMusicEffect, TXT_RGB);
    drawAppearanceIcon(x_menu, y_menu + 8, false);

    //drawMiniMap1(20, 13);
    //drawMiniMap3(20, 13);
    //drawMiniMap4(20, 13);
    //highlightedBox(19, 12, 9, 31, { 255, 255,  0 });

    drawMiniMapFaded1(20, 13);

    GotoXY(x_menu, 30 - 3); changeTextColor(TXT_RGB); cout << "Use WS to navigate"; changeTextColor();
    while (isEnter == false) {
        GotoXY(x_menu, y_menu);

        // navigate through the menu
        if (_kbhit()) { // if any key is pressed

            // variable to receive user input
            char handle;
            handle = toupper(_getch());
            Sleep(50);

            switch (handle)
            {
            case 'S': // if user pressed S or 'Arrow Down'
                if (checkMusicEffect) PlaySound(TEXT("sound/move.wav"), NULL, SND_FILENAME | SND_ASYNC);
                if (settingSelection == 1) {
                    drawSoundOn(x_menu, y_menu, checkMusicEffect, FADED_TXT_RGB);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }
                else if (settingSelection == 2) {
                    drawAppearanceIcon(x_menu, y_menu + 8, false);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }
                else {
                    highlightedBox(19, 12, 9, 31, { 255, 0,  0 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMap1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMap2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMap3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMap4(20, 13);
                        map = 4;
                        break;
                    }
                }
                ++settingSelection;
                if (settingSelection > 3) settingSelection = 1;

                if (settingSelection == 1) {
                    drawSoundOn(x_menu, y_menu, checkMusicEffect, TXT_RGB);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }
                else if (settingSelection == 2) {
                    drawAppearanceIcon(x_menu, y_menu + 8, true);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }
                else {
                    highlightedBox(19, 12, 9, 31, { 255, 0, 0 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMap1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMap2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMap3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMap4(20, 13);
                        map = 4;
                        break;
                    }
                }
                break;
            case 'W': // if user pressed W or 'Arrow Up'
                if (checkMusicEffect) PlaySound(TEXT("sound/move.wav"), NULL, SND_FILENAME | SND_ASYNC);
                if (settingSelection == 1) {
                    drawSoundOn(x_menu, y_menu, checkMusicEffect, FADED_TXT_RGB);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }
                else {
                    drawAppearanceIcon(x_menu, y_menu + 8, false);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }

                --settingSelection;
                if (settingSelection < 1) settingSelection = 3;

                if (settingSelection == 1) {
                    drawSoundOn(x_menu, y_menu, checkMusicEffect, TXT_RGB);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }
                else if (settingSelection == 2) {
                    drawAppearanceIcon(x_menu, y_menu + 8, true);
                    highlightedBox(19, 12, 9, 31, { 249, 232, 217 });
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMapFaded1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMapFaded2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMapFaded3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMapFaded4(20, 13);
                        map = 4;
                        break;
                    }
                }
                else {
                    highlightedBox(19, 12, 9, 31, { 255, 0, 0 });
                    //drawMiniMap1(20, 13);
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMap1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMap2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMap3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMap4(20, 13);
                        map = 4;
                        break;
                    }
                }
                break;

            case 13: // if user pressed 'Enter'
                if (checkMusicEffect) PlaySound(TEXT("sound/click.wav"), NULL, SND_FILENAME | SND_ASYNC);
                if (settingSelection == 1) {
                    checkMusicEffect ^= 1;
                    drawSoundOn(x_menu, y_menu, checkMusicEffect, TXT_RGB);
                }
                else if (settingSelection == 2) {
                    drawAppearanceIcon(x_menu, y_menu + 8, false);
                    changeColorSubScreen();
                    drawAppearanceIcon(x_menu, y_menu + 8, true);
                }
                else {
                    optionMAP++;
                    if (optionMAP > 4) {
                        optionMAP = 1;
                    }
                    switch (optionMAP)
                    {
                    case 1:
                        drawMiniMap1(20, 13);
                        map = 1;
                        break;
                    case 2:
                        drawMiniMap2(20, 13);
                        map = 2;
                        break;
                    case 3:
                        drawMiniMap3(20, 13);
                        map = 3;
                        break;
                    case 4:
                        drawMiniMap4(20, 13);
                        map = 4;
                        break;
                    }
                }
                break;
            case 27: // <ESC>
                isEnter = true;
                settingSelection = -1;
                break;
            }
        }
    }
}

void changeColorSubScreen()
{
    int colorSelection = 1;
    int prev_selection = colorSelection;
    bool isEnter = false, check = true, isManualColor = false;

    static unsigned int
        _manualfg_red = 0, _manualfg_green = 0, _manualfg_blue = 0,
        _manualbg_red = 0, _manualbg_green = 0, _manualbg_blue = 0;

    SetConsoleOutputCP(CP_UTF8);
    // PREVIEW SECTION
    GotoXY(56 + (60 - 8) / 2, 8);
    changeTextColor(TXT_RGB); cout << "PREVIEW";
    changeTextColor();
    drawDemoSnake(56 + (60 - 9) / 2, 9, _userChoiceFG, _userChoiceBG);

    changeTextColor(TXT_RGB);
    for (int i = 0; i < 58; ++i) {
        GotoXY(56 + i, 11);
        cout << u8"\u2550";
    }
    changeTextColor();

    // GIVEN COLOR SECTION
    drawDemoSnake(56 + (60 - 23) / 2, 13, DEFAULT1_FG, BG_RGB_2);
    drawDemoSnake(56 + (60 - 23) / 2 + 14, 13, DEFAULT2_FG, DEFAULT2_BG);
    drawDemoSnake(56 + (60 - 23) / 2, 15, DEFAULT3_FG, DEFAULT3_BG);
    drawDemoSnake(56 + (60 - 23) / 2 + 14, 15, DEFAULT4_FG, DEFAULT4_BG);
    //filled_rec(55, 17, 0, 60, TXT_RGB);

    // RGB SECTION
    changeTextColor(TXT_RGB);
    for (int i = 0; i < 58; ++i) {
        GotoXY(56 + i, 17);
        cout << u8"\u2550";
    }
    GotoXY(56 + (60 - 1) / 2, 17); cout << u8"\u2566";
    for (int i = 0; i < 8; ++i) {
        GotoXY(56 + (60 - 1) / 2, 18 + i);
        cout << u8"\u2551";
    }
    changeTextColor();

    changeTextColor(TXT_RGB);
    GotoXY(56 + (30 - 18) / 2, 18);
    cout << "MANUAL FOREGROUND";
    GotoXY(86 + (30 - 18) / 2, 18);
    cout << "MANUAL BACKGROUND";
    changeTextColor();

    changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
    GotoXY(56 + 3, 20); cout << "  RED: "; changeTextColor(); changeTextColor(TXT_RGB); cout << ' ' << _manualfg_red;
    changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
    GotoXY(56 + 3, 22); cout << " GREEN:"; changeTextColor(); changeTextColor(TXT_RGB); cout << ' ' << _manualfg_green;
    changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
    GotoXY(56 + 3, 24); cout << " BLUE: "; changeTextColor(); changeTextColor(TXT_RGB); cout << ' ' << _manualfg_blue;
    changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
    GotoXY(86 + 3, 20); cout << "  RED: "; changeTextColor(); changeTextColor(TXT_RGB); cout << ' ' << _manualbg_red;
    changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
    GotoXY(86 + 3, 22); cout << " GREEN:"; changeTextColor(); changeTextColor(TXT_RGB); cout << ' ' << _manualbg_green;
    changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
    GotoXY(86 + 3, 24); cout << " BLUE: "; changeTextColor(); changeTextColor(TXT_RGB); cout << ' ' << _manualbg_blue;
    changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
    changeTextColor();

    GotoXY(56 + (60 - 23) / 2 - 3, 13);
    changeTextColor({ 238, 114, 20 });
    cout << u8"➤";
    changeTextColor();

    GotoXY(56 + (60 - 22) / 2, 28);
    changeTextColor(TXT_RGB);
    cout << "Press <ESC> to go back";
    changeTextColor();

    while (isEnter == false) {
        if (check) {
            switch (prev_selection)
            {
            case 1:
                GotoXY(56 + (60 - 23) / 2 - 3, 13);     cout << ' ';
                break;
            case 2:
                GotoXY(56 + (60 - 23) / 2 + 11, 13);    cout << ' ';
                break;
            case 3:
                GotoXY(56 + (60 - 23) / 2 - 3, 15);     cout << ' ';
                break;
            case 4:
                GotoXY(56 + (60 - 23) / 2 + 11, 15);    cout << ' ';
                break;
            case 5:
                changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
                GotoXY(56 + 3, 20); cout << "  RED: ";
                break;
            case 6:
                changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
                GotoXY(56 + 3, 22); cout << " GREEN:";
                break;
            case 7:
                changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
                GotoXY(56 + 3, 24); cout << " BLUE: ";
                break;
            case 8:
                changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
                GotoXY(86 + 3, 20); cout << "  RED: ";
                break;
            case 9:
                changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
                GotoXY(86 + 3, 22); cout << " GREEN:";
                break;
            case 10:
                changeTextColor({ 255, 255, 255 }, { 196, 196, 196 });
                GotoXY(86 + 3, 24); cout << " BLUE: ";
                break;
            }
            changeTextColor();
            prev_selection = colorSelection;
            switch (colorSelection)
            {
            case 1:
                changeTextColor({ 238, 114, 20 });
                GotoXY(56 + (60 - 23) / 2 - 3, 13);     cout << u8"➤";
                break;
            case 2:
                changeTextColor({ 238, 114, 20 });
                GotoXY(56 + (60 - 23) / 2 + 11, 13);    cout << u8"➤";
                break;
            case 3:
                changeTextColor({ 238, 114, 20 });
                GotoXY(56 + (60 - 23) / 2 - 3, 15);     cout << u8"➤";
                break;
            case 4:
                changeTextColor({ 238, 114, 20 });
                GotoXY(56 + (60 - 23) / 2 + 11, 15);    cout << u8"➤";
                break;
            case 5:
                changeTextColor({ 255, 255, 255 }, { 255, 0, 0 });
                GotoXY(56 + 3, 20); cout << "  RED: ";
                break;
            case 6:
                changeTextColor({ 255, 255, 255 }, { 0, 255, 0 });
                GotoXY(56 + 3, 22); cout << " GREEN:";
                break;
            case 7:
                changeTextColor({ 255, 255, 255 }, { 0, 0, 255 });
                GotoXY(56 + 3, 24); cout << " BLUE: ";
                break;
            case 8:
                changeTextColor({ 255, 255, 255 }, { 255, 0, 0 });
                GotoXY(86 + 3, 20); cout << "  RED: ";
                break;
            case 9:
                changeTextColor({ 255, 255, 255 }, { 0, 255, 0 });
                GotoXY(86 + 3, 22); cout << " GREEN:";
                break;
            case 10:
                changeTextColor({ 255, 255, 255 }, { 0, 0, 255 });
                GotoXY(86 + 3, 24); cout << " BLUE: ";
                break;
            }
            changeTextColor();
            check = false;
        }
        // navigate through the menu
        if (_kbhit()) { // if any key is pressed
            // variable to receive user input
            char handle;
            handle = toupper(_getch());
            Sleep(50);
            check = true;

            switch (handle)
            {
            case 'S':
                if (checkMusicEffect) PlaySound(TEXT("sound/move.wav"), NULL, SND_FILENAME | SND_ASYNC);
                ++colorSelection;
                if (colorSelection > 10) colorSelection = 1;
                break;
            case 'W': // if user pressed W or 'Arrow Up'
                if (checkMusicEffect) PlaySound(TEXT("sound/move.wav"), NULL, SND_FILENAME | SND_ASYNC);
                check = true;
                --colorSelection;
                if (colorSelection < 1) colorSelection = 10;
                break;

            case 13: // if user pressed 'Enter'
                if (checkMusicEffect) PlaySound(TEXT("sound/click.wav"), NULL, SND_FILENAME | SND_ASYNC);
                switch (colorSelection)
                {
                case 1:
                    _userChoiceFG = DEFAULT1_FG;
                    _userChoiceBG = BG_RGB_2;
                    break;
                case 2:
                    _userChoiceFG = DEFAULT2_FG;
                    _userChoiceBG = DEFAULT2_BG;
                    break;
                case 3:
                    _userChoiceFG = DEFAULT3_FG;
                    _userChoiceBG = DEFAULT3_BG;
                    break;
                case 4:
                    _userChoiceFG = DEFAULT4_FG;
                    _userChoiceBG = DEFAULT4_BG;
                    break;
                case 5:
                    _manualfg_red = inputUnsignedNumber(67, 20);
                    isManualColor = true;
                    break;
                case 6:
                    _manualfg_green = inputUnsignedNumber(67, 22);
                    isManualColor = true;
                    break;
                case 7:
                    _manualfg_blue = inputUnsignedNumber(67, 24);
                    isManualColor = true;
                    break;
                case 8:
                    _manualbg_red = inputUnsignedNumber(97, 20);
                    isManualColor = true;
                    break;
                case 9:
                    _manualbg_green = inputUnsignedNumber(97, 22);
                    isManualColor = true;
                    break;
                case 10:
                    _manualbg_blue = inputUnsignedNumber(97, 24);
                    isManualColor = true;
                    break;
                default:
                    break;
                }
                if (isManualColor) {
                    _userChoiceFG = { _manualfg_red, _manualfg_green, _manualfg_blue };
                    _userChoiceBG = { _manualbg_red, _manualbg_green, _manualbg_blue };
                    isManualColor = false;
                }
                drawDemoSnake(56 + (60 - 9) / 2, 9, _userChoiceFG, _userChoiceBG);
                break;
            case 27: // <ESC>
                isEnter = true;
                colorSelection = -1;
                break;
            }
        }
    }
    GotoXY(56 + (60 - 22) / 2, 28);
    cout << "                      ";
}

unsigned int inputUnsignedNumber(int pos_x, int pos_y)
{
    GotoXY(pos_x, pos_y); cout << "                 ";
    GotoXY(pos_x, pos_y);
    unsigned int res;
    while (true) {
        ShowConsoleCursor(true);
        changeTextColor(TXT_RGB);
        cin >> res;
        changeTextColor();

        if (!cin.good() || res > 255) {
            ShowConsoleCursor(false);
            GotoXY(pos_x, pos_y);   cout << "                 ";
            changeTextColor({ 255, 0, 0 });
            GotoXY(pos_x, pos_y); cerr << "Please re-enter!";
            changeTextColor();
            Sleep(2000);
            GotoXY(pos_x, pos_y); cout << "                 ";

            cin.clear();
            cin.ignore(STREAM_SIZE, '\n');

            GotoXY(pos_x, pos_y);
        }
        else break;
    }
    ShowConsoleCursor(false);
    return res;
}


