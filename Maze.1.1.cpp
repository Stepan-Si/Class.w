﻿
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
const int WIDTH = 40; // ширина лабиринта
enum Color 
{
	DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15,
	DARKYELLOW = 6, DARKRED = 4, DARKGREY = 8, GREY = 7, AQUA = 11, DARKAQUA = 3
};
int steps = 0;
int health = 100; // количество очков здоровья главного героя
int coins = 0; // счётчик собранных монет
int box = 0;
COORD infobox;

// function definition (определение функции)
// настройка размеров шрифта
void SetConsoleFont()
{
	// настройки шрифта консоли
	CONSOLE_FONT_INFOEX font; // https://docs.microsoft.com/en-us/windows/console/console-font-infoex
	font.cbSize = sizeof(font);
	font.dwFontSize.Y = 70;
	font.FontFamily = FF_DONTCARE;
	font.FontWeight = FW_NORMAL;
	wcscpy_s(font.FaceName, 9, L"Consolas");
	SetCurrentConsoleFontEx(h, 0, &font);
}

// ещё одна кастомизированная функция, которая показывает количество монет в определённом месте консоли
void ShowCoins()
{
	infobox.X = WIDTH + 1;
	infobox.Y = 1;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKYELLOW);
	cout << "COINS: ";
	SetConsoleTextAttribute(h, Color::YELLOW);
	cout << coins << "\n"; // 0
}
void ShowSteps()
{
	infobox.X = WIDTH + 1;
	infobox.Y = 3;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKGREY);
	cout << "STEPS: ";
	SetConsoleTextAttribute(h, Color::GREY);
	cout << steps << "\n";
}
void ShowHealth()
{
	infobox.X = WIDTH + 1;
	infobox.Y = 2;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKRED);
	cout << "HEALTH: ";
	SetConsoleTextAttribute(h, Color::RED);
	cout << health << "  \n";
}
void ShowBox()        
{
	infobox.X = WIDTH + 1;
	infobox.Y = 4;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKAQUA);
	cout << "BOX: ";
	SetConsoleTextAttribute(h, Color::AQUA);
	cout << box << "\n"; 
}
int main()
{
	// enumeration (перечисление - это набор именованных целочисленных констант)
	// MazeObject - пользовательский (кастомизированный) тип данных
	enum MazeObject { HALL, WALL, COIN, ENEMY, BORDER, CROSS, BOX, };
	enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

	// скрытие мигающего курсора 
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = false; // спрятать курсор
	cursor.dwSize = 1; // 1...100
	SetConsoleCursorInfo(h, &cursor);

	system("title Maze");
	MoveWindow(GetConsoleWindow(), 20, 60, 1200, 700, true);
	// 20 - отступ слева от левой границы рабочего стола до левой границы окна консоли (в пикселях!)
	// 60 - отступ сверху от верхней границы рабочего стола до верхней границы окна консоли
	// 1200 - ширина окна консоли в пикселях
	// 700 - высота окна консоли
	// true - перерисовать окно после перемещения

	srand(time(0));

	const int HEIGHT = 10; // высота лабиринта

	int maze[HEIGHT][WIDTH] = {}; // maze - лабиринт по-английски

	// алгоритм заполнения массива
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			maze[y][x] = rand() % 4; // 4 типа объектов в игре

			if (maze[y][x] == MazeObject::ENEMY) // если в лабиринте сгенерился враг
			{
				int probability = rand() % 10; // 0...9, если выпало 0 - враг останется, останется только одна пятая часть врагов
				if (probability != 0) // убираем врага
				{
					maze[y][x] = MazeObject::HALL; // на место врага ставим коридор
				}
			}
			if (maze[y][x] == MazeObject::WALL) // если в лабиринте сгенерировалась стена
			{
				int probability = rand() % 2; // 0...1, если выпало 0 - стена останется, останется только половина стен
				if (probability != 0) // убираем стену
				{
					maze[y][x] = MazeObject::HALL; // на место стены ставим коридор
				}
			}

			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) maze[y][x] = MazeObject::BORDER; // белая рамка

			if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2) maze[y][x] = MazeObject::HALL; // вход

			if (x == WIDTH - 1 && y == HEIGHT - 3 ||
				x == WIDTH - 2 && y == HEIGHT - 3 ||
				x == WIDTH - 3 && y == HEIGHT - 3) maze[y][x] = MazeObject::HALL; // выход

		}
	}

	// показ лабиринта
	for (int y = 0; y < HEIGHT; y++) // перебор строк
	{
		for (int x = 0; x < WIDTH; x++) // перебор столбцов
		{
			switch (maze[y][x])
			{
			case MazeObject::HALL: // hall - коридор
				cout << " ";
				break;

			case MazeObject::WALL: // wall - стена
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				cout << (char)178;
				break;

			case MazeObject::BORDER: // border - рамка
				SetConsoleTextAttribute(h, Color::WHITE);
				cout << (char)178;
				break;

			case MazeObject::COIN: // coin - монетка
				SetConsoleTextAttribute(h, Color::YELLOW);
				cout << ".";
				break;

			case MazeObject::ENEMY: // enemy - враг
				SetConsoleTextAttribute(h, Color::RED);
				cout << (char)1;
				break;

			case MazeObject::BOX: 
				SetConsoleTextAttribute(h, AQUA);
				cout << (char)3;
				
			}
		}
		cout << "\n";
	}

	/////////////////////////////////////////////////////////////////////

	// размещение главного героя (ГГ)
	COORD position; // координаты нашего персонажа
	position.X = 0;
	position.Y = 2;
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << (char)2;

	/////////////////////////////////////////////////////////////////////
	// информация по всем показателям
	ShowCoins();
	ShowSteps();
	ShowHealth();
	ShowBox();

	while (true)
	{
		if (_kbhit()) // если было нажатие на клавиши пользователем
		{
			int code = _getch(); // get character, получение кода нажатой клавиши
			if (code == 224) { // если это стрелка
				code = _getch(); // получить конкретный код стрелки
			}

			// стирание персонажика в старой позиции
			SetConsoleCursorPosition(h, position);
			if (maze[position.Y][position.X] == MazeObject::CROSS)
			{
				SetConsoleTextAttribute(h, 13);
				cout << "X";
			}
			else
			{
				SetConsoleTextAttribute(h, 0);
				cout << " ";
			}

			if (code == KeyCode::ENTER)
			{
				int radius = 1;
				for (int y = position.Y - radius; y <= position.Y + radius; y++)
					for (int x = position.X - radius; x <= position.X + radius; x++)
					{
						if (x >= 0 && x <= WIDTH - 1 &&
							y >= 0 && y <= HEIGHT - 1 &&
							maze[y][x] == MazeObject::ENEMY)
						{
							maze[y][x] = MazeObject::CROSS;
							COORD temp;
							temp.Y = y;
							temp.X = x;
							SetConsoleCursorPosition(h, temp);
							SetConsoleTextAttribute(h, 13);
							cout << "X";
						}
					}
			}
			else if (code == KeyCode::ESCAPE)
			{
				cout << "ESCAPE!\n";
				system("cls");
				exit(0);
			}
			else if (code == KeyCode::SPACE)
			{
				cout << "SPACE!\n";
				system("cls");
				exit(0);
			}
			else if (code == KeyCode::LEFT
				&& maze[position.Y][position.X - 1] != MazeObject::WALL
				&& maze[position.Y][position.X - 1] != MazeObject::BORDER)
			{
				position.X--;
			}
			else if (code == KeyCode::RIGHT // если я собрался пойти направо
				&& maze[position.Y][position.X + 1] != MazeObject::WALL
				&& maze[position.Y][position.X + 1] != MazeObject::BORDER)
				// и при этом в лабиринте на той же строке (где смайлик) и
				// немного (на одну ячейку) правее на 1 столбец от смайлика
			{
				position.X++;
			}
			else if (code == KeyCode::UP
				&& maze[position.Y - 1][position.X] != MazeObject::WALL
				&& maze[position.Y - 1][position.X] != MazeObject::BORDER)
			{
				position.Y--;
			}
			else if (code == KeyCode::DOWN
				&& maze[position.Y + 1][position.X] != MazeObject::WALL
				&& maze[position.Y + 1][position.X] != MazeObject::BORDER)
			{
				position.Y++;
			}

			// показ персонажика в новой позиции
			SetConsoleCursorPosition(h, position);
			SetConsoleTextAttribute(h, Color::BLUE);
			cout << (char)2;

			////////////////////////////////////////////////////////////////
			// пересечение с элементами массива
			if (position.Y == HEIGHT - 3 &&
				position.X == WIDTH - 1)
			{
				MessageBoxA(0, "выход", "победа!", 0);
				system("cls");
				exit(0);
				// main(); // для запуска сначала но на другой рандомной локации
			}

			// пересечение с монетками
			// если в лабиринте по позиции смайлика (под ним) находится монетка
			if (maze[position.Y][position.X] == MazeObject::COIN)
			{
				coins++; // на одну монетку собрали больше
				ShowCoins();
				maze[position.Y][position.X] = MazeObject::HALL; // убираем монетку из лабиринта
			}
			if (position.Y > 0 && position.X > 0)
				steps++;
			ShowSteps();
            // пересечение с врагами
			// если в лабиринте по позиции смайлика (под ним) находится враг
			if (maze[position.Y][position.X] == MazeObject::ENEMY)
			{
				health -= 10;
				ShowHealth();


				if (health <= 0)
				{
					int answer = MessageBoxA(0, "здоровье закончилось!\n\nхотите начать сначала?", "ПОРАЖЕНИЕ!", MB_YESNO);
					system("cls");
					if (answer == IDYES)
					{
						main();
					}
					else
					{
						exit(0);
					}
				}
			}
		}
		else // нажатия не было, двигаем врагов
		{
			Sleep(15);
			///////////////////////////////////////////////////////////////
			// движение врагов
			COORD enemy_positions[100]; // массив который хранит координаты врагов
			int enemy_count = 0; // реальное количество врагов на данный момент после шага ГГ

			// перебор всех ячеек которые есть в лабиринте
			for (int y = 0; y < HEIGHT; y++) // перебор строк
			{
				for (int x = 0; x < WIDTH; x++) // перебор столбцов
				{
					// если очередная ячейка - это враг
					if (maze[y][x] == MazeObject::ENEMY)
					{
						enemy_positions[enemy_count].X = x;
						enemy_positions[enemy_count].Y = y;
						enemy_count++;
					}
				}
			}

			// перебор всех врагов
			for (int i = 0; i < enemy_count; i++)
			{
				int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
				if (r == LEFT &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::CROSS)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];

					SetConsoleCursorPosition(h, temp);

					if (maze[temp.Y][temp.X] == MazeObject::CROSS)
					{

						SetConsoleTextAttribute(h, 13);
						cout << "X";
					}
					else
					{
						SetConsoleTextAttribute(h, 0);
						cout << " ";
					}

					if (maze[temp.Y][temp.X] != MazeObject::CROSS)
						maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X - 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObject::ENEMY;
				}
				else if (r == RIGHT &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::CROSS)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];

					SetConsoleCursorPosition(h, temp);

					if (maze[temp.Y][temp.X] == MazeObject::CROSS)
					{

						SetConsoleTextAttribute(h, 13);
						cout << "X";
					}
					else
					{
						SetConsoleTextAttribute(h, 0);
						cout << " ";
					}

					if (maze[temp.Y][temp.X] != MazeObject::CROSS)
						maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X + 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObject::ENEMY;
				}

				else if (r == UP &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALL &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::CROSS)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];

					SetConsoleCursorPosition(h, temp);

					if (maze[temp.Y][temp.X] == MazeObject::CROSS)
					{

						SetConsoleTextAttribute(h, 13);
						cout << "X";
					}
					else
					{
						SetConsoleTextAttribute(h, 0);
						cout << " ";
					}

					if (maze[temp.Y][temp.X] != MazeObject::CROSS)
						maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y - 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObject::ENEMY;
				}
				else if (r == DOWN &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALL &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::ENEMY &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::CROSS)
				{
					// стирание врага в старой позиции
					COORD temp = enemy_positions[i];

					SetConsoleCursorPosition(h, temp);

					if (maze[temp.Y][temp.X] == MazeObject::CROSS)
					{

						SetConsoleTextAttribute(h, 13);
						cout << "X";
					}
					else
					{
						SetConsoleTextAttribute(h, 0);
						cout << " ";
					}

					if (maze[temp.Y][temp.X] != MazeObject::CROSS)
						maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// перемещаем врага в новую позицию
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y + 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObject::ENEMY;
				}
			}
		}
	}
}