//Coded by: Ashkan.rmk
//Theory of Languages and Machines Project
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <vector>
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

struct node_DPDA
{
	int *transition;
	char *pos_trans;
	char *pop_stack;
	string *push_stack;
	int num_state;
	int transit_num;
};

//Customized stack for DPDA
class Stack
{
private:
	int top;
	char *stack;
	int MaxSize;
public:
	Stack(int MaxStackSize)
	{
		MaxSize = MaxStackSize;
		stack = new char[MaxSize];
		stack[0] = '$';
		top = 0;
	}
	void Push(const char& item)
	{
		stack[++top] = item;
	}
	void Pop()
	{
		top--;
	}
	char* Show_top(char& item)
	{
		item = stack[top];
		return &item;
	}
	void contain()
	{
		int f = top;
		cout << "Stack: ";
		while (f > -1)
		{
			cout << stack[f--] << " ";
		}
		cout << endl;
	}
	bool is_Empty()
	{
		if (stack[top] == '$')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	void make_null()
	{
		int f = top;
		while (f > -1)
		{
			stack[f--] = ' ';
		}
		stack[0] = '$';
		top = 0;
	}
};

node *state;
node_DPDA *state_dpa;
int total_state_num;
int alphabet_num;
char *possible_transit;
int acc_state_num;
int *final_accept_state;
int start_state_num;
void menu();

///////DFA Functions
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
	bool flag;	//flag for go to next state
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
		flag = true;
		for (int i = 0; input[i] && goal; i++)
		{
			Sleep(1000);
			gotoxy(count, 6);
			cout << "^";
			gotoxy(count - 1, 6);
			cout << " ";
			count++;
			gotoxy(0, c++);
			flag = true;
			for (int j = 0; j < total_state_num && flag; j++)
			{
				flag = true;
				if (state[j].num_state == START)
				{
					for (int k = 0; k < state[j].transit_num; k++)
					{
						if (input[i] == possible_transit[k])
						{
							cout << "q" << START << "  --" << input[i] << "->  " << "q" << state[j].transition[k] << endl;
							START = state[j].transition[k];
							flag = false;	//Jump to next char in String
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
				cout << "Accept!\n\n" << endl;
				check = false;	//if state accepted
				system("pause>0");
			}
		}

		if (check)
		{
			color(4);
			gotoxy(40, 9);
			cout << "Reject!\n\n" << endl;
			system("pause>0");
		}

		gotoxy(30, 14);
		cout << "AGAIN?";
		gotoxy(25, 16);
		cout << "1) YES";
		gotoxy(25, 17);
		cout << "2) NO\n";
		gotoxy(25, 19);
		cout << "-> ";
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
//////////////////////////////////

///////DPDA Functions
void Init_DPDA_Manual()
{
	system("cls");

	fstream output;
	output.open("DPDA.txt", ios::out);	//Make File
	if (!output)
	{
		cerr << "File can not open." << endl;
		exit(1);
	}

	cout << "\n\n\tEnter MAXIMUM DPDA States:" << endl;
	cout << "\t-> ";
	cin >> total_state_num;
	output << total_state_num << endl;
	state_dpa = new node_DPDA[total_state_num];

	color(11);
	cout << "\t\tAttentions: L stands for LAMBDA alphabet\n\t\t\tEmpty Stack has $\n\n";
	color(15);
	cout << "\n\n\tEnter MAXIMUM DPDA alphabet numbers(include LAMBDA = L):" << endl;
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
	cout << "\n\n\t\t\t* Now Initial DPDA States *\n\n";
	color(15);

	bool ch = true;
	int index = 0;
	int count = 0;

	for (int i = 0; i < total_state_num; i++)
	{
		cout << "\n\t------------\n";
		cout << "\n\tEnter " << i + 1 << "th state number: ";
		cin >> state_dpa[i].num_state;
		output << state_dpa[i].num_state << endl;
		state_dpa[i].transition = new int[50];
		state_dpa[i].pop_stack = new char[50];
		state_dpa[i].push_stack = new string[50];
		state_dpa[i].pos_trans = new char[alphabet_num];
		index = 0;
		count = 0;
		ch = true;

		for (int j = 0; j < alphabet_num; j++)
		{
			cout << "\n\n\n\tDoes state " << state_dpa[i].num_state << " have transition with lable " << possible_transit[j] << "? ";
			cout << "\n\t1) YES";
			cout << "\n\t2) NO\n";
			cout << "\t-> ";
			int ch1;
			bool c = true;
			cin >> ch1;
			if (ch1 == 1)
			{
				c = true;
				ch = true;
				output << '@' << endl;
			}
			else
			{
				c = false;
				output << '#' << endl;
			}

			while (ch && c)
			{
				system("cls");
				color(11);
				cout << "\n\tState: " << state_dpa[i].num_state;
				color(15);
				cout << "\n\tWhich state will go with transition " << possible_transit[j] << "? ";

				state_dpa[i].pos_trans[count] = possible_transit[j];

				cin >> state_dpa[i].transition[index];
				output << state_dpa[i].transition[index] << endl;

				cout << "\n\tWhat to POP from Stack? (Just one element!) ";
				cin >> state_dpa[i].pop_stack[index];
				while (state_dpa[i].pop_stack[index] == 'L')
				{
					cout << "\n\tDo NOT Enter LAMBDA (L)!\n\tAgain: ";
					cin >> state_dpa[i].pop_stack[index];
				}
				output << state_dpa[i].pop_stack[index] << endl;

				cout << "\n\tWhat to PUSH in Stack? ";
				cin >> state_dpa[i].push_stack[index];
				output << state_dpa[i].push_stack[index] << endl;

				count++;

				gotoxy(30, 14);
				cout << "MORE TANSITION WITH " << possible_transit[j] << " ?";
				gotoxy(25, 16);
				cout << "1) YES";
				gotoxy(25, 17);
				cout << "2) NO\n";
				gotoxy(25, 19);
				cout << "-> ";
				int ch2;
				cin >> ch2;
				if (ch2 == 1)
				{
					ch = true;
					index++;
					system("cls");
					output << '@' << endl;
				}
				else
				{
					ch = false;
					output << '#' << endl;
					index++;
					system("cls");
				}
			}
		}
		state_dpa[i].transit_num = count;
	}

	system("cls");
	cout << "\n\n\tEnter Start State Number: " << endl;
	cout << "\t-> ";
	cin >> start_state_num;
	output << start_state_num << endl;

	system("cls");
	output.close();	//Close File
	color(11);
	cout << "\n\n\n\t\t\tYour DPDA completely Initialized!" << endl;
	color(15);
	_getch();
	system("cls");
}

void Read_DPDA_asFile()
{
	ifstream inputFile("DPDA.txt", ios::in | ios::beg);
	if (!inputFile)	//check for file opening
	{
		cerr << "some thing wrong during opening file!" << endl;
		_getche();
		exit(1);
	}

	inputFile >> total_state_num;
	state_dpa = new node_DPDA[total_state_num];

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

	int index, count;
	char tmp;
	for (int i = 0; i < total_state_num; i++)
	{
		inputFile >> state_dpa[i].num_state;
		state_dpa[i].transition = new int[50];
		state_dpa[i].pop_stack = new char[50];
		state_dpa[i].push_stack = new string[50];
		state_dpa[i].pos_trans = new char[alphabet_num];
		index = 0;
		count = 0;

		for (int j = 0; j < alphabet_num; j++)
		{
			while (1)
			{
				inputFile >> tmp;
				if (tmp == '#')
				{
					break;
				}
				else
				{
					state_dpa[i].pos_trans[index] = possible_transit[j];

					inputFile >> state_dpa[i].transition[index];

					inputFile >> state_dpa[i].pop_stack[index];

					inputFile >> state_dpa[i].push_stack[index];

					count++;

					index++;
				}
			}
		}
		state_dpa[i].transit_num = count;
	}

	inputFile >> start_state_num;

	color(11);
	cout << "\n\n\n\t\t\tYour DPDA has been Set!" << endl;
	color(15);
	_getch();
	system("cls");
}

void Check_String_DPDA()
{
	string input;
	Stack stack(100);
	bool goal;	//checking foreign alphabet
	bool check;	//checking accept state
	bool flag;	//flag for go to next state
	bool CON = true;
	int START;		//save Start State Number
	int lng;		//Check number of possible alphabets
	bool x = false;

	while (CON)
	{
		goal = false;
		x = false;
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
			for (int j = 0; j < alphabet_num; j++)		//possible_transit[j]
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
		flag = true;
		bool m = false;
		bool r = true;
		bool l_sib = false;
		for (int i = 0; i < input.length() + 1 && goal && r; i++)
		{
			Sleep(1000);
			gotoxy(count, 6);
			cout << "^";
			gotoxy(count - 1, 6);
			cout << " ";
			count++;
			gotoxy(0, c++);
			flag = true;
			for (int j = 0; j < total_state_num && flag; j++)
			{
				for (int q = 0; q < state_dpa[j].transit_num; q++)
				{
					if (state_dpa[j].pos_trans[q] == 'L' &&  input[i] == NULL)
					{
						m = true;	//has LAMBDA transition and string finished
					}
					else if (state_dpa[j].pos_trans[q] == 'L' && input[i+1] != NULL)
					{
						l_sib = true;	//has LAMBDA transition and string NOT finished!
					}
				}
				flag = true;
				if (state_dpa[j].num_state == START)
				{
					for (int k = 0; k < state_dpa[j].transit_num && flag; k++)
					{
						if (input[i] == state_dpa[j].pos_trans[k] || m || l_sib)
						{
							char tmp = state_dpa[j].pop_stack[k];
							char tmp1;
							stack.Show_top(tmp1);
							if (tmp == tmp1)
							{
								stack.Pop();
								string s = state_dpa[j].push_stack[k];
								for (int x = s.length(); x > 0; x--)
								{
									if (s[x - 1] != 'L')
									{
										stack.Push(s[x - 1]);
									}
								}
								cout << "q" << START << "  --" << input[i] << "->  " << "q" << state_dpa[j].transition[k] << " ";
								stack.contain();
								cout << endl;
								START = state_dpa[j].transition[k];
								flag = false;	//Jump to next char in String
								m = false;
								x = true;	//be in possible transtion of state
								r = true;
								break;
							}
						}
						else
						{
							r = false;	//stop checking string and reject!
						}
					}
				}
			}
		}



		for (int i = 0; i < acc_state_num; i++)
		{
			if (final_accept_state[i] == START && goal && stack.is_Empty() && x && r)
			{
				color(3);
				gotoxy(40, 9);
				cout << "Accept!\n\n" << endl;
				check = false;	//if state accepted
				system("pause>0");
			}
		}

		if (check)
		{
			color(4);
			gotoxy(40, 9);
			cout << "Reject!\n\n" << endl;
			system("pause>0");
		}

		gotoxy(30, 14);
		cout << "AGAIN?";
		gotoxy(25, 16);
		cout << "1) YES";
		gotoxy(25, 17);
		cout << "2) NO\n";
		gotoxy(25, 19);
		cout << "-> ";
		int ch;
		cin >> ch;
		if (ch == 1)
		{
			CON = true;
			stack.make_null();
		}
		else
		{
			CON = false;
			stack.make_null();
		}
	}
}
//////////////////////////////////

void about()
{
	system("cls");
	gotoxy(27, 10);
	cout << "Automata Machines Simulator v 1.5";
	gotoxy(27, 12);
	cout << "Coded By Ashkan.rmk";
	gotoxy(27, 14);
	cout << "All Rights Resereved!";
	gotoxy(27, 16);
	cout << ":D  ";
	_getch();
	system("cls");
}

void coming()
{
	system("cls");
	gotoxy(27, 10);
	cout << "coming soon!  :D";
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
	gotoxy(48, 5);
	cout << ":) Your Machine in progress...";
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

void DFA_menu()
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
		cout << "Back";

		ch = _getch();
		switch (ch)
		{
		case 72:	//Up
			select--;
			if (select == 0)
				select = 3;
			break;
		case 80:	//Down
			select++;
			if (select == 4)
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
				menu();
		}
	}
}

void DPDA_menu()
{
	system("cls");
	char ch;
	int select = 1;

	while (1)
	{
		system("cls");
		color(10);
		gotoxy(33, 6);
		cout << "DPDA Simulator";

		if (select == 1)
			color(11);
		else
			color(15);

		gotoxy(28, 10);
		cout << "Check Strings with Saved DPDA";

		if (select == 2)
			color(11);
		else
			color(15);

		gotoxy(28, 12);
		cout << "Input New DPDA and Save it";

		if (select == 3)
			color(11);
		else
			color(15);

		gotoxy(28, 14);
		cout << "Back";

		ch = _getch();
		switch (ch)
		{
		case 72:	//Up
			select--;
			if (select == 0)
				select = 3;
			break;
		case 80:	//Down
			select++;
			if (select == 4)
				select = 1;
			break;
		case 13:	//Enter
			if (select == 1)
			{
				Loading();
				Read_DPDA_asFile();
				Check_String_DPDA();
			}
			if (select == 2)
			{
				Init_DPDA_Manual();
			}
			if (select == 3)
				menu();
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
		cout << "Automatas  Machine Simulator";

		if (select == 1)
			color(11);
		else
			color(15);

		gotoxy(28, 10);
		cout << "Deterministic finite automaton (DFA)";

		if (select == 2)
			color(11);
		else
			color(15);

		gotoxy(28, 12);
		cout << "Deterministic pushdown automaton (DPDA)";

		if (select == 3)
			color(11);
		else
			color(15);

		gotoxy(28, 14);
		cout << "Turing machine";

		if (select == 4)
			color(11);
		else
			color(15);

		gotoxy(28, 16);
		cout << "About";

		if (select == 5)
			color(11);
		else
			color(15);

		gotoxy(28, 18);
		cout << "Exit";


		ch = _getch();
		switch (ch)
		{
		case 72:	//Up
			select--;
			if (select == 0)
				select = 5;
			break;
		case 80:	//Down
			select++;
			if (select == 6)
				select = 1;
			break;
		case 13:	//Enter
			if (select == 1)
			{
				DFA_menu();
			}
			if (select == 2)
			{
				DPDA_menu();
			}
			if (select == 3)
				coming();
			if (select == 4)
				about();
			if (select == 5)
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