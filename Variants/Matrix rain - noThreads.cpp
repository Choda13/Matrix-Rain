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

using namespace std;

#define BEGIN 12449
#define END 12450
#define TAIL_LENGTH 10
#define DELAY 25

mutex mtx;
int finish = 0;
int tail = TAIL_LENGTH;
vector <bool> zauzete;

SMALL_RECT GetConsoleSize();
char RandomChar();
void matrix(int brK, int brV);
void matrixV2(int brK, int brV);
void ChangeFgBg(int fg, int bg);
void gotoxy(int x, int y);
void SetUpConsole();
void ChangeConsoleSize(SHORT width, SHORT height);
int GetColumn(int brK);

int main()
{
	int brK, brV;

	srand((unsigned int)time(0));
	SetUpConsole();
	ChangeFgBg(1, 0);

	brK = 150;
	brV = 50;
	
	ChangeConsoleSize(brK, brV);
	matrixV2(brK, brV);

	return 1;
}

SMALL_RECT GetConsoleSize()
{
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(info);
	GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	return info.srWindow;
}

char RandomChar()
{
	char c;

	if (rand() % 2 == 0)
		c = rand() % (26) + 65;
	else
		c = rand() % (26) + 97;
	return c;
}

void ChangeFgBg(int fg, int bg)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (fg + (bg * 16)));
}

void gotoxy(int x, int y)
{
	COORD cord;
	cord.X = x;
	cord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord);
}

void SetUpConsole()
{
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
}

void ChangeConsoleSize(SHORT width, SHORT height)
{
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

void matrix(int brK, int brV)
{
	while (1)
	{
		for (int i = 0; i < brV; i++)
		{
			mtx.lock();
			gotoxy(brK, i);
			cout << RandomChar();

			if (i >= tail) {
				gotoxy(brK, i - tail);
				cout << L" ";
			}
			mtx.unlock();
			Sleep(DELAY);
		}

		for (int i = 0; i < tail; i++)
		{
			mtx.lock();
			gotoxy(brK, i + brV - tail);
			cout << L" ";
			mtx.unlock();
			Sleep(DELAY);
		}
	}
}

void matrixV2(int brK, int brV)
{
	int j=0;
	vector<int> kolone;
	
	for (int i = 0; i < brK; i++)
		kolone.push_back(rand() % brV-10);
	while (1)
	{
		Sleep(25);
		for (int k = 0; k < brK; k++)
		{
			gotoxy(k, kolone[k]%brV);
			cout << RandomChar();
			if (kolone[k] % brV < tail)
			{
				gotoxy(k, brV - tail + kolone[k] % brV);
				cout << " ";
			}
			else
			{
				gotoxy(k, kolone[k] % brV - tail);
				cout << " ";
			}
			kolone[k]++;
		}
	}
}

int GetColumn(int brK)
{
	int i;

	i = rand() % brK;
	if (zauzete[i])
	{
		zauzete[i] = false;
		return i;
	}

	return -1;
}

