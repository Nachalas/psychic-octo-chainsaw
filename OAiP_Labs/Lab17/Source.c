#include <Windows.h>
#include <iostream>

const int LOWER_BOUND = 23000;
const int UPPER_BOUND = 45000;
const int N = 12;
const int M = 15;

int GetIntFromConsole()
{
	int value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(32767, '\n');
		std::cout << "Введен неправильный символ. Введите число: ";
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
			std::cout << "Введенное число не входит в диапазон. Введите число: ";
		}
		else
		{
			std::cout << "Введен неправильный символ. Введите число: ";
		}
	}
	return value;
}

void AddToHashTable(int* hash_table, int val)
{
	int pos = val % M;
	if(hash_table[pos] != -1)
	{
		while(hash_table[pos] != -1)
		{
			pos++;
			if(pos == M)
			{
				pos = 0;
			}
		}
	}
	hash_table[pos] = val;
}

int HashTableFind(int* hash_table, int val)
{
	int pos = val % M;
	while (hash_table[pos] != -1)
	{
		if(hash_table[pos] == val)
		{
			return pos;
		}
		pos++;
		if(pos == M)
		{
			pos = 0;
		}
	}
	return -1;
}

void SetHashTableToDefaultValues(int* table)
{
	int i = 0;
	for(i = 0; i < M; i++)
	{
		table[i] = -1;
	}
}

void PrintHashTable(int* hash_table)
{
	int i;
	for(i = 0; i < M; i++)
	{
		std::cout << "table[" << i << "]" << " : " << hash_table[i] << std::endl;
	}
}

void ShowMenu()
{
	int selected_index = -1;
	int number_array[N] = {
		23000, 24000, 25000, 26000, 27000, 28000, 28500, 28999, 30154, 35489, 23648, 43058
	};
	int hash_table[M];
	SetHashTableToDefaultValues(hash_table);
	int search_elem = 0;
	int search_elem_index = 0;

	do
	{
		system("cls");
		std::cout << "Меню: \n";
		std::cout << "1. Ввести новый массив информации.\n";
		std::cout << "2. Вывести массив на экран.\n";
		std::cout << "3. Заполнить хеш-таблицу.\n";
		std::cout << "4. Вывести хеш-таблицу на экран.\n";
		std::cout << "5. Поиск по хеш-таблице.\n";
		std::cout << "0. Выйти.\n";
		std::cout << "Выберите пункт: ";

		selected_index = GetIntFromConsole();

		switch (selected_index)
		{
		case 1:
			for (int i = 0; i < N; i++)
			{
				std::cout << "Введите элемент " << i + 1 << "(" << LOWER_BOUND << " - " << UPPER_BOUND << "): ";
				number_array[i] = GetIntFromConsoleWithBounds(LOWER_BOUND, UPPER_BOUND);
			}
			continue;
		case 2:
			for (int i = 0; i < N; i++)
			{
				std::cout << number_array[i] << " ";
			}
			std::cout << std::endl;
			system("pause");
			continue;
		case 3:
			SetHashTableToDefaultValues(hash_table);
			for(int i = 0; i < N; i++)
			{
				AddToHashTable(hash_table, number_array[i]);
			}
			continue;
		case 4:
			PrintHashTable(hash_table);
			system("pause");
			continue;
		case 5:
			std::cout << "Введите элемент, который нужно найти: ";
			search_elem = GetIntFromConsoleWithBounds(LOWER_BOUND, UPPER_BOUND);
			search_elem_index = HashTableFind(hash_table, search_elem);
			if (search_elem_index != -1)
			{
				std::cout << "Элемент " << search_elem << " найден по индексу " << search_elem_index << std::endl;
			} else
			{
				std::cout << "Элемент " << search_elem << " не был найден." << std::endl;
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