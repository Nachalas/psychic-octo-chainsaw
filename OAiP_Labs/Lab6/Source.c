#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

const int MAX_STRING_LENGTH = 20;

bool IsStringCharInvalid(char c)
{
	return !(c >= 65 && c <= 90) && // A-Z
		!(c >= 97 && c <= 122) && // a-z
		!(c >= 48 && c <= 57) && // 0-9
		c != '\r' && c != '\b'; // enter + backspace
}

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

struct Info
{
	int key;
	char str[MAX_STRING_LENGTH];
};

struct TreeNode
{
	Info info;
	TreeNode* left;
	TreeNode* right;
};

void AddTreeElement(TreeNode* tree, Info info)
{
	if(tree->info.key == -1)
	{
		tree->info = info;
	} else
	{
		bool spot_found = true;
		TreeNode* previous_node = nullptr;
		TreeNode* t = tree;
		while (t != NULL && spot_found)
		{
			previous_node = t;
			if (info.key == t->info.key)
			{
				spot_found = false;
				std::cout << std::endl << "Найден дубликат ключа" << std::endl;
			}
			else
			{
				if (info.key < t->info.key)
				{
					t = t->left;
				}
				else
				{
					t = t->right;
				}
			}
		}
		if (spot_found)
		{
			t = new TreeNode();
			t->info = info;
			t->left = t->right = NULL;
			if(info.key < previous_node->info.key)
			{
				previous_node->left = t;
			} else
			{
				previous_node->right = t;
			}
		}
	}
}

void PrintTree(TreeNode* tree, int level) {
	if (tree != NULL) {
		PrintTree(tree->right, level + 1); // Правое поддерево
		for (int i = 0; i < level; i++) std::cout << "  ";
		std::cout << "(" << tree->info.key << ":" << tree->info.str << ")" << std::endl;
		PrintTree(tree->left, level + 1); // Левое поддерево
	}
}

bool IsTreeEmpty(TreeNode* tree)
{
	return tree->info.key == -1;
}

void FreeTree(TreeNode* tree)
{
	if(tree != NULL)
	{
		FreeTree(tree->left);
		FreeTree(tree->right);
		delete tree;
	}
}

void FreeInfoArray(Info* arr)
{
	delete[] arr;
}
 
Info GetInfoFromConsole()
{
	Info info;
	std::cout << std::endl << "Введите ключ: ";
	info.key = GetIntFromConsoleWithBounds(0, 10000);
	std::cout << "Введите строковую информацию: ";
	HandleStringInput(info.str);
	return info;
}

Info* GetInfoByKey(TreeNode* tree, int key)
{
	Info* info = NULL;

	while(tree != NULL)
	{
		if (tree->info.key == key)
		{
			info = &tree->info;
			break;
		}
		else
		{
			if (key < tree->info.key)
			{
				tree = tree->left;
			}
			else
			{
				tree = tree->right;
			}
		}
	}

	return info;
}

TreeNode* FindMinKeyNode(TreeNode* tree)
{
	while (tree->left != NULL)
	{
		tree = tree->left;
	}
	return tree;
}

TreeNode* FindMaxKeyNode(TreeNode* tree)
{
	while (tree->right != NULL)
	{
		tree = tree->right;
	}
	return tree;
}

void ShowMenu()
{
	int selected_index = -1;
	TreeNode* tree = new TreeNode();
	tree->info.key = -1;
	Info* info_array = nullptr;
	int info_array_elem_count = 0;
	int search_key = 0;
	Info* search_info = nullptr;
	TreeNode* min_key_node = nullptr;
	TreeNode* max_key_node = nullptr;

	do
	{
		system("cls");
		std::cout << "Меню: \n";
		std::cout << "1. Ввести новый массив информации.\n";
		std::cout << "2. Внести информацию из массива в дерево поиска.\n";
		std::cout << "3. Вывести дерево в консоль.\n";
		std::cout << "4. Добавить в дерево новую запись.\n";
		std::cout << "5. Поиск по ключу.\n";
		std::cout << "6. Индивидуальное задание.\n";
		std::cout << "0. Выйти.\n";
		std::cout << "Выберите пункт: ";

		selected_index = GetIntFromConsole();

		switch (selected_index)
		{
		case 1:
			std::cout << "Введите количество добавляемой информации: ";
			if (info_array != NULL)
			{
				FreeInfoArray(info_array);
			}
			info_array_elem_count = GetIntFromConsole();
			info_array = new Info[info_array_elem_count];
			for(int i = 0; i < info_array_elem_count; i++)
			{
				info_array[i] = GetInfoFromConsole();
			}
			continue;
		case 2:
			if(info_array != NULL)
			{
				FreeTree(tree);
				tree = new TreeNode();
				tree->info.key = -1;
				for(int i = 0; i < info_array_elem_count; i++)
				{
					AddTreeElement(tree, info_array[i]);
				}
				std::cout << "Данные успешно внесены." << std::endl;
			} else 
			{
				std::cout << "Не был введен массив данных" << std::endl;
			}
			system("pause");
			continue;
		case 3:
			if (!IsTreeEmpty(tree))
			{
				PrintTree(tree, 0);
			} else
			{
				std::cout << "Дерево не имеет элементов." << std::endl;
			}
			std::cout << std::endl;
			system("pause");
			continue;
		case 4:
		{
			Info info = GetInfoFromConsole();
			AddTreeElement(tree, info);
			std::cout << std::endl;
			system("pause");
			continue;
		}
		case 5:
			std::cout << "Введите искомый ключ: ";
			search_key = GetIntFromConsoleWithBounds(0, 10000);
			search_info = GetInfoByKey(tree, search_key);
			if(search_info != NULL)
			{
				std::cout << "Найден элемент: (" << search_info->key << ", \'" << search_info->str << "\')" << std::endl;
			} else
			{
				std::cout << "Элемент с указанным ключом не найден" << std::endl;
			}
			system("pause");
			continue;
		case 6:
			std::cout << "Задание: поменять информацию, содержащую максимальный и минимальный ключи" << std::endl;
			if(!IsTreeEmpty(tree))
			{
				min_key_node = FindMinKeyNode(tree);
				max_key_node = FindMaxKeyNode(tree);
				if(min_key_node != max_key_node)
				{
					char temp_str[20];
					temp_str[19] = '\0';
					strcpy_s(temp_str, MAX_STRING_LENGTH, min_key_node->info.str);
					strcpy_s(min_key_node->info.str, MAX_STRING_LENGTH, max_key_node->info.str);
					strcpy_s(max_key_node->info.str, MAX_STRING_LENGTH, temp_str);
				} else
				{
					std::cout << "Дерево имеет всего один элемент." << std::endl;
				}
			} else
			{
				std::cout << "Дерево не имеет элементов." << std::endl;
			}
			system("pause");
			continue;
		}
	} while (selected_index != 0);
	FreeTree(tree);
}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ShowMenu();

	return;
}