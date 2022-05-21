#include <conio.h>
#include <Windows.h>
#include <iostream>

const int N = 6;
const int M = 15;
const int MAX_STRING_LENGTH = 20;

struct Student
{
	char surname[MAX_STRING_LENGTH];
	int group;
	int mark;
};

bool IsStringCharInvalid(char c)
{
	return !(c >= 65 && c <= 90) && // A-Z
		!(c >= 97 && c <= 122) && // a-z
		c != '\r' && c != '\b'; // enter + backspace
}

void HandleStringInput(char* str)
{
	char c = ' ';
	int i = 0;

	while (1)
	{
		do
		{
			c = _getch();
		} while (IsStringCharInvalid(c));

		switch (c)
		{
		case '\r':
			if (i != 0)
			{
				str[i] = '\0';
				return;
			}
			continue;
		case '\b':
			if (i != 0)
			{
				std::cout << "\b \b";
				i--;
			}
			continue;
		default:
			if (i < MAX_STRING_LENGTH)
			{
				str[i] = c;
				i++;
				std::cout << c;
			}
		}
	}
}

int GetIntFromConsole()
{
	int value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "������ ������������ ������. ������� �����: ";
	}
	return value;
}

int GetIntFromConsoleWithBounds(int left, int right)
{
	int value = left - 1;
	while (!(std::cin >> value) || !(value <= right && value >= left)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		if (!(value <= right && value >= left))
		{
			std::cout << "��������� ����� �� ������ � ��������. ������� �����: ";
		}
		else
		{
			std::cout << "������ ������������ ������. ������� �����: ";
		}
	}
	return value;
}

void AddToHashTable(Student* hash_table, Student val)
{
	int pos = val.mark % M;
	int try_count = 1;
	if (hash_table[pos].mark != -1)
	{
		while (hash_table[pos].mark != -1)
		{
			pos = (pos + try_count * try_count) % M;
			try_count++;
		}
	}
	hash_table[pos] = val;
}

int HashTableFind(Student* hash_table, int mark)
{
	int pos = mark % M;
	int try_count = 1;

	while (hash_table[pos].mark != -1)
	{
		if (hash_table[pos].mark == mark)
		{
			return pos;
		}
		pos = (pos + try_count * try_count) % M;
		try_count++;
	}
	return -1;
}

Student GetStudentInfoFromConsole()
{
	Student student;
	std::cout << "������� �������: ";
	HandleStringInput(student.surname);
	std::cout << std::endl << "������� ����� ������ (1 - 20): ";
	student.group = GetIntFromConsoleWithBounds(1, 20);
	std::cout << "������� ������ �������� (1 - 10): ";
	student.mark = GetIntFromConsoleWithBounds(1, 10);

	return student;
}

void PrintStudent(Student* student)
{
	std::cout << "������� " << student->surname << ": "
		<< student->group << "��., ������ " << student->mark << std::endl;
}

void InitStudentsArray(Student* arr)
{
	arr[0] = Student{"Ivanov", 9, 8};
	arr[1] = Student{"Markov", 4, 4};
	arr[2] = Student{"Konev", 1, 9};
	arr[3] = Student{"Serov", 11, 5};
	arr[4] = Student{"Belov", 9, 7};
	arr[5] = Student{"Chernov", 4, 6};
}

void InitDeaultHashTable(Student* table)
{
	int i;
	for(i = 0; i < M; i++)
	{
		table[i].mark = -1;
	}
}

void PrintHashTable(Student* table)
{
	int i;
	for (i = 0; i < M; i++)
	{
		std::cout << "table[" << i << "]" << " : ";
		if (table[i].mark != -1)
		{
			PrintStudent(&table[i]);
		} else
		{
			std::cout << "-1" << std::endl;
		}
	}
}

void ShowMenu()
{
	int selected_index = -1;
	Student students[N];
	Student hash_table[M];
	int search_mark = 0;
	int search_index = 0;

	InitStudentsArray(students);

	do
	{
		system("cls");
		std::cout << "����: \n";
		std::cout << "1. ������ ����� ������ ����������.\n";
		std::cout << "2. ������� ������ �� �����.\n";
		std::cout << "3. ��������� ���-�������.\n";
		std::cout << "4. ������� ���-������� �� �����.\n";
		std::cout << "5. ����� �� ���-�������.\n";
		std::cout << "0. �����.\n";
		std::cout << "�������� �����: ";

		selected_index = GetIntFromConsole();

		switch (selected_index)
		{
		case 1:
			for (int i = 0; i < N; i++)
			{
				students[i] = GetStudentInfoFromConsole();
			}
			continue;
		case 2:
			for (int i = 0; i < N; i++)
			{
				PrintStudent(&students[i]);
			}
			system("pause");
			continue;
		case 3:
			InitDeaultHashTable(hash_table);
			for (int i = 0; i < N; i++)
			{
				AddToHashTable(hash_table, students[i]);
			}
			continue;
		case 4:
			PrintHashTable(hash_table);
			system("pause");
			continue;
		case 5:
			std::cout << "������� ������� ����: ";
			search_mark = GetIntFromConsoleWithBounds(1, 10);
			search_index = HashTableFind(hash_table, search_mark);
			if (search_index != -1)
			{
				std::cout << "������� � ������� " << search_mark << " ������ �� ������� " << search_index << std::endl;
				PrintStudent(&hash_table[search_index]);
			}
			else
			{
				std::cout << "������� � ������� " << search_mark << " �� ��� ������." << std::endl;
			}
			system("pause");
			continue;
		}
	} while (selected_index != 0);
}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ShowMenu();

	return;
}