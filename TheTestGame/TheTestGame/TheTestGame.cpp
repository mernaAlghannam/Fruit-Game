//Final Project: The falling fruits game
//CIT 120 - Merna Alghannam - May 6,2019
#include <windows.h>	// for  Sleep and SetConsoleTextAttributes
#include <iostream>	
#include <conio.h>		// for getch and kbhit
#include <fstream>
#include <cstdlib>	// for rand and srand
#include <ctime>  // time function
//#include <ncurses>
using namespace std;

void Intro();	// Introduce the game
void inpute();	// The inpute would move or pause the game (reads whatever you have entered)
void Canvas();	// Where aall the main fruit game is process and outputted once per frame
void GameOver();	// if you missed the target, you go to gameOver function
int FruitProcess();	// regenerates the type of fruit whenever it is called and where is should reapear at the top
void SetColor();	//sets color of fruits
void SaveToFile();
void AccessFile();

int x = 17;	// used in multiple places
int gameOver = false, started = true;	// since it is used in many places (determines the end or continuation of a game)
char fruit;		// since it is used in many places (it is for the type of fruit)
const int row = 21, col = 41;
int BestScore;	// initiates as 0 in the beginning of the game

int main()
{
	Intro();
	AccessFile();	// accesses the best score from the file

	while (gameOver == false)	// Loop through until the game is false (you woulld see the game and then the input would change the movement of the target)
	{
		Canvas();	// would refresh throughout the loop
		inpute();	//reads the input from keyboard
		Sleep(30);	// slows down the game
	}

	SaveToFile();	// Saves the best score
	exit(0);

	system("pause");
	return 0;
}

void Intro()
{
	cout << "\n\n\n\n\n\n\n\t\tWelcome to this wonderful incredible Great Game (trust me you will never see anything like it)\n\n"
		<< "\t\tPlease press 'a' or 'd' to try to catch the fruits.\n"
		<< "\n\n\n\t\tPlease press any key to start the game.\n\n\n";
	if (_getch())	// It waits for you press any key in order for you to go back to the game
	{
		started = true;
		system("cls");
		Canvas();
	}
}

void inpute()	// check your input and gives changes the game accordingly
{
	static int show;

	if (_kbhit())	// if you pressed any key
	{
		switch (_getch())
		{
		case 'a':	// If pressed, the target would go left
		case 'A':
			if (x < 2) // if this the target would restart from the end of the right
				x = 36;
			else
				x--;
			break;
		case 'd':
		case 'D':	// if pressed, the target would go right
			if (x > 35)	// if this, the target would restart from the beggining of the left
				x = 1;
			else
				x++;
			break;
		case 'm':	// pressed iy would go to the intro function and pauses the game
		case 'M':
			system("cls");
			Intro();
			break;
		default:	// any other button would show the input validation message untill show becomes less than 0
			show = 40;
			COORD V = { 0, 26 };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), V);	// moves the cursor to (0, 26) and outputs the input validation message
			cout << "Please press 'a' or 'd'";	// dissapears in 40 loops
		}
	}

	if (show < 0)
	{
		COORD N = { 0, 26 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), N);
		cout << "                       ";
	}
	else
		show--;
}

void Canvas() {

	static char f = FruitProcess();
	static int y = 1, score = 0, pause = 6, m=0;
	static bool Score;

	Score = false;

	char TheGame[23][41] = // where everything will be added in this function
	{
		"########################################",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"#                                      #",
		"########################################",
	};

	TheGame[21][x] = TheGame[21][x + 1] = TheGame[21][x + 2] = '_';	// the target would move according the value of the x (which changes using 'a' and 'b' "__" is the target


	TheGame[y][f] = fruit;	//// outputs the fruit everytime y increases

	for (int i = 0;i < 23;i++)
		TheGame[0][i] = '#';

	if (pause == 0)		// This is to slowdown the movement of the fruit (by one), slower than the amount of times you can move the target.
	{	
		pause = 6-m;	// The pause variable does that
		
		if ((score % 5)== 0 && score>0 && m != 6 && y==1)	//if pause is greater than 0 and the m increases every five points
			m++;

		if (y == 22)	// this is for the fruit to go down on the y-axis (if this, go back to the ttop and regenerate) else continue going down
		{
			y = 1;
			f = FruitProcess();
		}
		else
			y++;
	}
	else
		pause--;

	if (TheGame[21][x] == fruit || TheGame[21][x + 1] == fruit || TheGame[21][x + 2] == fruit)	// this is for score counter if the fruit is right on top the target
	{
		if (pause == 0)	
			score++;

		Score = true;
	}

	if (started)
	{
		system("cls");
		for (int i = 0; i < 23; i++)	// outputs the entire array after all the changes were made all so sets their text colors
		{
			cout << TheGame[i]<< endl;
		}
		//this is to output the scores at the bottom
		cout << "Score: " << score << endl;
		cout << "Best Score: " << BestScore << endl;
		cout << "Press 'm' to go to menu.\n\n";
	}
	else
	{
		if (y > 1)
		{
			COORD pos = { f, y - 1 }; 
			//mvprintw(8,7,"n");
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);		//prints space before fruit to erase the former fruit
			cout << " ";
		}

		COORD e = { 1, 21 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), e);		//prints space underneath the target
		cout << "                                      ";

		if (y < 22)
		{
			COORD po = { f, y };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), po);	//prints fruit per loop
			SetColor();
			cout << fruit;
		}

		COORD p = { x, 21 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);	//prints target
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);	//resets the color to white for anything not a fruit
		cout << "___";

		COORD s = { 7, 23 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), s);	//prints the score
		cout << score;
	}

	started = false;

	if (y == 21 && Score == false && pause == 0)	//if the fruit is at the bottom of the game but not on top of the target, you check what the best score is and you go to gameover function
	{
		if (score > BestScore)
			BestScore = score;

		score = 0;
		y = 1;
		m = 0;
		f = FruitProcess();
		GameOver();
	}

}

void GameOver()
{
	bool pressCorrect = false;
	bool pressed = false;
	int y;	// why determines where '#' will be cicled

	while (!pressCorrect)	// this will loop through untill you choose 'y', not before choosing either 's' or 'w'
	{
		system("cls");	// this will cause it to refresh with the new outputs everythung loop
		cout << "\n\n\t\t\t\t\tDo you want to try again? \n";
		cout << "\t\t\t\t\tPress 'w' or 's' to select and 'y' to confirm.\n\n";
		char Save[10][8] =	// this is where the '#' would circle around the yes or no
		{
			"       ",
			"       ",
			"  YES  ",
			"       ",
			"       ",
			"       ",
			"       ",
			"   NO  ",
			"       ",
			"       ",
		};

		if (_kbhit())	// if you pressed any button
		{
			switch (_getch())
			{
			case 'w':
			case 'W': // you would select this if you want to try again and 'w' is going up
				y = 0;
				gameOver = false;
				pressed = true;
				break;
			case 's':
			case 'S':	// you selected this if you want to exit and 's' is going down
				y = 5;
				gameOver = true;
				pressed = true;
				break;
			case 'y':
			case 'Y': //you press 'y' to confirm only if selected either w or s before it and this is to confirm your choice and excit the loop and the fuction
				if (pressed)
					pressCorrect = true;
			}
		}

		//if you selected s or w, '#' would circle around either the yes or no
		if (pressed)
		{
			//print x-axis
			for (int i = 0;i < 8;i++)
			{
				Save[y][i] = '#';
				Save[y + 4][i] = '#';
			}
			//print y-axis
			for (int i = y + 1;i < (y + 4);i++)
			{
				Save[i][0] = '#';
				Save[i][7] = '#';
			}
		}
		//prints the array after the selection
		for (int i = 0; i < 10; i++)
		{
			cout << "\t\t\t\t\t\t\t";
			for (int j = 0; j < 8; j++)
			{
				cout << Save[i][j];
			}
			cout << endl;
		}
	}
	started = true;
	system("cls");
}

int FruitProcess()	// this is where the type of fruit and where it is going to regenerate at the top at random
{
	char Fruits[] = { 'G', 'L', 'A', 'B' };

	srand(static_cast<unsigned int>(time(0)));

	int num = rand() % 4;
	fruit = Fruits[num];

	int FruitPlacement = rand() % 37 + 1;

	return FruitPlacement;
}

void SaveToFile()	// this is to save best score to file
{
	ofstream SaveScore("BestScore.txt");
	SaveScore << BestScore;
	SaveScore.close();
}

void AccessFile()	// this is to access best score from the file
{
	ifstream TakeScore("BestScore.txt");
	TakeScore >> BestScore;
	TakeScore.close();
}

void SetColor()	//sets color of the fruit
{
	switch (fruit)
	{
	case 'G':	//sets light cyan
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		break;
	case 'B':	//sets yellow
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		break;
	case 'A':	//sets light red
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		break;
	case 'L':	//sets light green
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		break;
	}
}