#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

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

// Функция выводит в консоль меню с вариантами авторизации,
// возвращает выбранный пользователем пункт
int ShowAuthMenu()
{
	int selected_option = 0;
	do
	{
		system("cls");
		std::cout << "Меню авторизации:\n";
		std::cout << "1. Войти в качестве администратора.\n";
		std::cout << "2. Войти в качестве пользователя.\n";
		std::cout << "3. Выйти.\n";
		std::cout << "Выберите пункт: ";
		selected_option = GetIntFromConsole();
	} while (selected_option < 1 || selected_option > 3);
	return selected_option;
}

// TODO: comment
void ShowAuthForm()
{
	system("cls");

	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor_position = {0,1};

	std::cout << "Пожалуйста, введите данные:" << std::endl;
	std::cout << "_____________" << std::endl;
	std::cout << "_____________" << std::endl;
	
	SetConsoleCursorPosition(console_handle, cursor_position);


}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int selected_option = ShowAuthMenu();

	ShowAuthForm();

	return 0;
}