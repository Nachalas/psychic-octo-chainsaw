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
Программа предоставляет сведения о товарах, имеющихся на складе:
- наименование товара,
- количество единиц товара,
- цена единицы товара,
- дата поступления товара на склад,
- ФИО зарегистрировавшего товар
Индивидуальное задание:
вывести в алфавитном порядке список товаров,
хранящихся более x месяцев,
стоимость которых превышает y рублей (x, y вводятся с клавиатуры).
Общее для всех вариантов задание:
реализовать авторизацию для входа в систему,
функционал администратора
и функционал пользователя
(см. более подробно в функциональных требованиях к курсовой работе,
подраздел 1.2).
 */

// Последовательно показывает поля формы для индивидуального задания
void PerformTask()
{
	system("cls");
	if (!products.empty())
	{
		std::cout << "=== Индивидуальное задание ===" << std::endl;
		std::cout << "Вывести в алфавитном порядке список товаров: " << std::endl;
		std::cout << "- Хранящихся более x месяцев" << std::endl;
		std::cout << "- Стоимость которых превышает y рублей" << std::endl;
		std::cout << "Введите x: ";
		int months = GetIntFromConsole();
		std::cout << "Введите y: ";
		double price = GetDoubleFromConsole();
		PrintProductsTable(GetProductsByDurationAndPrice(months, price));
	}
	else
	{
		std::cout << "Таблица товаров на данный момент пуста." << std::endl;
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
				std::cout << "Введены неправильные данные." << std::endl;
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
				std::cout << "Введены неправильные данные." << std::endl;
				system("pause");
			}
			system("cls");
			break;
		default:
			system("cls");
			SaveUsersToFile();
			SaveProductsToFile();
			std::cout << "Работа программы завершена." << std::endl;
		}
	} while (selected_option != 3);

	return 0;
}