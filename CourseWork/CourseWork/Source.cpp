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
	std::string surname; // фамилия
	std::string name; // имя
	std::string middle_name; // отчество
};

// получить фио в формате строки для вывода в таблице
std::string GetFIOString(const FIO& fio)
{
	return fio.surname + " " + fio.name + " " + fio.middle_name; 
}

struct UserInfo
{
	AuthInfo auth_info;
	FIO fio;
	int role; // 0 - пользователь, 1 - админ
};

struct ProductEntry
{
	std::string name; // наименование товара
	int count; // количество товара на складе
	double price; // цена за один товар
	SYSTEMTIME time; // время добавления товара
	FIO added_by; // ФИО зарегистрировавшего товар
};

// функция для сравнения двух дат
bool IsTimeLess(SYSTEMTIME lhs, SYSTEMTIME rhs)
{
	return std::tie(lhs.wYear, lhs.wMonth, lhs.wDay, lhs.wHour, lhs.wMinute, lhs.wSecond)
	< std::tie(rhs.wYear, rhs.wMonth, rhs.wDay, rhs.wHour, rhs.wMinute, rhs.wSecond);
}

// Конвертирует дату формата SYSTEMTIME в time_t
time_t system2utc(const SYSTEMTIME& time)
{
	struct tm stm = { 0 };
	stm.tm_sec = time.wSecond;
	stm.tm_min = time.wMinute;
	stm.tm_hour = time.wHour;
	stm.tm_mday = time.wDay;
	stm.tm_mon = time.wMonth - 1;
	stm.tm_year = time.wYear - 1900;
	stm.tm_wday = time.wDayOfWeek;
	stm.tm_yday = 0;
	stm.tm_isdst = 0;

	return mktime(&stm);
}

// Проверяет, прошло ли с даты time months месяцев
bool isNMonthsBeforeNow(SYSTEMTIME time, int months)
{
	SYSTEMTIME now;
	GetLocalTime(&now);
	int monthsBefore = (system2utc(now) - system2utc(time)) / 2678400;
	return monthsBefore >= months;
}

// получить дату в формате DD/MM/YYYY HH:MM
std::string GetTimeString(SYSTEMTIME time)
{
	return (time.wDay < 10 ? "0" : "") + std::to_string(time.wDay) + "/"
		+ (time.wMonth < 10 ? "0" : "") + std::to_string(time.wMonth) + "/"
		+ std::to_string(time.wYear) + " "
		+ (time.wHour < 10 ? "0" : "") + std::to_string(time.wHour) + ":"
		+ (time.wMinute < 10 ? "0" : "") + std::to_string(time.wMinute) + ":"
		+ (time.wSecond < 10 ? "0" : "") + std::to_string(time.wSecond);
	
}

// Максимальные длины для полей логин и пароль, ФИО
const int MAX_LOGIN_LENGTH = 12;
const int MAX_PASSWORD_LENGTH = 12;
const int MAX_USER_INPUT_LENGTH = 20;
// Названия служебных файлов
const std::string ADMIN_CREDENTIALS_FILENAME = "admin.txt";
const std::string USERS_CREDENTIALS_FILENAME = "users.txt";
const std::string PRODUCTS_DATA_FILENAME = "products.txt";
// Вектор, содержащий данные о пользователях
std::vector<UserInfo> users;
// Вектор, содержащий данные о товарах
std::vector<ProductEntry> products;
// Текущий авторизованный пользователь
UserInfo current_user;

// Считывает число типа int из консоли, при неверных входных данных пытается считать число заново
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

// Аналог GetIntFromConsole(), но с дополнительной проверкой на вхождение в диапазон
int GetIntFromConsoleWithBounds(int left, int right)
{
	int value = left - 1;
	while (!(std::cin >> value) || !(value <= right && value >= left)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if(!(value <= right && value >= left))
		{
			std::cout << "Введенное число не входит в диапазон. Введите число: ";
		} else
		{
			std::cout << "Введен неправильный символ. Введите число: ";
		}
	}
	return value;
}

// Считывает число типа double из консоли, при неверных входных данных пытается считать число заново
double GetDoubleFromConsole()
{
	double value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Введен неправильный символ. Введите число: ";
	}
	return value;
}

// Функция предоставляющая пользователю возможность вводить дату с консоли
// с точностью вплоть до минут
SYSTEMTIME GetSystemtimeFromConsole()
{
	SYSTEMTIME new_time{};
	std::cout << "Введите год (2020-2022): ";
	new_time.wYear = GetIntFromConsoleWithBounds(2020, 2022);
	std::cout << "Введите месяц (1-12): ";
	new_time.wMonth = GetIntFromConsoleWithBounds(1, 12);
	std::cout << "Введите день (1-31): ";
	new_time.wDay = GetIntFromConsoleWithBounds(1, 31);
	std::cout << "Введите час (1-24): ";
	new_time.wHour = GetIntFromConsoleWithBounds(0, 23);
	std::cout << "Введите минуты (0-59): ";
	new_time.wMinute = GetIntFromConsoleWithBounds(0, 59);
	//std::cout << "Введите секунды (0-59): ";
	//new_time.wSecond = GetIntFromConsoleWithBounds(0, 59);
	new_time.wSecond = 0;
	return new_time;
}

// Функции для проверки валидности введенных пользователем символов
bool IsLoginCharInvalid(char c)
{
	return !(c >= 65 && c <= 90) && // A-Z
		!(c >= 97 && c <= 122) && // a-z
		!(c >= 48 && c <= 57) && // 0-9
		c != '\r' && c != '\b'; // enter + backspace
}

bool IsPasswordCharInvalid(char c)
{
	return !(c >= 33 && c <= 122) && c != '\r' && c != '\b';
}

bool IsCyrillicCharInvalid(char c)
{
	return !(c >= -64 && c <= 0) && c != '\r' && c != '\b';
}
// ------------

// Функции, контролирующие набор данных пользователем: валидируют вводимые символы и
// следят за длиной вводимых строк. Функция для ввода пароля скрывает символами *
// введенные данные
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
// -------

// Функция для введения фамилии, имени, отчества из консоли
FIO GetFIOFromConsole()
{
	FIO new_fio;
	std::cout << "Введите новую фамилию: ";
	new_fio.surname = HandleCyrillicInput();
	std::cout << std::endl << "Введите новое имя: ";
	new_fio.name = HandleCyrillicInput();
	std::cout << std::endl << "Введите новое отчество: ";
	new_fio.middle_name = HandleCyrillicInput();
	return new_fio;
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

// Функция показывает форму для ввода пользовательских данных: логин + пароль
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

// Последовательно показывает поля формы для ввода данных,
// необходимых для добавления нового пользователя.
// Введенный логин проверяет на уникальность
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

// Последовательно показывает поля формы для ввода данных,
// необходимых для добавления нового товара.
// параметр admin_mode отвечает за режим добавления товара:
// - если товар добавляет админ, он может вводить произвольное ФИО добавившего пользователя
// - если товар добавляет пользователь, как ФИО добавившего пользователя берётся его ФИО
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
	} else
	{
		product.added_by.name = current_user.fio.name;
		product.added_by.surname = current_user.fio.surname;
		product.added_by.middle_name = current_user.fio.middle_name;
	}


	products.push_back(product);
	std::cout << std::endl << "Запись успешно добавлена." << std::endl;
	system("pause");
}

// Блок констант, задающих ширину колонкам данных в PrintUserEntry и PrintUserTableHeader
const int USER_NUMBER_COLUMN_WIDTH = 5;
const int USER_LOGIN_COLUMN_WIDTH = 20;
const int USER_PASSWORD_COLUMN_WIDTH = 30;
const int USER_SURNAME_COLUMN_WIDTH = 30;
const int USER_NAME_COLUMN_WIDTH = 30;
const int USER_MIDDLE_NAME_COLUMN_WIDTH = 30;

// Функция, отвечающая за вывод в консоль заголовков колонок таблицы пользователей
void PrintUserTableHeader()
{
	std::cout << "_______________________________________________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(USER_NUMBER_COLUMN_WIDTH) << "#" << "|"
		<< std::setw(USER_LOGIN_COLUMN_WIDTH) << "Логин" << "|"
		<< std::setw(USER_PASSWORD_COLUMN_WIDTH) << "Пароль" << "|"
		<< std::setw(USER_SURNAME_COLUMN_WIDTH) << "Фамилия" << "|"
		<< std::setw(USER_NAME_COLUMN_WIDTH) << "Имя" << "|"
		<< std::setw(USER_MIDDLE_NAME_COLUMN_WIDTH) << "Отчество" << "|"
		<< std::endl;
	std::cout << "|_____|____________________|______________________________|______________________________|______________________________|______________________________|" << std::endl;
}

// Выводит строку с данными пользователя user
void PrintUserEntry(int index, const UserInfo& user)
{
	std::cout << "|" << std::fixed
		<< std::setw(USER_NUMBER_COLUMN_WIDTH) << index << "|"
		<< std::setw(USER_LOGIN_COLUMN_WIDTH) << user.auth_info.login << "|"
		<< std::setw(USER_PASSWORD_COLUMN_WIDTH) << user.auth_info.password << "|"
		<< std::setw(USER_SURNAME_COLUMN_WIDTH) << user.fio.surname << "|"
		<< std::setw(USER_NAME_COLUMN_WIDTH) << user.fio.name << "|"
		<< std::setw(USER_MIDDLE_NAME_COLUMN_WIDTH) << user.fio.middle_name << "|"
		<< std::endl;
	std::cout << "|_____|____________________|______________________________|______________________________|______________________________|______________________________|" << std::endl;
}

//  Блок констант, задающих ширину колонкам данных в PrintProductEntry и PrintProductTableHeader
const int PRODUCT_NUMBER_COLUMN_WIDTH = 5;
const int PRODUCT_NAME_COLUMN_WIDTH = 20;
const int PRODUCT_COUNT_COLUMN_WIDTH = 15;
const int PRODUCT_PRICE_COLUMN_WIDTH = 15;
const int PRODUCT_DATE_COLUMN_WIDTH = 20;
const int PRODUCT_FIO_COLUMN_WIDTH = 62;

// Функция, отвечающая за вывод в консоль заголовков колонок таблицы товаров
void PrintProductTableHeader()
{
	std::cout << "_______________________________________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(PRODUCT_NUMBER_COLUMN_WIDTH) << "#" << "|"
		<< std::setw(PRODUCT_NAME_COLUMN_WIDTH) << "Название" << "|"
		<< std::setw(PRODUCT_COUNT_COLUMN_WIDTH) << "Количество" << "|"
		<< std::setw(PRODUCT_PRICE_COLUMN_WIDTH) << "Цена" << "|"
		<< std::setw(PRODUCT_DATE_COLUMN_WIDTH) << "Дата" << "|"
		<< std::setw(PRODUCT_FIO_COLUMN_WIDTH) << "ФИО добавившего пользователя" << "|"
		<< std::endl;
	std::cout << "|_____|____________________|_______________|_______________|____________________|______________________________________________________________|" << std::endl;
}

// Выводит строку с данными о товаре product
void PrintProductEntry(int index, const ProductEntry& product)
{
	std::cout << "|" << std::fixed
		<< std::setw(PRODUCT_NUMBER_COLUMN_WIDTH) << index << "|"
		<< std::setw(PRODUCT_NAME_COLUMN_WIDTH) << product.name << "|"
		<< std::setw(PRODUCT_COUNT_COLUMN_WIDTH) << product.count << "|"
		<< std::setw(PRODUCT_PRICE_COLUMN_WIDTH) << std::setprecision(2) << product.price << "|"
		<< std::setw(PRODUCT_DATE_COLUMN_WIDTH) << GetTimeString(product.time) << "|"
		<< std::setw(PRODUCT_FIO_COLUMN_WIDTH) << GetFIOString(product.added_by) << "|"
		<< std::endl;
	std::cout << "|_____|____________________|_______________|_______________|____________________|______________________________________________________________|" << std::endl;
}

// функция для печати таблицы с данными о пользователях
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

// функция для печати таблицы с данными о товарах
void PrintProductsTable(const std::vector<ProductEntry>& products_vector)
{
	system("cls");
	std::cout << "=== Товары ===" << std::endl;

	if (!products_vector.empty())
	{
		PrintProductTableHeader();
		for (int i = 0; i < products_vector.size(); i++)
		{
			PrintProductEntry(i + 1, products_vector[i]);
		}
	}
	else
	{
		std::cout << "На данный момент нет записей." << std::endl;
	}

	system("pause");
}

// функция выбора пользователя (применяется при редактировании и удалении)
int SelectUserRow()
{
	std::cout << "Введите индекс пользователя: ";
	return GetIntFromConsole();
}

// функция выбора товара (применяется при редактировании и удалении)
int SelectProductRow()
{
	std::cout << "Введите индекс продукта: ";
	return GetIntFromConsole();
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

// Позволяет выбрать пользователя, данные которого нужно редактировать,
// вызывает функцию для показа меню с доступными опциями или уведомляет
// о том, что таблица пуста/выбран неправильный пользователь
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
			// Если были отредактированы собственные данные
			if(users[selectedIndex].auth_info.login == current_user.auth_info.login)
			{
				current_user = users[selectedIndex];
			}
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

// Позволяет выбрать товар, данные которого нужно редактировать,
// вызывает функцию для показа меню с доступными опциями или уведомляет
// о том, что таблица пуста/выбран неправильный товар
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

// Позволяет выбрать пользователя, которого нужно удалить
// или уведомляет о том, что таблица пуста/выбран неправильный пользователь
void DeleteUserEntry()
{
	system("cls");
	if(!users.empty())
	{
		std::cout << "=== Удаление пользователя ===" << std::endl;
		int selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			if(users[selectedIndex].auth_info.login != current_user.auth_info.login)
			{
				users.erase(users.begin() + selectedIndex);
			} else
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
	} else
	{
		std::cout << "Таблица пользователей на данный момент пуста." << std::endl;
		system("pause");
	}
}

// Позволяет выбрать товар, который нужно удалить
// или уведомляет о том, что таблица пуста/выбран неправильный товар
void DeleteProductEntry()
{
	system("cls");
	if(!products.empty())
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
	} else
	{
		std::cout << "Таблица товаров на данный момент пуста." << std::endl;
		system("pause");
	}
}

// Функции, возвращающие вектор товаров, отсортированных по признаку:
// - по имени
std::vector<ProductEntry> GetProductsSortedByName()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
	{
			return lhs.name < rhs.name;
	});
	return new_products;
}

// - по цене
std::vector<ProductEntry> GetProductsSortedByPrice()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return lhs.price < rhs.price;
		});
	return new_products;
}

// - по дате добавления
std::vector<ProductEntry> GetProductsSortedByDate()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return IsTimeLess(lhs.time, rhs.time);
		});
	return new_products;
}
// -------

// Функции, возвращающие вектор товаров, соответствующих:
// - название равно параметру name
std::vector<ProductEntry> GetProductsByName(const std::string& name)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(), 
		products.end(), 
		std::back_inserter(new_products), 
		[name](const ProductEntry& product) {return product.name == name; });
	return new_products;
}

// - фамилия добавившего товар пользователя равна параметру surname
std::vector<ProductEntry> GetProductsByAdderSurname(const std::string& surname)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(),
		products.end(),
		std::back_inserter(new_products),
		[surname](const ProductEntry& product) {return product.added_by.surname == surname; });
	return new_products;
}

// - товар был добавлен в дату date (с точностью до год-месяц-день)
std::vector<ProductEntry> GetProductsByDate(SYSTEMTIME date)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(),
		products.end(),
		std::back_inserter(new_products),
		[date](const ProductEntry& product)
		{
			return product.time.wMonth == date.wMonth && product.time.wDay == date.wDay && product.time.wYear == date.wYear;
		});
	return new_products;
}
// -------

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

// Показывает меню с вариантами сортировки товаров
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

// Возвращает вектор товаров согласно требований индивидуального задания:
// вывести в алфавитном порядке список товаров,
// хранящихся более x месяцев,
// стоимость которых превышает y рублей(x, y вводятся с клавиатуры)
std::vector<ProductEntry> GetProductsByDurationAndPrice(int min_months, double min_price)
{
	std::vector<ProductEntry> new_products;
	std::copy_if(
		products.begin(),
		products.end(),
		std::back_inserter(new_products),
		[min_months, min_price](const ProductEntry& product)
		{
			return isNMonthsBeforeNow(product.time, min_months) && product.price > min_price;
		});
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return lhs.name < rhs.name;
		});
	return new_products;
}

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

// Показывает опции меню, доступные для пользователя
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

// Показывает опции меню, доступные для администратора
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

// Проверяют введенные пользователем в форме для авторизации
// данные и, если такой пользователь с соответствующим уровнем
// доступа найден, авторизует пользователя и возвращает true
bool CheckAdminCredentialsAndLogIn(const AuthInfo& form)
{
	bool credentials_found = false;
	auto it = std::find_if(users.begin(), users.end(), [form](const UserInfo& user)
	{
			return user.auth_info.login == form.login;
	});
	if(it != users.end())
	{
		if (it->auth_info.password == form.password && it->role == 1)
		{
			credentials_found = true;
			current_user = *it;
		}
	}
	return credentials_found;
}

bool CheckUserCredentialsAndLogIn(const AuthInfo& form)
{
	bool found_credentials = false;
	auto it = std::find_if(users.begin(), users.end(), [form](const UserInfo& user)
	{
			return user.auth_info.login == form.login;
	});
	if(it != users.end())
	{
		if(it->auth_info.password == form.password && it->role == 0)
		{
			found_credentials = true;
			current_user = *it;
		}
	}
	return found_credentials;
}
// --------

// Считывает данные пользователей и их уровни доступа из файла
// или уведомляет, что не удалось открыть файл
void ReadUsersFromFile()
{
	std::ifstream ifs;
	ifs.open(USERS_CREDENTIALS_FILENAME);
	if(ifs.is_open())
	{
		std::string file_line;
		while(std::getline(ifs, file_line))
		{
			UserInfo user_info;
			std::istringstream oss(file_line);
			oss >> user_info.auth_info.login >> user_info.auth_info.password >> user_info.role >> user_info.fio.name >> user_info.fio.surname >> user_info.fio.middle_name;
			users.push_back(user_info);
		}
	} else
	{
		std::cout << "Не удалось открыть файл с данными пользователей." << std::endl;
		system("pause");
	}
}

// Считывает данные о товарах из файла
// или уведомляет, что не удалось открыть файл
void ReadProductsFromFile()
{
	std::ifstream ifs;
	ifs.open(PRODUCTS_DATA_FILENAME);
	if (ifs.is_open())
	{
		std::string file_line;
		while (std::getline(ifs, file_line))
		{
			ProductEntry product_info;
			SYSTEMTIME& time = product_info.time;
			FIO& fio = product_info.added_by;
			std::istringstream oss(file_line);

			oss >> product_info.name >> product_info.count >> product_info.price 
				>> time.wYear >> time.wMonth >> time.wDay >> time.wHour >> time.wMinute >> time.wSecond
				>> fio.name >> fio.surname >> fio.middle_name;
			products.push_back(product_info);
		}
	}
	else
	{
		std::cout << "Не удалось открыть файл с данными о товарах." << std::endl;
		system("pause");
	}
}

// Сохраняет считанные из файла и измененные в процессе работы программы данные обратно в файл
// или уведомляет о невозможности открыть файл
void SaveUsersToFile()
{
	std::ofstream ofs;
	ofs.open(USERS_CREDENTIALS_FILENAME);
	if (ofs.is_open())
	{
		for(int i = 0; i < users.size(); i++)
		{
			const UserInfo& user_info = users[i];
			// TODO: нормальные колонки
			ofs << user_info.auth_info.login << " " << user_info.auth_info.password << " " << user_info.role << " "
			<< user_info.fio.name << " " << user_info.fio.surname << " " << user_info.fio.middle_name << std::endl;
		}
	} else
	{
		std::cout << "Не удалось открыть файл для сохранения данных пользователей." << std::endl;
		system("pause");
	}
}

void SaveProductsToFile()
{
	std::ofstream ofs;
	ofs.open(PRODUCTS_DATA_FILENAME);
	if (ofs.is_open())
	{
		for (int i = 0; i < products.size(); i++)
		{
			const ProductEntry& product = products[i];
			const SYSTEMTIME& time = product.time;
			const FIO& fio = product.added_by;
			// TODO: нормальные колонки
			ofs << product.name << " " << product.count << " " << product.price << " "
				<< time.wYear << " " << time.wMonth << " " << time.wDay << " " << time.wHour << " " << time.wMinute << " " << time.wSecond << " "
				<< fio.name << " " << fio.surname << " " << fio.middle_name << std::endl;
		}
	}
	else
	{
		std::cout << "Не удалось открыть файл для сохранения данных о товарах." << std::endl;
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