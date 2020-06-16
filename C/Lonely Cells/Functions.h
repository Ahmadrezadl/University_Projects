#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#define endl printf("\n");
#define m(X) n-X-1
#define red printf("\033[0;31m");
#define green printf("\033[0;32m");
#define reset printf("\033[0m");

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

enum key{up=8,down,left,right,esc,enter};
enum option{load,singleplay,multiplay,quit};

char *randstring(){
	char *str = (char*)malloc((5+1) * sizeof(char));
	const char charset[] = "qwertyuiopasdfghjklzxcvbnm123456789";
	for(int i = 0;i < 10;i++)
	{
		int key = rand() % (sizeof(charset) / sizeof(char) - 1);
		str[i] = charset[key];
	}
	str[10] = '\0';
	return str;
}

int getKey(){
	int ch1, ch2;
	ch1 = getch();
	if(ch1 >= 49 && ch1 <= 55)
		return ch1-48;
	if(ch1 == 13)return enter;
	if(ch1 == 27)return esc;
	ch2 = 0;
	if (ch1 == 224) {
		ch2 = getch();
		switch(ch2)
		{
			case 72: return up;
			case 80: return down;
			case 75: return left;
			case 77: return right;
			default:
			return getKey();
		};
	}
	else
	{
		return getKey();
	}
}

void gotoxy(int eex, int eey)
{
	if(eex%2 == 0)
	{
		eex *= 8;
		eex += 3;
		eey *= 6;
		eey += 3;
	}
	else
	{
		eex *= 8;
		eex += 3;
		eey *= 6;
		eey += 4;
		eey += 3;
	}
	
	COORD coord;
	coord.X = eex;
	coord.Y = eey;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}



void gotoinfo(int n,int x)
{
	n-=1;
	n *= 9;
	n+= 9;
	x += 4;
	COORD coord;
	coord.X = n;
	coord.Y = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

#endif
