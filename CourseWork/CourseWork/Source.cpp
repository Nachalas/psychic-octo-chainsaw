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
		std::cout << "������ ������������ ������. ������� �����: ";
	}
	return value;
}

// ������� ������� � ������� ���� � ���������� �����������,
// ���������� ��������� ������������� �����
int ShowAuthMenu()
{
	int selected_option = 0;
	do
	{
		system("cls");
		std::cout << "���� �����������:\n";
		std::cout << "1. ����� � �������� ��������������.\n";
		std::cout << "2. ����� � �������� ������������.\n";
		std::cout << "3. �����.\n";
		std::cout << "�������� �����: ";
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

	std::cout << "����������, ������� ������:" << std::endl;
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