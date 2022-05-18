#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <Windows.h>
#include <conio.h>

#undef max
#undef min

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

struct AuthInfo
{
	std::string login;
	std::string password;
};

struct FIO {
	std::string surname;
	std::string name;
	std::string middle_name;
};

struct UserInfo
{
	AuthInfo auth_info;
	FIO fio;
};

struct ProductEntry
{
	std::string name; // наименование товара
	int count; // количество товара на складе
	double price; // цена за один товар
	// date 
	FIO added_by; // ФИО зарегистрировавшего товар
};

// Максимальные длины для полей логин и пароль, ФИО
const int MAX_LOGIN_LENGTH = 12;
const int MAX_PASSWORD_LENGTH = 12;
const int MAX_USER_INPUT_LENGTH = 20;
// TODO: comment
const std::string ADMIN_CREDENTIALS_FILENAME = "admin.txt";
// Вектор, содержащий данные о пользователях
std::vector<UserInfo> users;
// Вектор, содержащий данные о товарах
std::vector<ProductEntry> products;


// TODO: comment
int GetIntFromConsole()
{
	int value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Введен неправильный символ. Введите число: ";
	}
	return value;
}

bool IsLoginCharInvalid(char c)
{
	return !(c >= 65 && c <= 90) && // A-Z
		!(c >= 97 && c <= 122) && // a-z
		!(c >= 48 && c <= 57) && // 0-9
		c != '\r' && c != '\b'; // enter
}

bool IsPasswordCharInvalid(char c)
{
	return !(c >= 33 && c <= 122) && c != '\r' && c != '\b';
}

bool IsCyrillicCharInvalid(char c)
{
	return !(c >= -64 && c <= 0) && c != '\r' && c != '\b';
}


std::string HandleLoginInput()
{
	std::string login_string;
	char c = ' ';

	while (true)
	{
		do
		{
			c = _getch();
		} while (IsLoginCharInvalid(c));

		switch (c)
		{
		case '\r':
			if (!login_string.empty())
			{
				return login_string;
			}
			continue;
		case '\b':
			if (!login_string.empty())
			{
				login_string.pop_back();
				std::cout << "\b_\b";
			}
			continue;
		default:
			if (login_string.size() < MAX_LOGIN_LENGTH)
			{
				login_string.push_back(c);
				std::cout << c;
			}
		}
	}
}

std::string HandlePasswordInput()
{
	std::string password_string;
	char c = ' ';

	while(true)
	{
		do
		{
			c = _getch();
		} while (IsPasswordCharInvalid(c));

		switch (c)
		{
		case '\r':
			if (!password_string.empty())
			{
				return password_string;
			}
			continue;
		case '\b':
			if(!password_string.empty())
			{
				password_string.pop_back();
				std::cout << "\b_\b";
			}
			continue;
		default:
			if (password_string.size() < MAX_PASSWORD_LENGTH)
			{
				password_string.push_back(c);
				std::cout << '*';
			}
		}
	}
}

std::string HandleCyrillicInput()
{
	std::string input_string;
	char c = ' ';

	while(true)
	{
		do
		{
			c = _getch();
		} while (IsCyrillicCharInvalid(c));

		switch (c)
		{
		case '\r':
			if (!input_string.empty())
			{
				return input_string;
			}
			continue;
		case '\b':
			if (!input_string.empty())
			{
				input_string.pop_back();
				std::cout << "\b \b";
			}
			continue;
		default:
			if (input_string.size() < MAX_USER_INPUT_LENGTH)
			{
				input_string.push_back(c);
				std::cout << c;
			}
		}
	}
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
AuthInfo ShowAuthForm()
{
	AuthInfo form_info;

	system("cls");

	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor_position = {0,1};

	std::cout << "Пожалуйста, введите данные:" << std::endl;
	std::cout << "____________" << std::endl;
	std::cout << "____________" << std::endl;
	
	SetConsoleCursorPosition(console_handle, cursor_position);

	std::string user_login = HandleLoginInput();

	cursor_position.Y = 2;
	SetConsoleCursorPosition(console_handle, cursor_position);

	std::string user_password = HandlePasswordInput();

	form_info.login = user_login;
	form_info.password = user_password;

	return form_info;
}

void ShowUserMenu()
{
	
}

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
		if(it != users.end())
		{
			std::cout << std::endl << "Пользователь с таким логином уже существует." << std::endl;
		} else
		{
			user_auth_info.login = user_login;
			break;
		}
	}
	std::cout << std::endl << "Введите пароль пользователя: ";
	user_auth_info.password = HandlePasswordInput();
	users.push_back(user_info);
	std::cout << std::endl << "Запись успешно добавлена." << std::endl;
	system("pause");
}

void PrintUserTableHeader()
{
	std::cout << "_______________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(5) << "#" << "|"
		<< std::setw(20) << "Логин" << "|"
		<< std::setw(30) << "Пароль" << "|"
		<< std::setw(30) << "Фамилия" << "|"
		<< std::setw(30) << "Имя" << "|"
		<< std::endl;
	std::cout << "|_____|____________________|______________________________|______________________________|______________________________|" << std::endl;
}

void PrintUserEntry(int index, const UserInfo& user)
{
	std::cout << "|" << std::fixed
		<< std::setw(5) << index << "|"
		<< std::setw(20) << user.auth_info.login << "|"
		<< std::setw(30) << user.auth_info.password << "|"
		<< std::setw(30) << user.fio.surname << "|"
		<< std::setw(30) << user.fio.name << "|"
		<< std::endl;
	std::cout << "|_____|____________________|______________________________|______________________________|______________________________|" << std::endl;
}

void PrintUsersTable()
{
	system("cls");
	std::cout << "=== Пользователи ===" << std::endl;

	if (!users.empty())
	{
		PrintUserTableHeader();
		for (int i = 0; i < users.size(); i++)
		{
			PrintUserEntry(i + 1, users[i]);
		}
	} else
	{
		std::cout << "На данный момент нет записей." << std::endl;
	}

	system("pause");
}

int SelectUserRow()
{
	std::cout << "Введите индекс пользователя: ";
	return GetIntFromConsole();
}

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
		switch(selected_index)
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
			std::cout << "Введите новый логин: ";
			user.auth_info.login = HandleLoginInput();
			continue;
		case 5:
			std::cout << "Введите новый пароль: ";
			user.auth_info.password = HandlePasswordInput();
			continue;
		}
	} while (selected_index != 0);
}

void EditUserEntry() {
	system("cls");
	if(!users.empty())
	{
		std::cout << "=== Редактирование учетной записи ===" << std::endl;
		int selectedIndex = -1;
		selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			ShowEditUserMenu(users[selectedIndex]);
		}
		else
		{
			std::cout << "Выбран несуществующий пользователь." << std::endl;
			system("pause");
		}
	} else
	{
		std::cout << "На данный момент нет записей, которые можно редактировать." << std::endl;
	}
}

void DeleteUserEntry()
{
	system("cls");
	if(!users.empty())
	{
		std::cout << "=== Удаление пользователя ===" << std::endl;
		int selectedIndex = -1;
		selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			users.erase(users.begin() + selectedIndex);
		}
		else
		{
			std::cout << "Выбран несуществующий пользователь." << std::endl;
			system("pause");
		}
	} else
	{
		std::cout << "Таблица пользователей на данный момент пуста." << std::endl;
		system("pause");
	}
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
		}
	} while (selected_option != 0);
}

bool CheckAdminCredentials(const AuthInfo& form)
{
	std::ifstream ifs;
	ifs.open(ADMIN_CREDENTIALS_FILENAME);
	bool found_credentials = false;
	if(ifs.is_open())
	{
		std::string file_login;
		std::string file_password;
		ifs >> file_login >> file_password;
		if(form.login == file_login && form.password == file_password)
		{
			found_credentials = true;
		}
	} else
	{
		std::cout << "Произошла ошибка при открытии файла." << std::endl;
	}
	return found_credentials;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int selected_option = 0;

	do
	{
		selected_option = ShowAuthMenu();
		AuthInfo form_info;
		switch (selected_option)
		{
		case 1:
			form_info = ShowAuthForm();
			system("cls");
			if (CheckAdminCredentials(form_info))
			{
				ShowAdminMenu();
			}
			else
			{
				std::cout << "Введены неправильные данные." << std::endl;
				std::cout << "Нажмите любую клавишу чтобы продолжить." << std::endl;
				_getch();
			}
			break;
		case 2:
			form_info = ShowAuthForm();
			system("cls");
			break;
		default:
			system("cls");
			std::cout << "Работа программы завершена." << std::endl;
		}
	} while (selected_option != 3);

	return 0;
}