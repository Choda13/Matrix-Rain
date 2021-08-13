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

#define TAIL_LENGTH 10

mutex mtx, mtx2;
int finish = 0;
int tail = TAIL_LENGTH;
int DELAY = 25;
vector <bool> zauzete;

SMALL_RECT GetConsoleSize();
char RandomChar();
void matrix(int brK, int brV);
void ChangeFgBg(int fg, int bg);
void gotoxy(int x, int y);
void SetUpConsole();
void ChangeConsoleSize(SHORT width, SHORT height);
int DetermineColor(int j);
int GetColumn(int brK);

int main()
{
	int brK, brV;
	vector<thread> tredovi;
	srand((unsigned int)time(0));
	SetUpConsole();
	ChangeFgBg(1, 0);

	brK = 25;
	brV = 50;

	ChangeConsoleSize(brK, brV);

	tredovi.resize(brK);
	zauzete.resize(brK);

	for (int i = 0; i < brK; i++)
		zauzete[i] = true;

	while (finish != brK)
	{
		int i;
		do
			i = GetColumn(brK);
		while (i == -1);
		finish++;
		tredovi[i] = thread(matrix, i, brV);
		tredovi[i].detach();
		Sleep(100);
	}

	while (1)Sleep(1);
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
	info.ColorTable[1] = RGB(255, 255, 255);
	info.ColorTable[2] = RGB(50, 205, 50);
	info.ColorTable[3] = RGB(127, 255, 0);
	info.ColorTable[4] = RGB(0, 128, 0);
	info.ColorTable[5] = RGB(0, 100, 0);
	info.ColorTable[6] = RGB(0, 100, 0);
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
			if(i < tail)
				for (int j = 0; j <= i; j++)
				{
					mtx.lock();
					gotoxy(brK, i-j);
					ChangeFgBg(DetermineColor(j), 0);
					cout << RandomChar();
					mtx.unlock();
				}
			else {
				for (int j = 0; j < tail; j++)
				{
					mtx.lock();
					gotoxy(brK, i - j);
					ChangeFgBg(DetermineColor(j), 0);
					cout << RandomChar();
					mtx.unlock();
				}
			}
			if (i >= tail) {
				mtx.lock();
				gotoxy(brK, i - tail);
				cout << " ";
				mtx.unlock();
			}
			Sleep(DELAY);
		}

		for (int i = 0; i < tail; i++)
		{
			mtx.lock();
			gotoxy(brK, brV - tail + i);
			cout << " ";
			mtx.unlock();
			for (int j = 0; j < tail - i-1; j++)
			{
				mtx.lock();
				gotoxy(brK,brV-j-1);
				ChangeFgBg(DetermineColor(j+1+i),0);
				cout << RandomChar();
				mtx.unlock();
			}
			Sleep(DELAY);
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

int DetermineColor(int j)
{
	if (!j)
		return 1;
	if (j == 1 || j == 2)
		return 2;
	if (j == 3 || j == 4)
		return 3;
	if (j == 5 || j == 6)
		return 4;
	if (j == 7 || j == 8)
		return 5;
	return 6;
}