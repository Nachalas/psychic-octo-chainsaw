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

void StraightTreeTraversal(TreeNode* tree)
{
	if (tree != NULL)
	{
		std::cout << "(" << tree->info.key << ", \'" << tree->info.str << "\')" << std::endl;
		StraightTreeTraversal(tree->left);
		StraightTreeTraversal(tree->right);
	}
}

void ReverseTreeTraversal(TreeNode* tree)
{
	if (tree != NULL)
	{
		StraightTreeTraversal(tree->left);
		StraightTreeTraversal(tree->right);
		std::cout << "(" << tree->info.key << ", \'" << tree->info.str << "\')" << std::endl;
	}
}

void KeyIncreaseTreeTraversal(TreeNode* tree)
{
	if(tree != NULL)
	{
		KeyIncreaseTreeTraversal(tree->left);
		std::cout << "(" << tree->info.key << ", \'" << tree->info.str << "\')" << std::endl;
		KeyIncreaseTreeTraversal(tree->right);
	}
}

void RemoveTreeElement(TreeNode* root, int key) {
	TreeNode* Del, *Prev_Del, *R, *Prev_R;

	Del = root;
	Prev_Del = NULL;

	while (Del != NULL && Del->info.key != key) {
		Prev_Del = Del;
		if (Del->info.key > key)
		{
			Del = Del->left;
		}
		else
		{
			Del = Del->right;
		}
	}

	if (Del == NULL) {
		std::cout << "Элемент с указанным ключом не был найден" << std::endl;
		return;
	}
	
	if (Del->right == NULL) { R = Del->left; }
	else {
		if (Del->left == NULL) { R = Del->right; }
		else {
			Prev_R = Del;
			R = Del->left;
			while (R->right != NULL) {
				Prev_R = R;
				R = R->right;
			}

			if (Prev_R == Del) R->right = Del->right;
			else {
				R->right = Del->right;
				Prev_R->right = R->left;
				R->left = Prev_R;
			}
		}
	}
	if (Del == root)
	{
		root = R;
	}
	else
	{
		if (Del->info.key < Prev_Del->info.key)
		{
			Prev_Del->left = R;
		}
		else
		{
			Prev_Del->right = R;
		}
	}
		
	delete Del;
	std::cout << "Элемент успешно удалён" << std::endl;
}

void GetTreeElemCount(TreeNode* tree, int& count)
{
	if (tree != NULL)
	{
		count++;
		GetTreeElemCount(tree->left, count);
		GetTreeElemCount(tree->right, count);
	}
}

void CopyTreeToArray(TreeNode* tree, Info* arr, int& index)
{
	if (tree != NULL)
	{
		arr[index] = tree->info;
		index++;
		CopyTreeToArray(tree->left, arr, index);
		CopyTreeToArray(tree->right, arr, index);
	}
}

// Сортировка пузырьком
void SortInfoArray(Info* array, int size)
{
	int i, j;
	int temp_key;
	for(i = 0; i < size - 1; i++)
	{
		for(j = 0; j < size - i - 1; j++)
		{
			if(array[j].key > array[j + 1].key)
			{
				temp_key = array[j].key;
				array[j].key = array[j + 1].key;
				array[j + 1].key = temp_key;
			}
		}
	}
}

void Make_Blns(TreeNode** p, int n, int k, Info* info_array) {
	if (n == k) {
		*p = NULL;
		return;
	}
	else {
		int m = (n + k) / 2;
		*p = new TreeNode();
		(*p)->info.key = info_array[m].key;
		strcpy_s((*p)->info.str, MAX_STRING_LENGTH, info_array[m].str);
		Make_Blns(&(*p)->left, n, m, info_array);
		Make_Blns(&(*p)->right, m + 1, k, info_array);
	}
}


TreeNode* BalanceTree(TreeNode* tree)
{
	int tree_elem_count = 0;
	int info_array_index = 0;
	GetTreeElemCount(tree, tree_elem_count);
	Info* info_array = new Info[tree_elem_count];
	CopyTreeToArray(tree, info_array, info_array_index);
	SortInfoArray(info_array, tree_elem_count);
	FreeTree(tree);
	Make_Blns(&tree, 0, tree_elem_count, info_array);
	return tree;
}

void ShowTraversalOptionsMenu(TreeNode* tree)
{
	int selected_option = -1;
	do
	{
		system("cls");
		std::cout << "Меню выбора способа обхода дерева: \n";
		std::cout << "1. Прямой обход (root => left => right).\n";
		std::cout << "2. Обратный обход (left => right => root).\n";
		std::cout << "3. В порядке возрастания ключа (left => root => right).\n";
		std::cout << "0. Выйти.\n";
		std::cout << "Выберите пункт: ";

		selected_option = GetIntFromConsole();

		switch (selected_option)
		{
		case 1:
			if(!IsTreeEmpty(tree))
			{
				PrintTree(tree, 0);
				StraightTreeTraversal(tree);
			} else
			{
				std::cout << "Дерево не имеет элементов." << std::endl;
			}
			system("pause");
			continue;
		case 2:
			if (!IsTreeEmpty(tree))
			{
				PrintTree(tree, 0);
				ReverseTreeTraversal(tree);
			} else
			{
				std::cout << "Дерево не имеет элементов." << std::endl;
			}
			system("pause");
			continue;
		case 3:
			if (!IsTreeEmpty(tree))
			{
				PrintTree(tree, 0);
				KeyIncreaseTreeTraversal(tree);
			} else
			{
				std::cout << "Дерево не имеет элементов." << std::endl;
			}
			system("pause");
			continue;
		}
	} while (selected_option != 0);
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
	int key_to_delete = -1;

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
		std::cout << "7. Обходы дерева.\n";
		std::cout << "8. Удалить элемент из дерева.\n";
		std::cout << "9. Сбалансировать дерево.\n";
		std::cout << "0. Выйти.\n";
		std::cout << "Выберите пункт: ";

		selected_index = GetIntFromConsole();

		switch (selected_index)
		{
		case 1:
			if (info_array != NULL)
			{
				FreeInfoArray(info_array);
			}
			std::cout << "Введите количество добавляемой информации: ";
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
		case 7:
			ShowTraversalOptionsMenu(tree);
			continue;
		case 8:
			std::cout << "Введите ключ элемента для удаления: " << std::endl;
			key_to_delete = GetIntFromConsoleWithBounds(0, 10000);
			RemoveTreeElement(tree, key_to_delete);
			system("pause");
			continue;
		case 9:
			tree = BalanceTree(tree);
			system("pause");
			continue;
		}
	} while (selected_index != 0);
	FreeTree(tree);
	if (info_array != NULL)
	{
		FreeInfoArray(info_array);
	}
}

void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ShowMenu();

	return;
}