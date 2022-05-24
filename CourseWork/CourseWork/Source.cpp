#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <tuple>

#include "TableUtils.h"
#include "ConsoleUtils.h"
#include "Menu.h"
#include "VectorUtils.h"
#include "TimeUtils.h"
#include "FileUtils.h"
#include "Model.h"
#include "Auth.h"
#include "Source.h"

#include "Global.h"

std::vector<UserInfo> users;
std::vector<ProductEntry> products;
UserInfo current_user;

/*
��������� ������������� �������� � �������, ��������� �� ������:
- ������������ ������,
- ���������� ������ ������,
- ���� ������� ������,
- ���� ����������� ������ �� �����,
- ��� ������������������� �����
�������������� �������:
������� � ���������� ������� ������ �������,
���������� ����� x �������,
��������� ������� ��������� y ������ (x, y �������� � ����������).
����� ��� ���� ��������� �������:
����������� ����������� ��� ����� � �������,
���������� ��������������
� ���������� ������������
(��. ����� �������� � �������������� ����������� � �������� ������,
��������� 1.2).
 */

// ��������������� ���������� ���� ����� ��� ��������������� �������
void PerformTask()
{
	system("cls");
	if (!products.empty())
	{
		std::cout << "=== �������������� ������� ===" << std::endl;
		std::cout << "������� � ���������� ������� ������ �������: " << std::endl;
		std::cout << "- ���������� ����� x �������" << std::endl;
		std::cout << "- ��������� ������� ��������� y ������" << std::endl;
		std::cout << "������� x: ";
		int months = GetIntFromConsole();
		std::cout << "������� y: ";
		double price = GetDoubleFromConsole();
		PrintProductsTable(GetProductsByDurationAndPrice(months, price));
	}
	else
	{
		std::cout << "������� ������� �� ������ ������ �����." << std::endl;
		system("pause");
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int selected_option = 0;

	ReadUsersFromFile();
	ReadProductsFromFile();

	do
	{
		selected_option = ShowAuthMenu();
		AuthInfo form_info;
		switch (selected_option)
		{
		case 1:
			form_info = ShowAuthForm();
			system("cls");
			if (CheckAdminCredentialsAndLogIn(form_info))
			{
				ShowAdminMenu();
			}
			else
			{
				std::cout << "������� ������������ ������." << std::endl;
				system("pause");
			}
			break;
		case 2:
			form_info = ShowAuthForm();
			system("cls");
			if(CheckUserCredentialsAndLogIn(form_info))
			{
				ShowUserMenu();
			} else
			{
				std::cout << "������� ������������ ������." << std::endl;
				system("pause");
			}
			system("cls");
			break;
		default:
			system("cls");
			SaveUsersToFile();
			SaveProductsToFile();
			std::cout << "������ ��������� ���������." << std::endl;
		}
	} while (selected_option != 3);

	return 0;
}