#include "DataAccessUtils.h"

#include "Global.h"

void ShowAddNewUserForm()
{
	system("cls");
	UserInfo user_info;
	AuthInfo& user_auth_info = user_info.auth_info;
	FIO& user_fio = user_info.fio;
	std::cout << "=== Добавление учетной записи ===" << std::endl;
	std::cout << "Введите имя пользователя: ";
	user_fio.name = HandleCyrillicInput();
	std::cout << std::endl << "Введите фамилию пользователя: ";
	user_fio.surname = HandleCyrillicInput();
	std::cout << std::endl << "Введите отчество пользователя: ";
	user_fio.middle_name = HandleCyrillicInput();
	while (true)
	{
		std::cout << std::endl << "Введите логин пользователя: ";
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
			user_auth_info.login = user_login;
			break;
		}
	}
	std::cout << std::endl << "Введите пароль пользователя: ";
	user_auth_info.password = HandlePasswordInput();
	std::cout << std::endl << "Выберите роль для нового пользователя (0 - пользователь, 1 - админ): ";
	user_info.role = GetIntFromConsoleWithBounds(0, 1);

	users.push_back(user_info);
	std::cout << std::endl << "Запись успешно добавлена." << std::endl;
	system("pause");
}

void ShowAddNewProductForm(bool admin_mode) {
	system("cls");
	ProductEntry product;
	std::cout << "=== Добавление нового товара ===" << std::endl;
	std::cout << "Введите название товара: ";
	product.name = HandleCyrillicInput();
	std::cout << std::endl << "Введите количество товара на складе: ";
	product.count = GetIntFromConsole();
	std::cout << "Введите цену на товар: ";
	product.price = GetDoubleFromConsole();
	product.time = GetSystemtimeFromConsole();
	if (admin_mode)
	{
		std::cout << "Введите имя добавившего пользователя: ";
		product.added_by.name = HandleCyrillicInput();
		std::cout << std::endl << "Введите фамилию добавившего пользователя: ";
		product.added_by.surname = HandleCyrillicInput();
		std::cout << std::endl << "Введите отчество добавившего пользователя: ";
		product.added_by.middle_name = HandleCyrillicInput();
	}
	else
	{
		product.added_by.name = current_user.fio.name;
		product.added_by.surname = current_user.fio.surname;
		product.added_by.middle_name = current_user.fio.middle_name;
	}


	products.push_back(product);
	std::cout << std::endl << "Запись успешно добавлена." << std::endl;
	system("pause");
}

void EditUserEntry() {
	system("cls");
	if (!users.empty())
	{
		std::cout << "=== Редактирование учетной записи ===" << std::endl;
		int selectedIndex = -1;
		selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			ShowEditUserMenu(users[selectedIndex]);
			// Если были отредактированы собственные данные
			if (users[selectedIndex].auth_info.login == current_user.auth_info.login)
			{
				current_user = users[selectedIndex];
			}
		}
		else
		{
			std::cout << "Выбран несуществующий пользователь." << std::endl;
			system("pause");
		}
	}
	else
	{
		std::cout << "На данный момент нет записей, которые можно редактировать." << std::endl;
	}
}

void EditProductEntry() {
	system("cls");
	if (!products.empty())
	{
		std::cout << "=== Редактирование товара ===" << std::endl;
		int selectedIndex = -1;
		selectedIndex = SelectProductRow();
		if (selectedIndex >= 0 && selectedIndex < products.size())
		{
			ShowEditProductMenu(products[selectedIndex]);
		}
		else
		{
			std::cout << "Выбран несуществующий товар." << std::endl;
			system("pause");
		}
	}
	else
	{
		std::cout << "На данный момент нет записей, которые можно редактировать." << std::endl;
	}
}

void DeleteUserEntry()
{
	system("cls");
	if (!users.empty())
	{
		std::cout << "=== Удаление пользователя ===" << std::endl;
		int selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			if (users[selectedIndex].auth_info.login != current_user.auth_info.login)
			{
				users.erase(users.begin() + selectedIndex);
			}
			else
			{
				std::cout << "Нельзя удалить свой аккаунт." << std::endl;
				system("pause");
			}
		}
		else
		{
			std::cout << "Выбран несуществующий пользователь." << std::endl;
			system("pause");
		}
	}
	else
	{
		std::cout << "Таблица пользователей на данный момент пуста." << std::endl;
		system("pause");
	}
}

void DeleteProductEntry()
{
	system("cls");
	if (!products.empty())
	{
		std::cout << "=== Удаление товара ===" << std::endl;
		int selectedIndex = SelectProductRow();
		if (selectedIndex >= 0 && selectedIndex < products.size())
		{
			products.erase(products.begin() + selectedIndex);
		}
		else
		{
			std::cout << "Выбран несуществующий товар." << std::endl;
			system("pause");
		}
	}
	else
	{
		std::cout << "Таблица товаров на данный момент пуста." << std::endl;
		system("pause");
	}
}