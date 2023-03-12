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
	if (INVALID_HANDLE_VALUE != hConsole)
	{
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(hConsole, pos);
	}
}


class Time
{
	int sec;
	int min;
	int hour;
public:
	Time()
	{
		sec = 0;
		min = 0;
		hour = 0;
	}

	Time &operator++()
	{
		sec++;
		if (sec > 59)
		{
			min++;
			if (min > 59)
			{
				hour++;
				min = 0;
			}
			sec = 0;
		}
		return *this;
	}

	void PrintTime()
	{
		cout << hour << ":" << min << ":" << sec;
	}
};

class Table
{
	int w;//ستون
	int len;//سطر
	int CharTable;
	int state[20][20];//ارایه حالت که بود و نبود شکل ها را بررسی میکند
public:
	Table(int m,int h)
	{
		w = m;
		len = h;
		CharTable = 219;
		/*state = new int*[w];
		for (int i = 0; i < w; i++)
			state[i] = new int[h];
		state = { 0 };*/
		for (int i = 0; i < len; i++)
			for (int j = 0; j < w; j++)
				state[i][j] = 0;//همه ی ارایه را صفر میکند
		for (int i = 0; i < len; i++)//سطر اول جدول را یک میکند
			state[0][i] = 1;
		for (int i = 0; i < len; i++)//سطر اخر
			state[w - 1][i] = 1;
		for (int i = 0; i < w; i++)//ستون چپ
			state[i][0] = 1;
		//for (int i = 0; i < w; i++)//ستون راست
			//state[i][len - 1] = 1;
	}

	//ارایه حالت را اگ شکل داشت پاک میشد صفر میکنه اگ داشت چاپ میشد یک میکنه
	void setState0(int x, int y) { state[y][x] = 0; }
	void setState1(int x, int y) { state[y][x] = 1; }

	void Bax()
	{
		SetConsoleTextAttribute(hConsole, 5);
		for (int i = 0; i < len; i++)//x
			cout << (char)CharTable;
		cout << endl;
		for (int i = 2; i < w; i++)//y
		{
			cout << (char)CharTable;
			for (int j = 2; j < len; j++)//x
				cout << " ";
			cout << (char)CharTable << endl;
		}
		for (int i = 0; i < len; i++)//y
			cout << (char)CharTable;
	}

	bool IsInTable(int y, int x)
	{
		if (y > 3 && y < len - 3 || x < w - 3)//چک میکنه تو جدول هست یا ن
		{
			if (state[x][y + 2] == 1)// اینام چک میکنن ک دور اون شکل شکل دیگ ای هست یا ن
				return false;
			//if (state[x - 2][y + 1] == 1)
				//return false;
			//if (state[x + 1][y + 1] == 1)
				//return false;
			if (state[x + 2][y + 1] == 1)
				return false;
			//if (state[x + 2][y + 2] == 1)
				//return false;

			if (state[x][y + 2] == 0)
				return true;
			if (state[x - 2][y + 1] == 0)
				return true;
			if (state[x + 1][y + 1] == 0)
				return true;			
			if (state[x + 2][y + 1] == 0)
				return true;
			//if (state[x + 2][y + 2] == 0)
				//return true;

			/*
			if (state[x][y + 2] == 1)// اینام چک میکنن ک دور اون شکل شکل دیگ ای هست یا ن
				return false;
			else if (state[x][y + 2] == 0)
				return true;
			
			//if (state[x -1][y + 2] == 1)
				//return false;
			//else if (state[x - 1][y + 2] == 0)
				//return true;
			
			//if (state[x + 1][y + 1] == 1)
				//return false;
			//else if (state[x + 1][y + 1] == 0)
				//return true;
			
			//if (state[x + 2][y + 1] == 1)
				//return false;
			//else if (state[x + 2][y + 1] == 0)
				//return true;
					
			//if (state[x + 2][y + 2] == 1)
				//return false;
			//else if (state[x + 2][y + 2] == 0)
				//return true;
			*/
		}
		return false;
	}

	bool Shift()//تابعی که چک میکنه اگر یک خط پر شد اونو پاک کنه
	{
		bool flag = false;
		int count = 0;
		for (int i = 1; i < w - 1; i++)
		{
			for (int j = 0; j < len; j++)
				if (state[i][j] == 1)
					count++;
			if (count == len - 1)//اگر تعداد یک ها با سطرهاش یا طولش برابر بود حالا بیاد شیفتش بده
			{
				flag = true;
				while (i > 1)
				{
					for (int k = 0; k < len; k++)
						state[i][k] = state[i - 1][k];//تمام مقادیر خط بالاییشو ببره پایین روی اون یک ها
					i--;
				}
			}
			count = 0;
		}
		return flag;
	}

	void DeleteAfterShift()
	{
		for (int i = 0; i < w; i++)
			for (int j = 0; j < len; j++)
				if (state[i][j] == 1)
				{
					SetConsoleTextAttribute(hConsole, 5);
					gotoxy(j, i);
					cout << (char)CharTable;
				}
				else
				{
					gotoxy(j, i);
					cout << ' ';
				}
		for (int i = 0; i < w; i++)//ستون راست
		{
			gotoxy(len - 1,i);
			cout << (char)CharTable;
		}
	}

	bool EndGame()
	{
		for (int i = 1; i < w-1; i++)
			if (state[1][i] == 1)//چک میکنه ک اگر سط دوم توی ارایه حتی اگر یکدونش یک شده ینی دیگ جایی نداره ک شکل بعدی بیاد پس از بازی میاد بیرون
				return true;
		return false;
	}
};

class Shape
{
protected:
	int X;//مختصات شکل
	int Y;
	int state[3][3];
	int CharShape;
	Table *table;

public:
	Shape(int x, int y, Table *t)//:table(t)
	{
		X = x;
		Y = y;
		CharShape = 178;
		table = t;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				state[i][j] = 0;
	}

	int getX() { return X; }
	int getY() { return Y; }

	void Move(char ch)
	{
		if (table->IsInTable(X, Y))
		{
			if (GetAsyncKeyState(VK_LEFT))X--;
			if (GetAsyncKeyState(VK_RIGHT))X++;
			if (GetAsyncKeyState(VK_DOWN))Y++;
		}
		/*if (table->IsInTable(X, Y))
			switch (ch)
			{
				case 'a':X--;
					break;
				case 'd':X++;
					break;
				case 's':Y++;
					break;
			}*/
	}

	Shape &operator++(int)
	{
		if (table->IsInTable(X, Y))
			Y++;
		return *this;
	}

	virtual void Print() = 0;//تابع چاپ
	virtual void UnPrint() = 0;//تابع حذف از صفحه
	virtual void Rotatoin(char ch) { }//چرخش

};

class S :public Shape
{
public:
	S(int x, int y, Table *t) :Shape(x, y, t) 
	{
		state[0][1] = state[1][1] = state[0][2] = state[1][2] = 1;
	}
	
	void Print()
	{
		SetConsoleTextAttribute(hConsole, 4);
		gotoxy(X, Y); cout << (char)CharShape; table->setState1(X, Y);
		gotoxy(X, Y + 1); cout << (char)CharShape; table->setState1(X, Y + 1);
		gotoxy(X + 1, Y); cout << (char)CharShape; table->setState1(X + 1, Y);
		gotoxy(X + 1, Y + 1); cout << (char)CharShape; table->setState1(X + 1, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{
			gotoxy(X, Y + i);
			for (int j = 0; j < 3; j++)
				if (state[i][j] == 1)
				{
					cout << (char)CharShape;
					table->setState1(X , Y + j);
				}
				
		}*/
	}

	void UnPrint()
	{
		gotoxy(X, Y); cout << " "; table->setState0(X, Y);
		gotoxy(X, Y + 1); cout << " "; table->setState0(X, Y + 1);
		gotoxy(X + 1, Y); cout << " "; table->setState0(X + 1, Y);
		gotoxy(X + 1, Y + 1); cout << " "; table->setState0(X + 1, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{
			gotoxy(X, Y + i);
			for (int j = 0; j < 3; j++)
				if (state[i][j] == 1)
				{
					cout << " ";
					table->setState0(X , Y + j);
				}
		}*/
	}
};

class L
	:public Shape
{
public:
	L(int x, int y, Table *t) :Shape(x, y, t) 
	{
		state[0][1] = state[1][1] = state[1][2] = 1;
	}

	void Print()
	{
		SetConsoleTextAttribute(hConsole, 1);
		gotoxy(X, Y); cout << (char)CharShape; table->setState1(X, Y);
		gotoxy(X, Y + 1); cout << (char)CharShape; table->setState1(X, Y + 1);
		gotoxy(X + 1, Y + 1); cout << (char)CharShape; table->setState1(X + 1, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{

			for (int j = 0; j < 3; j++)
			{
				gotoxy(X - 1 + i, Y + j);
				if (state[i][j] == 1)
				{
					cout << (char)CharShape;
					table->setState1(X - 1 + i, Y + j);
				}
			}
		}*/
	}

	void UnPrint()
	{
		gotoxy(X, Y); cout << " "; table->setState0(X, Y);
		gotoxy(X, Y + 1); cout << " "; table->setState0(X, Y + 1);
		gotoxy(X + 1, Y + 1); cout << " "; table->setState0(X + 1, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{

			for (int j = 0; j < 3; j++)
			{
				gotoxy(X - 1 + i, Y + i);
				if (state[i][j] == 1)
				{
					cout << " ";
					table->setState0(X - 1 + i, Y + j);
				}
			}
		}*/
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case '4': state[1][2] = 0; state[1][0] = 1;
			break;
		case '6': state[0][1] = 0; state[2][2] = 1;
			break;
		}
	}
};

class Z :public Shape
{
public:
	Z(int x, int y,Table *t) :Shape(x, y,t)
	{
		state[0][0] = state[0][1] = state[1][1] = state[1][2] = 1;
	}

	void Print()
	{
		SetConsoleTextAttribute(hConsole, 2);
		gotoxy(X, Y); cout << (char)CharShape; table->setState1(X, Y);
		gotoxy(X - 1, Y); cout << (char)CharShape; table->setState1(X - 1, Y);
		gotoxy(X, Y + 1); cout << (char)CharShape; table->setState1(X, Y + 1);
		gotoxy(X + 1, Y + 1); cout << (char)CharShape; table->setState1(X + 1, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{
		gotoxy(X, Y + i);
		for (int j = 0; j < 3; j++)
		if (state[i][j] == 1)
		{
		cout << (char)CharShape;
		table->setState1(X + i, Y + j);
		}
		}*/
	}

	void UnPrint()
	{
		gotoxy(X, Y); cout << " "; table->setState0(X, Y);
		gotoxy(X - 1, Y); cout << " "; table->setState0(X - 1, Y);
		gotoxy(X, Y + 1); cout << " "; table->setState0(X, Y + 1);
		gotoxy(X + 1, Y + 1); cout << " "; table->setState0(X + 1, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{
		gotoxy(X, Y + i);
		for (int j = 0; j < 3; j++)
		if (state[i][j] == 1)
		{
		cout << " ";
		table->setState0(X + i, Y + j);
		}
		}*/
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case '4':state[0][0] = state[1][2] = 0;
			state[1][0] = state[2][0] = 1;
			break;
		case '6':state[0][0] = 0;
			state[1][2] = state[2][2] = 1;
			break;
		}
	}
	
};

class U :public Shape
{
public:
	U(int x, int y, Table *t) :Shape(x, y, t) 
	{
		state[0][0] = state[1][0] = state[1][1] = state[0][2] = state[1][2] = 1;
	}

	void Print()
	{
		SetConsoleTextAttribute(hConsole, 3);
		//gotoxy(X, Y); cout << (char)CharShape; table->setState1(X, Y);
		//gotoxy(X, Y + 1); cout << (char)CharShape; table->setState1(X, Y + 1);
		gotoxy(X, Y+1); cout << (char)CharShape; table->setState1(X, Y+1);
		gotoxy(X-1, Y); cout << (char)CharShape; table->setState1(X-1, Y);
		gotoxy(X+1, Y); cout << (char)CharShape; table->setState1(X+1, Y);
		gotoxy(X - 1, Y + 1); cout << (char)CharShape; table->setState1(X-1, Y+1);
		gotoxy(X+1, Y+1); cout << (char)CharShape; table->setState1(X+1, Y+1);
		/*for (int i = 0; i < 3; i++)
		{
		gotoxy(X, Y + i);
		for (int j = 0; j < 3; j++)
		if (state[i][j] == 1)
		{
		cout << (char)CharShape;
		table->setState1(X + i, Y + j);
		}
		}*/
	}

	void UnPrint()
	{
		//gotoxy(X, Y); cout << " "; table->setState0(X, Y);
		//gotoxy(X, Y + 1); cout << " "; table->setState0(X, Y + 1);
		gotoxy(X, Y+1); cout << " "; table->setState0(X, Y+1);
		gotoxy(X-1, Y); cout << " "; table->setState0(X-1, Y);
		gotoxy(X+1, Y); cout << " "; table->setState0(X+1, Y);
		gotoxy(X-1, Y+1); cout << " "; table->setState0(X-1, Y+1);
		gotoxy(X+1, Y+1); cout << " "; table->setState0(X+1, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{
		gotoxy(X, Y + i);
		for (int j = 0; j < 3; j++)
		if (state[i][j] == 1)
		{
		cout << " ";
		table->setState0(X + i, Y + j);
		}
		}*/
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case '4':state[0][0] = state[1][0] = state[1][2] = 0;
			state[0][1] = state[2][1] = state[2][2] = 1;
			break;
		case '6':state[1][0] = state[0][2] = state[1][2] = 0;
			state[0][1] = state[2][0] = state[2][1] = 1;
			break;
		}
	}
};

class T :public Shape
{
public:
	T(int x, int y, Table *t) :Shape(x, y, t) 
	{
		state[0][1] = state[1][1] = state[1][2] = state[1][0] = 1;
	}

	void Print()
	{
		SetConsoleTextAttribute(hConsole, 6);
		gotoxy(X - 1, Y + 1); cout << (char)CharShape; table->setState1(X - 1, Y + 1);
		gotoxy(X, Y); cout << (char)CharShape; table->setState1(X, Y);
		gotoxy(X + 1, Y + 1); cout << (char)CharShape; table->setState1(X + 1, Y + 1);
		gotoxy(X, Y + 1); cout << (char)CharShape; table->setState1(X, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{
		gotoxy(X, Y + i);
		for (int j = 0; j < 3; j++)
		if (state[i][j] == 1)
		{
		cout << (char)CharShape;
		table->setState1(X + i, Y + j);
		}
		}*/
	}

	void UnPrint()
	{
		gotoxy(X - 1, Y + 1); cout << " "; table->setState0(X - 1, Y + 1);
		gotoxy(X, Y); cout << " "; table->setState0(X, Y);
		gotoxy(X + 1, Y + 1); cout << " "; table->setState0(X + 1, Y + 1);
		gotoxy(X, Y + 1); cout << " "; table->setState0(X, Y + 1);
		/*for (int i = 0; i < 3; i++)
		{
		gotoxy(X, Y + i);
		for (int j = 0; j < 3; j++)
		if (state[i][j] == 1)
		{
		cout << " ";
		table->setState0(X + i, Y + j);
		}
		}*/
	}

	void Rotation(char ch)
	{
		switch (ch)
		{
		case '4':state[1][2] = 0;
			state[2][1] = 1;
			break;
		case '6':state[1][0] = 0;
			state[2][1] = 1;
			break;
		}
	}
};


void _tmain()
{
	cout << "Moves Shapes:" << endl << endl << " " << (char)27 << "\t " << (char)26 << endl << "     " << (char)25 << endl << endl << endl << "START...";
	while (true)//این حلقه زمانی بیرون میره ک کاربر حتما اسپیس بزنه تا بازی شروع بشه
		if (_getch() == (char)32)
			break;
	system("cls");
	Time time;
	Table table(20, 20);
	Shape *shape = new L(9, 1, &table);
	table.Bax();
	int score = 0;//امتیاز
	int timer = 0;//تنظیم کننده ی سرعت تایمر است
	while (true)
	{
		if (_kbhit())//زمانی ک کاربرد کاراکتری وارد کرد این شط انجاام میشه
		{
			char ch = _getch();
			shape->UnPrint();
			shape->Move(ch); // و این کاراکتر به تابع حرکت ارسال میشه 
			//shape->Rotatoin(ch);
			shape->Print();
			if (ch == (char)32)//اگر اسپیس زد بازی استپ بشه تا زمانی ک یدونه کاراکتر بزنه
				_getch();
		}
		shape->UnPrint();
		(*shape)++;
		shape->Print();
		if (!table.IsInTable(shape->getX(), shape->getY()))//اگر شکل به جایی رسید ک نمیتوانست حرکت کند و ثابت ماند شکل جدیدی وارد بازی میکنیم
		{
			int random = rand() % 5;//تابع رندم: یک عدد از بین 0تا 4 انتخاب میکند آن عددرا در نامبر ذخیره میکنیم و متناسب با ان شکل جدید را به شیپ اختصاص میدهیم
			switch (random)
			{
				case 0: shape = new S(9, 1, &table); break;
				case 1: shape = new U(9, 1, &table); break;
				case 2: shape = new T(9, 1, &table); break;
				case 3: shape = new L(9, 1, &table); break;
				case 4: shape = new Z(9, 1, &table); break;
			}
		}
		if (timer % 4 == 0)//این شرط تامیر را تنظیم میکند
		{
			SetConsoleTextAttribute(hConsole, 14);
			gotoxy(7, 24);
			time.PrintTime();
			++time;
		}
		if (table.Shift())// اگر یک خط پرشد آن خط را پاک میکند
		{
			table.DeleteAfterShift();
			score++;
		}
		if (table.EndGame())// اگر ب انتهای بازی رسیدیم یعنی حتی اگر یدونه یک در سطر اول جدول بود از بازی خارج میشود
		{
			SetConsoleTextAttribute(hConsole, 12); gotoxy(5, 10); cout << "Game Over!";
			_getch(); _getch();
			exit(0);
		}	
		gotoxy(6, 22); cout << "Score: " << score;
		Sleep(250); timer++;
	}
}