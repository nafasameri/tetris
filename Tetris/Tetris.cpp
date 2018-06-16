// Tetris.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "iostream"
#include "windows.h"
#include "synchapi.h"
using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


void gotoxy(int x, int y)
{
	COORD pos;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (INVALID_HANDLE_VALUE != hConsole)
	{
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(hConsole, pos);
	}
}


class Table
{
	int w;
	int h;
	int **state;
public:
	Table(int n,int m)
	{
		w = n;
		h = m;
		state = new int*[w];
		for (int i = 0; i < w; i++)
			state[i] = new int[h];
		state = { 0 };
		for (int i = 0; i < w; i++)
			state[0][i] = 1;
		for (int i = 0; i < w; i++)
			state[h - 1][i] = 1;
		for (int i = 0; i < h; i++)
			state[i][0] = 1;
		for (int i = 0; i < h; i++)
			state[i][w - 1] = 1;
	}

	void Bax()
	{
		//SetConsoleTextAttribute(hConsole, 13);
		for (int i = 0; i < w; i++)
			cout << (char)219;
		cout << endl;
		for (int i = 1; i < h; i++)
		{
			cout << (char)219;
			for (int j = 0; j < w - 2; j++)
				cout << " ";
			cout << (char)219 << endl;
		}
		for (int i = 0; i < w; i++)
			cout << (char)219;
	}

	bool IsInTable(int x, int y)
	{
		/*if (x < 2 || x > w-1 || y > h-3)
			return false;
		else
			return true;*/
		if (state[x][y] == 0)
			return true;
		else if (state[x][y + 1] == 0)
			return true;
		else if (state[x + 1][y] == 0)
			return true;
		else if (state[x - 1][y] == 0)
			return true;
		else if (state[x + 1][y + 1] == 0)
			return true;
		else if (state[x - 1][y + 1] == 0)
			return true;
		else
			return false;
	}
};

class Shape
{
protected:
	int X;
	int Y;
	Table table;

public:
	Shape(int x, int y, Table t):table(t)
	{
		X = x;
		Y = y;
	}

	void Move(char ch)
	{
		if (table.IsInTable(X, Y))
			switch (ch)
			{
				case '4':X--;
					break;
				case '6':X++;
					break;
				case '2':Y++;
					break;
			}
	}

	Shape &operator++(int)
	{
		if (table.IsInTable(X, Y))
			Y++;
		return *this;
	}

	virtual void Print() = 0;
	virtual void UnPrint() = 0;
	virtual void Rotatoin(char ch) {}

};

class S :public Shape
{
public:
	S(int x, int y, Table t) :Shape(x, y, t) {}

	void Print()
	{
		gotoxy(X, Y); cout << (char)219;
		gotoxy(X, Y + 1); cout << (char)219;
		gotoxy(X + 1, Y); cout << (char)219;
		gotoxy(X + 1, Y + 1); cout << (char)219;
	}

	void UnPrint()
	{
		gotoxy(X, Y); cout << " ";
		gotoxy(X, Y + 1); cout << " ";
		gotoxy(X + 1, Y); cout << " ";
		gotoxy(X + 1, Y + 1); cout << " ";
	}


};

class L :public Shape
{
public:
	L(int x, int y, Table t) :Shape(x, y, t) {}

	void Print()
	{
		gotoxy(X, Y); cout << (char)178;
		gotoxy(X, Y + 1); cout << (char)178;
		gotoxy(X, Y + 2); cout << (char)178;
		gotoxy(X + 1, Y + 2); cout << (char)178;
	}

	void UnPrint()
	{
		gotoxy(X, Y); cout << " ";
		gotoxy(X, Y + 1); cout << " ";
		gotoxy(X, Y + 2); cout << " ";
		gotoxy(X + 1, Y + 2); cout << " ";
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case 'a':
			break;
		case 'd':
			break;
		}
	}
};

class Z :public Shape
{
public:
	Z(int x, int y,Table t) :Shape(x, y,t){}

	void Print()
	{
		gotoxy(X, Y); cout << (char)219;
		gotoxy(X + 1, Y); cout << (char)219;
		gotoxy(X + 1, Y + 1); cout << (char)219;
		gotoxy(X + 2, Y + 1); cout << (char)219;
	}

	void UnPrint()
	{
		gotoxy(X, Y); cout << " ";
		gotoxy(X + 1, Y); cout << " ";
		gotoxy(X + 1, Y + 1); cout << " ";
		gotoxy(X + 2, Y + 1); cout << " ";
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case 'a':
			break;
		case 'd':
			break;
		}
	}
	
};

class I :public Shape
{
public:
	I(int x, int y, Table t) :Shape(x, y, t) {}

	void Print()
	{
		gotoxy(X, Y); cout << (char)219;
		gotoxy(X, Y + 1); cout << (char)219;
		//gotoxy(X, Y + 2); cout << (char)219;
		//gotoxy(X, Y + 3); cout << (char)219;
	}

	void UnPrint()
	{
		gotoxy(X, Y); cout << " ";
		gotoxy(X, Y + 1); cout << " ";
		//gotoxy(X, Y + 2); cout << " ";
		//gotoxy(X, Y + 3); cout << " ";
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case 'a':
			break;
		case 'd':
			break;
		}
	}
};

class T :public Shape
{
public:
	T(int x, int y, Table t) :Shape(x, y, t) {}

	void Print()
	{
		gotoxy(X - 1, Y); cout << (char)219;
		gotoxy(X, Y); cout << (char)219;
		gotoxy(X + 1, Y); cout << (char)219;
		gotoxy(X, Y + 1); cout << (char)219;
	}

	void UnPrint()
	{
		gotoxy(X - 1, Y); cout << " ";
		gotoxy(X, Y); cout << " ";
		gotoxy(X + 1, Y); cout << " ";
		gotoxy(X, Y + 1); cout << " ";
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case 'a':
			break;
		case 'd':
			break;
		}
	}
};



void main()
{
	Table table(35, 30);
	table.Bax();
	char ch;
	Shape *shape;
	

	int number = rand() % 5;
	switch (number)
	{
		case 0:
		{
			S s(15, 2, table);
			shape = &s;
			break;
		}
		case 1:
		{
			L l(15, 2, table);
			shape = &l;
			break;
		}
		case 2:
		{
			Z z(15, 2, table);
			shape = &z;
			break;
		}
		case 3:
		{
			T t(15, 2, table);
			shape = &t;
			break;
		}
		case 4:
		{	I i(15, 2, table);
		shape = &i;
		break;
		}
	}
	while (true)
	{
		if (_kbhit()) 
		{
			shape->UnPrint();
			ch = _getch();
			shape->Move(ch);
		}
		shape->UnPrint();
		shape++;
		shape->Print();
		Sleep(500);
	}
}