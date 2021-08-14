#include <Windows.h>
#include <iostream>
#include <stdlib.h>
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


#define TAIL_LENGTH 20
#define DELAY 1
int tail = TAIL_LENGTH;

using namespace std;

bool GetConsoleSize(COORD& cord);
void ChangeConsoleSize(SHORT width, SHORT height);
void ChangeFgBg(int fg, int bg);
void gotoxy(int x, int y);
void SetUpConsole();

char RandomChar();
void matrix(int brK, int brV);

int main(){
	srand((unsigned int)time(0));

	int brK, brV;
	COORD cSize;
	SetUpConsole();

	if (!GetConsoleSize(cSize))
		return 1;
	brK = cSize.X;
	brV = cSize.Y;

	matrix(brK, brV);

	return 0;
}

bool GetConsoleSize(COORD& cord)
{
	CONSOLE_SCREEN_BUFFER_INFO p;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GetConsoleScreenBufferInfo(console, &p)) {
		cord = p.dwSize;
		return true;
	}
	return false;
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
	vector<int> vrste;

	for (int i = 0; i < brK; i++)
		vrste.push_back(rand() % brV - 10);

	while (1){
		Sleep(DELAY);
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