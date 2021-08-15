#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <conio.h>
#include <stdio.h>
#include <fcntl.h>
#include <conio.h>
#include <cstdlib>
#include <time.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <io.h>


#define TAIL_WITH_SCREEN_PROP 8
#define SMALL_VALUES_PROP 2
#define DELAY 1

//TODO: Fix cls function
int tail;
int delay = DELAY;
CONSOLE_SCREEN_BUFFER_INFOEX oldInfo;
CONSOLE_FONT_INFOEX oldCFI;
CONSOLE_CURSOR_INFO oldCCI;

using namespace std;

void ParseArgs(int argc, char** argv);

bool GetConsoleSize(COORD& cord);
void ChangeConsoleSize(SHORT width, SHORT height);
void ChangeFgBg(int fg, int bg);
void gotoxy(int x, int y);
void SetUpConsole();
void RevertConsole();
void cls(HANDLE hConsole);

int GenerateTailLength(int rowNums);
char RandomChar();
void matrix(int brK, int brV);

int main(int argc, char** argv){
	srand((unsigned int)time(0));

	int brK, brV;
	COORD cSize;
	
	ParseArgs(argc, argv);
	SetUpConsole();
	atexit(RevertConsole);
	signal(SIGINT, exit);

	if (!GetConsoleSize(cSize))
		return 1;
	brK = cSize.X;
	brV = cSize.Y;
	tail = GenerateTailLength(brV);

	matrix(brK, brV);

	return 0;
}

void ParseArgs(int argc, char** argv) {
	if (argc < 2)
		delay = DELAY;
	else if (!atoi(argv[1]))
		delay = DELAY;
	else
		delay = atoi(argv[1]);
}

void SetUpConsole(){
	_setmode(_fileno(stdout), _O_TEXT);

	CONSOLE_SCREEN_BUFFER_INFOEX info;
	CONSOLE_FONT_INFOEX cfi;
	CONSOLE_CURSOR_INFO cci;
	HANDLE OUT_HANDLE;

	OUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

	info.cbSize = sizeof(info);
	cfi.cbSize = sizeof(cfi);

	GetConsoleScreenBufferInfoEx(OUT_HANDLE, &info);
	GetCurrentConsoleFontEx(OUT_HANDLE, true, &cfi);
	oldInfo = info;
	oldCFI = cfi;

	info.ColorTable[0] = RGB(0, 0, 0);
	info.ColorTable[1] = RGB(127, 255, 0);
	info.ColorTable[2] = RGB(255, 255, 255);
	info.ColorTable[3] = RGB(114, 71, 56);
	info.ColorTable[4] = RGB(50, 31, 26);
	info.ColorTable[5] = RGB(132, 94, 73);
	info.ColorTable[6] = RGB(73, 47, 37);
	info.ColorTable[7] = RGB(22, 11, 4);
	info.ColorTable[8] = RGB(158, 158, 158);
	info.ColorTable[9] = RGB(102, 102, 102);
	info.ColorTable[10] = RGB(43, 43, 43);
	info.ColorTable[11] = RGB(0, 119, 65);
	info.ColorTable[12] = RGB(126, 33, 35);
	info.ColorTable[13] = RGB(10, 10, 130);
	info.ColorTable[14] = RGB(240, 191, 40);
	info.ColorTable[15] = RGB(255, 255, 255);
	GetCurrentConsoleFontEx(OUT_HANDLE, false, &oldCFI);
	wcscpy_s(cfi.FaceName, L"MS Gothic");
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 18;

	cci.dwSize = 100;
	cci.bVisible = FALSE;

	SetConsoleScreenBufferInfoEx(OUT_HANDLE, &info);
	SetCurrentConsoleFontEx(OUT_HANDLE, true, &cfi);
	SetConsoleCursorInfo(OUT_HANDLE, &cci);
	ChangeFgBg(1, 0);
}
void RevertConsole() {
	HANDLE OUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferInfoEx(OUT_HANDLE, &oldInfo);
	SetCurrentConsoleFontEx(OUT_HANDLE, true, &oldCFI);
	SetConsoleCursorInfo(OUT_HANDLE, &oldCCI);
	cls(OUT_HANDLE);
}
bool GetConsoleSize(COORD& cord)
{
	CONSOLE_SCREEN_BUFFER_INFO p;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleScreenBufferInfo(console, &p)) {
		cord.X = p.srWindow.Right;
		cord.Y = p.srWindow.Bottom;
		return true;
	}
	return false;
}
void gotoxy(int x, int y){
	COORD cord;
	cord.X = x;
	cord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
}
void ChangeConsoleSize(SHORT width, SHORT height){
	CONSOLE_SCREEN_BUFFER_INFOEX cinfo;
	HANDLE OUT_HANDLE;

	OUT_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

	cinfo.cbSize = sizeof(cinfo);
	GetConsoleScreenBufferInfoEx(OUT_HANDLE, &cinfo);
	cinfo.dwSize.X = width;
	cinfo.dwMaximumWindowSize.X = 1920;
	cinfo.dwMaximumWindowSize.Y = 1080;
	cinfo.srWindow.Bottom = height;
	SetConsoleScreenBufferInfoEx(OUT_HANDLE, &cinfo);
}
void ChangeFgBg(int fg, int bg) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (fg + (bg * 16)));
}
void cls(HANDLE hConsole)
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	SMALL_RECT scrollRect;
	COORD scrollTarget;
	CHAR_INFO fill;

	// Get the number of character cells in the current buffer.
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		return;
	}

	// Scroll the rectangle of the entire buffer.
	scrollRect.Left = 0;
	scrollRect.Top = 0;
	scrollRect.Right = csbi.dwSize.X;
	scrollRect.Bottom = csbi.dwSize.Y;

	// Scroll it upwards off the top of the buffer with a magnitude of the entire height.
	scrollTarget.X = 0;
	scrollTarget.Y = (SHORT)(0 - csbi.dwSize.Y);

	// Fill with empty spaces with the buffer's default text attribute.
	fill.Char.UnicodeChar = TEXT(' ');
	fill.Attributes = csbi.wAttributes;

	// Do the scroll
	ScrollConsoleScreenBuffer(hConsole, &scrollRect, NULL, scrollTarget, &fill);

	// Move the cursor to the top left corner too.
	csbi.dwCursorPosition.X = 0;
	csbi.dwCursorPosition.Y = 0;

	SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
}

int GenerateTailLength(int rowNums){
	if (rowNums > TAIL_WITH_SCREEN_PROP * 3)
		return rowNums / TAIL_WITH_SCREEN_PROP;
	else if (rowNums > SMALL_VALUES_PROP)
		return SMALL_VALUES_PROP;
	else
		return rowNums / 2;
}
char RandomChar(){
	char c;
	if (rand() % 2 == 0) 
		c = rand() % (26) + 65;
	else
		c = rand() % (26) + 97;
	return c;
}
void matrix(int brK, int brV){
	int j = 0;
	vector<int> vrste; //Predstavlja trenutnu vrednost vrste u datoj koloni, tj y poziciju u datoj x vrednosti

	for (int i = 0; i < brK; i++)
		vrste.push_back(rand() % brV);

	while (1){
		Sleep(delay);
		for (int k = 0; k < brK; k++){
			gotoxy(k, vrste[k] % brV);
			cout << RandomChar();
			if (vrste[k] % brV < tail){
				gotoxy(k, brV - tail + vrste[k] % brV);
				cout << " ";
			}
			else{
				gotoxy(k, vrste[k] % brV - tail);
				cout << " ";
			}
			vrste[k]++;
		}
	}
}
