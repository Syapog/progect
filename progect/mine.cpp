#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;

struct PlayerInventory
{
	int coins = 0;
	int bombs = 0;
	int pickaxes = 0;
	int springBoots = 0;
};
struct Bomb
{
	bool isPlanted = false;
	int timer = 2;
	int x;
	int y;
};
void Menu();
void Play();
void BombExplosion();
void DrawnMap();
void Action();
void PickUpItems();
void ShowStatistics();
int CoinCounting();
void EndGame();
void LevelsChoice();
void ShowRules();
void OpenLvl(int, bool);
void LoadSave();
void SaveLvl();
void Save();
char* CreatePath(int);
void SetCursorVisibility(bool);
void SetCursorPosition(int, int);

PlayerInventory inventory;
Bomb bomb;
int lastLvl = 0;
int nowLvl = 0;
int bestResult = 0;
int requiredResult = 0;
int lvlCount = 3;
int selectedItem = 0;
int moves = 0;
int input;
int playerX = 1, playerY = 1;
bool isItemUsed = false;
bool isPlaying = false;
char map[10][20] = { {'#',' ',' ',' ',' ','o',' ',' ','L',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' '},
					 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
					 {' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#'},
					 {' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#'},
					 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','0',' ',' ',' ','#',' ','#'},
					 {' ',' ','P',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ','#'},
					 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
					 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
					 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
					 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '} };

int main()
{
	setlocale(LC_ALL, "Russian");

	LoadSave();
	Menu();
	system("pause");
}

void Menu()
{

	bool isWorking = true;
	while (isWorking)
	{
		SetCursorVisibility(true);
		system("cls");
		cout << "*������� ����*" << endl;
		cout << "1. ������/����������" << endl;
		cout << "2. ����� �������" << endl;
		cout << "3. ������� � ����������" << endl;
		cout << "4. ����� �� ����" << endl;
		cout << "�������� ����� ����: ";
		cin >> input;
		switch (input)
		{
		case 1:
			nowLvl = lastLvl;
			Play();
			break;
		case 2:
			LevelsChoice();
			break;
		case 3:
			ShowRules();
			break;
		case 4:
			isWorking = false;
			break;
		default:
			cout << "�������������� ����� ����." << endl;
			system("pause");
			break;
		}
	}
}

void Play()
{
	system("cls");
	SetCursorVisibility(false);
	moves = 0;
	inventory.coins = 0;
	isPlaying = true;
	OpenLvl(nowLvl, false);
	while (isPlaying)
	{
		if (bomb.isPlanted)
			BombExplosion();
		DrawnMap();
		ShowStatistics();
		Action();
		PickUpItems();
		system("cls");
	}
}

void BombExplosion()
{
	if (bomb.timer <= 0)
	{
		for (int i = bomb.y - 1; i <= bomb.y + 1; i++)
		{
			for (int j = bomb.x - 1; j <= bomb.x + 1; j++)
			{
				if (i == playerY && j == playerX)
					moves += 10;
				map[i][j] = ' ';
			}
		}
		bomb.isPlanted = false;
		bomb.timer = 2;
	}
	else if (bomb.timer > 0)
		bomb.timer--;
}

void DrawnMap()
{
	SetCursorPosition(0, 0);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			cout << map[i][j];
		}
		cout << "\n";
	}
}

void Action()
{
	if (playerX > 19)
		playerX = 19;
	if (playerX < 0)
		playerX = 0;
	if (playerY > 9)
		playerY = 9;
	if (playerY < 0)
		playerY = 0;
	SetCursorPosition(playerX, playerY);
	cout << '@';

	input = _getch();
	if (input == 27)
	{
		isPlaying = false;
	}
	if (input == 230 || input == 150 || input == 119 || input == 87)
	{
		if (selectedItem != 0)
			selectedItem--;
		else
			selectedItem = 2;
	}
	if (input == 155 || input == 235 || input == 115 || input == 83)
	{
		if (selectedItem != 2)
			selectedItem++;
		else
			selectedItem = 0;
	}

	if (input == 102 || input == 70 || input == 128 || input == 160)
	{
		if (selectedItem == 0)
		{
			if (!bomb.isPlanted && inventory.bombs > 0)
			{
				inventory.bombs--;
				bomb.isPlanted = true;
				bomb.x = playerX;
				bomb.y = playerY;
				map[bomb.y][bomb.x] = 'O';
				moves++;
			}
		}
		else
		{
			input = _getch();
			isItemUsed = true;
		}
	}

	if (input == 224)
	{
		input = _getch();
		switch (input)
		{
		case 75:
			if (map[playerY][playerX - 1] != '#')
				playerX--;
			else if (selectedItem == 1 && inventory.pickaxes > 0 && isItemUsed)
			{
				map[playerY][playerX - 1] = ' ';
				inventory.pickaxes--;
			}
			else if (selectedItem == 2 && inventory.springBoots > 0 && map[playerY][playerX - 2] != '#' && isItemUsed)
			{
				playerX -= 2;
				inventory.springBoots--;
			}
			moves++;
			break;
		case 72:
			if (map[playerY - 1][playerX] != '#')
				playerY--;
			else if (selectedItem == 1 && inventory.pickaxes > 0 && isItemUsed)
			{
				map[playerY - 1][playerX] = ' ';
				inventory.pickaxes--;
			}
			else if (selectedItem == 2 && inventory.springBoots > 0 && map[playerY - 2][playerX] != '#' && isItemUsed)
			{
				playerY -= 2;
				inventory.springBoots--;
			}
			moves++;
			break;
		case 77:
			if (map[playerY][playerX + 1] != '#')
				playerX++;
			else if (selectedItem == 1 && inventory.pickaxes > 0 && isItemUsed)
			{
				map[playerY][playerX + 1] = ' ';
				inventory.pickaxes--;
			}
			else if (selectedItem == 2 && inventory.springBoots > 0 && map[playerY][playerX + 2] != '#' && isItemUsed)
			{
				playerX += 2;
				inventory.springBoots--;
			}
			moves++;
			break;
		case 80:
			if (map[playerY + 1][playerX] != '#')
				playerY++;
			else if (selectedItem == 1 && inventory.pickaxes > 0 && isItemUsed)
			{
				map[playerY + 1][playerX] = ' ';
				inventory.pickaxes--;
			}
			else if (selectedItem == 2 && inventory.springBoots > 0 && map[playerY + 2][playerX] != '#' && isItemUsed)
			{
				playerY += 2;
				inventory.springBoots--;
			}
			moves++;
			break;
		default:
			break;
		}
	}
	isItemUsed = false;
}

void PickUpItems()
{
	switch (map[playerY][playerX])
	{
	case 'T':
		inventory.coins++;
		map[playerY][playerX] = ' ';
		if (CoinCounting() == 0)
			EndGame();
		break;
	case 'B':
		inventory.bombs++;
		map[playerY][playerX] = ' ';
		break;
	case 'P':
		inventory.pickaxes++;
		map[playerY][playerX] = ' ';
		break;
	case 'S':
		inventory.springBoots++;
		map[playerY][playerX] = ' ';
		break;
	default:
		break;
	}
}

int CoinCounting()
{
	int number = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (map[i][j] == 'T')
				number++;
		}
	}
	return number;
}

void ShowStatistics()
{
	for (int i = 0; i < 20; i++)
	{
		SetCursorPosition(i, 10);
		cout << "=";
	}
	for (int i = 0; i < 11; i++)
	{
		SetCursorPosition(20, i);
		cout << "|";
	}
	SetCursorPosition(28, 0);
	cout << "*������� " << nowLvl << "*";
	SetCursorPosition(30, 1);
	cout << "����: " << moves;
	SetCursorPosition(30, 2);
	cout << "����� �����������: " << requiredResult;
	SetCursorPosition(30, 3);
	cout << "������: ";
	if (bestResult == 0)
		cout << "-";
	else
		cout << bestResult;
	SetCursorPosition(30, 4);
	cout << "������� ������: " << inventory.coins;
	SetCursorPosition(30, 7);
	cout << "--------------------------";
	SetCursorPosition(30, 9);
	cout << "�����: " << inventory.bombs;
	SetCursorPosition(30, 10);
	cout << "�����: " << inventory.pickaxes;
	SetCursorPosition(30, 11);
	cout << "������� �� ���������: " << inventory.springBoots;
	SetCursorPosition(28, 9 + selectedItem);
	cout << ">";
}

void EndGame()
{
	bool isWorking = true;
	isPlaying = false;
	system("cls");
	cout << "*���� ��������*" << endl;
	cout << "���������� �����: " << moves << endl;
	if (bestResult > moves || bestResult == 0)
	{
		cout << "����� ������! ";
		if (bestResult != 0)
			cout << "������� ������ : " << bestResult << endl;
		else
			cout << endl;
		bestResult = moves;
	}
	else
	{
		cout << "������: " << bestResult << endl;
	}
	if (nowLvl == lastLvl)
	{
		if (lastLvl < lvlCount)
		{
			cout << "��������� ��� ������������� ���������� ������: �� ������ " << requiredResult << " �����." << endl;
			if (requiredResult >= moves)
			{
				cout << "�� �������������� ��������� �������!" << endl;
			}
			else
			{
				cout << "������� ����� ����� ��� ������������� ���������� ������." << endl;
			}
		}
		else
		{
			cout << "������� �� ����������� ����!" << endl;
		}
	}
	SaveLvl();
	cout << "\n" << endl;

	SetCursorVisibility(true);
	cout << "1. ������ ������� ������" << endl;
	cout << "2. ����� � ����" << endl;
	if (nowLvl == lastLvl && requiredResult >= moves && lastLvl < lvlCount)
	{
		cout << "3. ��������� �������." << endl;
		lastLvl++;
		Save();
	}
	else if (nowLvl + 1 <= lastLvl)
	{
		cout << "3. ��������� �������." << endl;
	}
	while (isWorking)
	{
		cout << "�������� ����� ����: ";
		cin >> input;
		switch (input)
		{
		case 1:
			isWorking = false;
			Play();
			system("cls");
			break;
		case 2:
			isWorking = false;
			system("cls");
			break;
		case 3:
			if (nowLvl == lastLvl - 1 && requiredResult >= moves)
			{
				isWorking = false;
				nowLvl = lastLvl;
				Play();
				system("cls");
			}
			else if (nowLvl + 1 <= lastLvl - 1)
			{
				isWorking = false;
				nowLvl++;
				Play();
			}
			else
			{
				cout << "�������������� ����� ����." << endl;
				system("pause");
			}
			break;
		default:
			cout << "�������������� ����� ����." << endl;
			system("pause");
			break;
		}
	}
}

void LevelsChoice()
{
	bool isWorking = true;
	while (isWorking)
	{
		system("cls");
		cout << "*����� ������*" << endl;
		for (int i = 1; i <= lvlCount; i++)
		{
			OpenLvl(i, true);
			cout << "������� " << i;
			if (i > lastLvl)
				cout << " - ������" << endl;
			else
				cout << endl;
			cout << "  ����� �����������: " << requiredResult << endl;
			cout << "  ������: ";
			if (bestResult == 0)
				cout << "-" << endl;
			else
				cout << bestResult << endl;
			cout << endl;
		}
		cout << "����� � ���� - " << lvlCount + 1 << endl;
		cout << endl;
		cout << "�������� �������: ";
		cin >> input;
		if (input < 1 || input > lvlCount)
		{
			if (input == lvlCount + 1)
				isWorking = false;
			else
			{
				cout << "�������������� ����� ����." << endl;
				system("pause");
			}
		}
		else
		{
			if (input > lastLvl)
			{
				cout << "�������� ���������� �������, ����� ������� � ����." << endl;
				system("pause");
			}
			else
			{
				nowLvl = input;
				isWorking = false;
				Play();
			}
		}
	}
}

void ShowRules()
{
	system("cls");
	cout << "���� ����." << endl;
	cout << "  �� ������� �� ������ ������ (@). ���� ���� - ������� ��� ����� (T) �� ������ �� ����������� ���������� �����, \n����������� ����������� � ���� ���� (#). � ���� ��� ������� ��������� ��������, ���� � ������� ����� ���� �����.\n������ ������� ��� ���������� ��������� �� ����������� ���������� ����� (����� �����������), ����� ������� ���������." << endl;
	cout << "����������." << endl;
	cout << "  ���������� ���������� �������������� � ������� ������� �� ����������. ������ ������� �� ������ ��������� �� ���. \n��� �� �� ������ ������������� ����� ���������� � ����� ��������� � ������� ������� W � S. \n������������ �������� �� ������� F." << endl;
	cout << "��������." << endl;
	cout << "� ���� ���������� ��� ���� ��������������� ���������: �����, ����� � ������� �� ���������. \n������ ������� �������� ����������� � �������� ��� �������������. ��������� � ���������:" << endl;
	cout << "  �����(B). �������� ��� �������� � ����� ������ ����. ��� ������ ����� �������� � ������� F ����� \n������ ��� ���� �����(O). � ���� ���� ��� ���� ����� ���������� �� �� ����-�� �� ���� ������, \n����� ����� ��� ����������. ���� ����� ������ � ������ ������, �� ������� 10 �������� �����." << endl;
	cout << "  �����(P). ���������� ���� ������ �����, ��������� ������� � ���������� �������� � ����. \n��� ������� F ����� ������ ������� �����������, � ������� ����� ������. ����, � ����� ����������� �������� � ���� ����� ������ �����, �� ��� ����������." << endl;
	cout << "  ������� �� ���������(S). �������� ��, ����� ������������� ���� ������������� �������� � ���� ����� \n� ��������� �����������, ���� �� ���� ������ ��� ��� �����." << endl;
	system("pause");
}


void OpenLvl(int lvl, bool showOnlyResults)
{
	ifstream F(CreatePath(lvl), ios::in);
	if (!F)
	{
		cout << "File" << CreatePath(lvl) << "not found\n";
		system("pause");
	}
	else
	{
		F >> bestResult >> requiredResult;
		if (!showOnlyResults)
		{
			F >> playerX >> playerY >> inventory.bombs >> inventory.pickaxes >> inventory.springBoots;
			for (int i = 0; i < 10; i++)
			{
				F >> map[i];
			}
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (map[i][j] == '.')
						map[i][j] = ' ';
				}
			}
		}
		F.close();
	}
}

void SaveLvl()
{
	int t = bestResult;
	OpenLvl(nowLvl, false);
	ofstream F(CreatePath(nowLvl), ios::out);
	F << t << " " << requiredResult << " " << playerX << " " << playerY << " " << inventory.bombs << " " << inventory.pickaxes << " " << inventory.springBoots << "\n";
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (map[i][j] == ' ')
				map[i][j] = '.';
			F << map[i][j];
		}
		F << "\n";
	}
	F.close();
}

void LoadSave()
{
	ifstream F("save.txt", ios::in);
	if (!F) cout << "File not found\n";
	else
	{
		F >> lastLvl;
		F.close();
	}
}

void Save()
{
	ofstream F("save.txt", ios::out);
	F << lastLvl;
	F.close();
}

void SetCursorPosition(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetCursorVisibility(bool visibility)
{
	CONSOLE_CURSOR_INFO cursor = { 1, visibility };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

char* CreatePath(int lvl)
{
	char* t = new char[20];
	sprintf_s(t, 19, "Lvls//Lvl%i%s", lvl, ".txt");
	return t;
}
