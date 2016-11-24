//Coded by: Ashkan.rmk
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
using namespace std;

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

void color(int number)
{
	HANDLE color;
	color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, number);
}

struct node
{
	int *transition;
	int num_state;
	int transit_num;
};

node *state;
int total_state_num;
int alphabet_num;
char *possible_transit;
int acc_state_num;
int *final_accept_state;
int start_state_num;

void Init_DFA_Manual()
{
	system("cls");

	fstream output;
	output.open("DFA.txt", ios::out);	//Make File
	if (!output)
	{
		cerr << "File can not open." << endl;
		exit(1);
	}

	cout << "\n\n\tEnter MAXIMUM DFA States:" << endl;
	cout << "\t-> ";
	cin >> total_state_num;
	output << total_state_num << endl;
	state = new node[total_state_num];

	cout << "\n\n\tEnter MAXIMUM DFA alphabet numbers:" << endl;
	cout << "\t-> ";
	cin >> alphabet_num;
	output << alphabet_num << endl;

	cout << "\n\n";
	possible_transit = new char[alphabet_num];
	for (int i = 0; i < alphabet_num; i++)
	{
		cout << "\tEnter " << i + 1 << "th alphabet: ";
		cin >> possible_transit[i];
		output << possible_transit[i] << endl;
	}

	cout << "\n\n\tEnter Total accepted states number:" << endl;
	cout << "\t-> ";
	cin >> acc_state_num;
	output << acc_state_num << endl;
	final_accept_state = new int[acc_state_num];
	for (int i = 0; i < acc_state_num; i++)
	{
		cout << "\tEnter " << i + 1 << "th accepted state number: ";
		cin >> final_accept_state[i];
		output << final_accept_state[i] << endl;
	}

	system("cls");
	color(11);
	cout << "\n\n\t\t\t* Now Initial DFA States *\n\n";
	color(15);
	for (int i = 0; i < total_state_num; i++)
	{
		cout << "\n\t------------\n";
		state[i].transit_num = alphabet_num;
		cout << "\n\tEnter " << i + 1 << "th state number: ";
		cin >> state[i].num_state;
		output << state[i].num_state << endl;
		state[i].transition = new int[alphabet_num];
		for (int j = 0; j < alphabet_num; j++)
		{
			cout << "\n\tWhich state will go with transition " << possible_transit[j] << "? ";
			cin >> state[i].transition[j];
			output << state[i].transition[j] << endl;
		}
	}

	system("cls");
	cout << "\n\n\tEnter Start State Number: " << endl;
	cout << "\t-> ";
	cin >> start_state_num;
	output << start_state_num << endl;

	system("cls");
	output.close();	//Close File
	color(11);
	cout << "\n\n\n\t\t\tYour DFA completely Initialized!" << endl;
	color(15);
	_getch();
	system("cls");
}

void Read_DFA_asFile()
{
	ifstream inputFile("DFA.txt", ios::in | ios::beg);
	if (!inputFile)	//check for file opening
	{
		cerr << "some thing wrong during opening file!" << endl;
		_getche();
		exit(1);
	}

	inputFile >> total_state_num;
	state = new node[total_state_num];

	inputFile >> alphabet_num;

	possible_transit = new char[alphabet_num];
	for (int i = 0; i < alphabet_num; i++)
	{
		inputFile >> possible_transit[i];
	}

	inputFile >> acc_state_num;
	final_accept_state = new int[acc_state_num];
	for (int i = 0; i < acc_state_num; i++)
	{
		inputFile >> final_accept_state[i];
	}

	for (int i = 0; i < total_state_num; i++)
	{
		state[i].transit_num = alphabet_num;
		inputFile >> state[i].num_state;
		state[i].transition = new int[alphabet_num];
		for (int j = 0; j < alphabet_num; j++)
		{
			inputFile >> state[i].transition[j];
		}
	}

	inputFile >> start_state_num;

	color(11);
	cout << "\n\n\n\t\t\tYour DFA has been Set!" << endl;
	color(15);
	_getch();
	system("cls");
}

void Check_String()
{
	string input;
	bool goal;	//checking foreign alphabet
	bool check;	//checking accept state
	bool CON = true;	
	int START;		//save Start State Number
	int lng;		//Check number of possible alphabets
	while (CON)
	{
		goal = false;
		check = true;
		lng = 0;
		START = start_state_num;
		system("cls");
		color(7);
		cout << "\n\n\n\n\t\tEnter Your String:" << endl;
		cout << "\n\t\t-> ";
		cin >> input;

		system("cls");
		gotoxy(35, 5);
		color(6);
		cout << input;

		/////////Show Heads Var
		int count = 35;
		int c = 3;

		////////Check For Foreign Alphabets
		for (int i = 0; input[i]; i++)
		{
			for (int j = 0; possible_transit[j]; j++)
			{
				if (input[i] == possible_transit[j])
				{
					lng++;
				}
			}

		}
		if (lng == input.size())
		{
			goal = true;
		}
		else
		{
			gotoxy(5, 5);
			cout << "Input Foreign Alphabet in your string";
		}
		/////////////////////////////////////////

		gotoxy(0, 1);
		cout << "* States Status *\n\n";
		for (int i = 0; input[i] && goal; i++)
		{
			Sleep(1000);
			gotoxy(count, 6);
			cout << "^";
			gotoxy(count, 7);
			cout << "|";
			count++;
			gotoxy(0, c++);
			for (int j = 0; j < total_state_num ; j++)	
			{
				if (state[j].num_state == START)
				{
					for (int k = 0; k < state[j].transit_num; k++)
					{
						if (input[i] == possible_transit[k])
						{
							cout << "q" << START << "  --" << input[i] << "->  " << "q" << state[j].transition[k] << endl;
							START = state[j].transition[k];
							break;
						}
					}
				}
			}
		}



		for (int i = 0; i < acc_state_num; i++)
		{
			if (final_accept_state[i] == START && goal)
			{
				color(3);
				gotoxy(40, 9);
				cout << "\n\n\t\tAccept!\n\n" << endl;
				check = false;	//if state accepted
				system("pause>0");
			}
		}

		if (check)
		{
			color(4);
			gotoxy(40, 9);
			cout << "\n\n\t\tReject!\n\n" << endl;
			system("pause>0");
		}

		cout << "\n\n\n\t\t\tAGAIN?\n\t\t1) YES\n\t\t2) NO\n";
		cout << "\n\t\t-> ";
		int ch;
		cin >> ch;
		if (ch == 1)
			CON = true;
		else
		{
			CON = false;
		}
	}
}

void about()
{
	system("cls");
	gotoxy(27, 10);
	cout << "DFA Simulator v 1.0";
	gotoxy(27, 12);
	cout << "Coded By Ashkan.rmk";
	gotoxy(27, 14);
	cout << ":D";
	_getch();
	system("cls");
}

void Loading()
{
	system("cls");
	gotoxy(20, 7);
	cout << "====================================";
	gotoxy(20, 13);
	cout << "====================================";
	gotoxy(32, 9);
	cout << "LOADING...";
	gotoxy(35, 11);
	Sleep(200);
	cout << "* ";
	Sleep(200);
	system("color 06");
	cout << "* ";
	Sleep(200);
	system("color 07");
	cout << "* ";
	gotoxy(53, 5);
	cout << ":) Your DFA in progress...";
	gotoxy(41, 11);
	Sleep(200);
	system("color 08");
	cout << "* ";

	gotoxy(35, 11);
	Sleep(200);
	cout << "  ";
	Sleep(200);
	system("color 06");
	cout << "  ";
	Sleep(200);
	system("color 07");
	cout << "  ";
	Sleep(200);
	system("color 08");
	cout << "  ";

	gotoxy(35, 11);
	Sleep(200);
	cout << "* ";
	Sleep(200);
	system("color 06");
	cout << "* ";
	Sleep(200);
	system("color 07");
	cout << "* ";
	Sleep(200);
	system("color 08");
	cout << "* ";
	system("cls");
}

void exit_menu()
{
	system("cls");
	char ch;
	int select = 2;

	while (1)
	{
		system("cls");
		color(20);
		gotoxy(28, 6);
		cout << "Are you Sure to EXIT?";

		if (select == 1)
			color(11);
		else
			color(15);

		gotoxy(28, 10);
		cout << "YES";

		if (select == 2)
			color(11);
		else
			color(15);

		gotoxy(28, 12);
		cout << "NO";

		ch = _getch();
		switch (ch)
		{
		case 72:	//Up
			select--;
			if (select == 0)
				select = 2;
			break;
		case 80:	//Down
			select++;
			if (select == 3)
				select = 1;
			break;
		case 13:	//Enter
			if (select == 1)
				exit(1);
			if (select == 2)
			{
				return;
			}
		}
	}

}

void menu()
{
	system("cls");
	char ch;
	int select = 1;

	while (1)
	{
		system("cls");
		color(10);
		gotoxy(33, 6);
		cout << "DFA Simulator";

		if (select == 1)
			color(11);
		else
			color(15);

		gotoxy(28, 10);
		cout << "Check Strings with Saved DFA";

		if (select == 2)
			color(11);
		else
			color(15);

		gotoxy(28, 12);
		cout << "Input New DFA and Save it";

		if (select == 3)
			color(11);
		else
			color(15);

		gotoxy(28, 14);
		cout << "About";

		if (select == 4)
			color(11);
		else
			color(15);

		gotoxy(28, 16);
		cout << "Exit";


		ch = _getch();
		switch (ch)
		{
		case 72:	//Up
			select--;
			if (select == 0)
				select = 4;
			break;
		case 80:	//Down
			select++;
			if (select == 5)
				select = 1;
			break;
		case 13:	//Enter
			if (select == 1)
			{
				Loading();
				Read_DFA_asFile();
				Check_String();
			}
			if (select == 2)
			{
				Init_DFA_Manual();
			}
			if (select == 3)
				about();
			if (select == 4)
				//exit(0);
				exit_menu();
		}
	}
}

int main()
{
	menu();
	return 0;
}
