#include "Menu.h"

#include "Global.h"

void ShowUserMenu()
{
	int selected_option = 0;
	do
	{
		system("cls");
		std::cout << "=== Меню пользователя ===\n";
		std::cout << "1. Вывести данные о товарах.\n";
		std::cout << "2. Добавить запись о товаре.\n";
		std::cout << "3. Сортировки.\n";
		std::cout << "4. Поиск по товарам.\n";
		std::cout << "5. Индивидуальное задание.\n";
		std::cout << "0. Выйти.\n";
		std::cout << "Выберите пункт: ";
		selected_option = GetIntFromConsole();

		switch (selected_option)
		{
		case 1:
			PrintProductsTable(products);
			continue;
		case 2:
			ShowAddNewProductForm(false);
			continue;
		case 3:
			ShowSortingOptionsMenu();
			continue;
		case 4:
			ShowSearchOptionsMenu();
			continue;
		case 5:
			PerformTask();
			continue;
		}

	} while (selected_option != 0);
}

void ShowAdminMenu()
{
	int selected_option = 0;
	do
	{
		system("cls");
		std::cout << "=== Меню администратора ===\n";
		std::cout << "1. Добавить учетную запись.\n";
		std::cout << "2. Вывести учетные записи пользователей.\n";
		std::cout << "3. Редактировать учетную запись пользователя.\n";
		std::cout << "4. Удалить учетную запись пользователя.\n";
		std::cout << "5. Добавить запись о товаре.\n";
		std::cout << "6. Вывести данные о товарах.\n";
		std::cout << "7. Редактировать запись с товаром.\n";
		std::cout << "8. Удалить товар.\n";
		std::cout << "9. Сортировки.\n";
		std::cout << "10. Поиск по товарам.\n";
		std::cout << "11. Индивидуальное задание.\n";
		std::cout << "0. Выйти.\n";
		std::cout << "Выберите пункт: ";
		selected_option = GetIntFromConsole();

		switch (selected_option)
		{
		case 1:
			ShowAddNewUserForm();
			continue;
		case 2:
			PrintUsersTable();
			continue;
		case 3:
			EditUserEntry();
			continue;
		case 4:
			DeleteUserEntry();
			continue;
		case 5:
			ShowAddNewProductForm(true);
			continue;
		case 6:
			PrintProductsTable(products);
			continue;
		case 7:
			EditProductEntry();
			continue;
		case 8:
			DeleteProductEntry();
			continue;
		case 9:
			ShowSortingOptionsMenu();
			continue;
		case 10:
			ShowSearchOptionsMenu();
			continue;
		case 11:
			PerformTask();
			continue;
		}
	} while (selected_option != 0);
}

void ShowSortingOptionsMenu()
{
	system("cls");
	if (!products.empty())
	{
		int selected_option = 0;
		do
		{
			system("cls");
			std::cout << "=== Сортировать товары ===\n";
			std::cout << "1. Сортировать по названию.\n";
			std::cout << "2. Сортировать по цене.\n";
			std::cout << "3. Сортировать по дате добавления.\n";
			std::cout << "0. Выйти.\n";
			selected_option = GetIntFromConsole();

			switch (selected_option)
			{
			case 1:
				PrintProductsTable(GetProductsSortedByName());
				continue;
			case 2:
				PrintProductsTable(GetProductsSortedByPrice());
				continue;
			case 3:
				PrintProductsTable(GetProductsSortedByDate());
				continue;
			}
		} while (selected_option != 0);
	}
	else
	{
		std::cout << "Таблица товаров на данный момент пуста." << std::endl;
		system("pause");
	}
}

// Показывает меню с выбором полей пользователя, доступных к редактированию
void ShowEditUserMenu(UserInfo& user)
{
	int selected_index = -1;
	do
	{
		system("cls");
		std::cout << "Меню доступных полей:\n";
		std::cout << "1. Имя.\n";
		std::cout << "2. Фамилия.\n";
		std::cout << "3. Отчество.\n";
		std::cout << "4. Логин.\n";
		std::cout << "5. Пароль.\n";
		std::cout << "0. Вернуться.\n";
		std::cout << "Выберите пункт: ";
		selected_index = GetIntFromConsole();

		system("cls");
		switch (selected_index)
		{
		case 1:
			std::cout << "Введите новое имя: ";
			user.fio.name = HandleCyrillicInput();
			continue;
		case 2:
			std::cout << "Введите новую фамилию: ";
			user.fio.surname = HandleCyrillicInput();
			continue;
		case 3:
			std::cout << "Введите новое отчество: ";
			user.fio.middle_name = HandleCyrillicInput();
			continue;
		case 4:
			while (true)
			{
				std::cout << "Введите новый логин: ";
				std::string user_login = HandleLoginInput();
				auto it = std::find_if(
					users.begin(),
					users.end(),
					[user_login](const UserInfo& user) {
						return user.auth_info.login == user_login;
					}
				);
				if (it != users.end())
				{
					std::cout << std::endl << "Пользователь с таким логином уже существует." << std::endl;
				}
				else
				{
					user.auth_info.login = user_login;
					break;
				}
			}
			continue;
		case 5:
			std::cout << "Введите новый пароль: ";
			user.auth_info.password = HandlePasswordInput();
			continue;
		}
	} while (selected_index != 0);
}

// Показывает меню с выбором полей товара, доступных к редактированию
void ShowEditProductMenu(ProductEntry& product)
{
	int selected_index = -1;
	do
	{
		system("cls");
		std::cout << "Меню доступных полей:\n";
		std::cout << "1. Название.\n";
		std::cout << "2. Количество.\n";
		std::cout << "3. Цена.\n";
		std::cout << "4. Дата добавления.\n";
		std::cout << "5. ФИО добавившего пользователя.\n";
		std::cout << "0. Вернуться.\n";
		std::cout << "Выберите пункт: ";
		selected_index = GetIntFromConsole();

		system("cls");
		switch (selected_index)
		{
		case 1:
			std::cout << "Введите новое название: ";
			product.name = HandleCyrillicInput();
			continue;
		case 2:
			std::cout << "Введите новое количество: ";
			product.count = GetIntFromConsole();
			continue;
		case 3:
			std::cout << "Введите новую цену: ";
			product.price = GetDoubleFromConsole();
			continue;
		case 4:
			product.time = GetSystemtimeFromConsole();
			continue;
		case 5:
			product.added_by = GetFIOFromConsole();
			continue;
		}
	} while (selected_index != 0);
}

// Показывает меню с вариантами поиска по товарам
void ShowSearchOptionsMenu()
{
	system("cls");
	if (!products.empty())
	{
		int selected_option = 0;
		std::string user_str_input;
		SYSTEMTIME date_input{};
		do
		{
			system("cls");
			std::cout << "=== Поиск по товарам ===\n";
			std::cout << "1. Поиск по названию.\n";
			std::cout << "2. Поиск по дате добавления.\n";
			std::cout << "3. Поиск по фамилии добавившего.\n";
			std::cout << "0. Выйти.\n";
			selected_option = GetIntFromConsole();

			switch (selected_option)
			{
			case 1:
				system("cls");
				std::cout << "Введите название товара: ";
				user_str_input = HandleCyrillicInput();
				PrintProductsTable(GetProductsByName(user_str_input));
				continue;
			case 2:
				system("cls");
				std::cout << "Введите год (2020-2022): ";
				date_input.wYear = GetIntFromConsoleWithBounds(2020, 2022);
				std::cout << "Введите месяц (1-12): ";
				date_input.wMonth = GetIntFromConsoleWithBounds(1, 12);
				std::cout << "Введите день (1-31): ";
				date_input.wDay = GetIntFromConsoleWithBounds(1, 31);
				PrintProductsTable(GetProductsByDate(date_input));
				continue;
			case 3:
				system("cls");
				std::cout << "Введите фамилию: ";
				user_str_input = HandleCyrillicInput();
				PrintProductsTable(GetProductsByAdderSurname(user_str_input));
				continue;
			}
		} while (selected_option != 0);
	}
	else
	{
		std::cout << "Таблица товаров на данный момент пуста." << std::endl;
		system("pause");
	}
}

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