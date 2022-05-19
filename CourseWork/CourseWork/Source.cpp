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

struct AuthInfo
{
	std::string login;
	std::string password;
};

struct FIO {
	std::string surname; // �������
	std::string name; // ���
	std::string middle_name; // ��������
};

// �������� ��� � ������� ������ ��� ������ � �������
std::string GetFIOString(const FIO& fio)
{
	return fio.surname + " " + fio.name + " " + fio.middle_name; 
}

struct UserInfo
{
	AuthInfo auth_info;
	FIO fio;
	int role; // 0 - ������������, 1 - �����
};

struct ProductEntry
{
	std::string name; // ������������ ������
	int count; // ���������� ������ �� ������
	double price; // ���� �� ���� �����
	SYSTEMTIME time; // ����� ���������� ������
	FIO added_by; // ��� ������������������� �����
};

// ������� ��� ��������� ���� ���
bool IsTimeLess(SYSTEMTIME lhs, SYSTEMTIME rhs)
{
	return std::tie(lhs.wYear, lhs.wMonth, lhs.wDay, lhs.wHour, lhs.wMinute, lhs.wSecond)
	< std::tie(rhs.wYear, rhs.wMonth, rhs.wDay, rhs.wHour, rhs.wMinute, rhs.wSecond);
}

// ������������ ���� ������� SYSTEMTIME � time_t
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

// ���������, ������ �� � ���� time months �������
bool isNMonthsBeforeNow(SYSTEMTIME time, int months)
{
	SYSTEMTIME now;
	GetLocalTime(&now);
	int monthsBefore = (system2utc(now) - system2utc(time)) / 2678400;
	return monthsBefore >= months;
}

// �������� ���� � ������� DD/MM/YYYY HH:MM
std::string GetTimeString(SYSTEMTIME time)
{
	return (time.wDay < 10 ? "0" : "") + std::to_string(time.wDay) + "/"
		+ (time.wMonth < 10 ? "0" : "") + std::to_string(time.wMonth) + "/"
		+ std::to_string(time.wYear) + " "
		+ (time.wHour < 10 ? "0" : "") + std::to_string(time.wHour) + ":"
		+ (time.wMinute < 10 ? "0" : "") + std::to_string(time.wMinute) + ":"
		+ (time.wSecond < 10 ? "0" : "") + std::to_string(time.wSecond);
	
}

// ������������ ����� ��� ����� ����� � ������, ���
const int MAX_LOGIN_LENGTH = 12;
const int MAX_PASSWORD_LENGTH = 12;
const int MAX_USER_INPUT_LENGTH = 20;
// �������� ��������� ������
const std::string USERS_CREDENTIALS_FILENAME = "users.txt";
const std::string PRODUCTS_DATA_FILENAME = "products.txt";
// ������, ���������� ������ � �������������
std::vector<UserInfo> users;
// ������, ���������� ������ � �������
std::vector<ProductEntry> products;
// ������� �������������� ������������
UserInfo current_user;

// ��������� ����� ���� int �� �������, ��� �������� ������� ������ �������� ������� ����� ������
int GetIntFromConsole()
{
	int value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "������ ������������ ������. ������� �����: ";
	}
	return value;
}

// ������ GetIntFromConsole(), �� � �������������� ��������� �� ��������� � ��������
int GetIntFromConsoleWithBounds(int left, int right)
{
	int value = left - 1;
	while (!(std::cin >> value) || !(value <= right && value >= left)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if(!(value <= right && value >= left))
		{
			std::cout << "��������� ����� �� ������ � ��������. ������� �����: ";
		} else
		{
			std::cout << "������ ������������ ������. ������� �����: ";
		}
	}
	return value;
}

// ��������� ����� ���� double �� �������, ��� �������� ������� ������ �������� ������� ����� ������
double GetDoubleFromConsole()
{
	double value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "������ ������������ ������. ������� �����: ";
	}
	return value;
}

// ������� ��������������� ������������ ����������� ������� ���� � �������
// � ��������� ������ �� �����
SYSTEMTIME GetSystemtimeFromConsole()
{
	SYSTEMTIME new_time{};
	std::cout << "������� ��� (2020-2022): ";
	new_time.wYear = GetIntFromConsoleWithBounds(2020, 2022);
	std::cout << "������� ����� (1-12): ";
	new_time.wMonth = GetIntFromConsoleWithBounds(1, 12);
	std::cout << "������� ���� (1-31): ";
	new_time.wDay = GetIntFromConsoleWithBounds(1, 31);
	std::cout << "������� ��� (1-24): ";
	new_time.wHour = GetIntFromConsoleWithBounds(0, 23);
	std::cout << "������� ������ (0-59): ";
	new_time.wMinute = GetIntFromConsoleWithBounds(0, 59);
	//std::cout << "������� ������� (0-59): ";
	//new_time.wSecond = GetIntFromConsoleWithBounds(0, 59);
	new_time.wSecond = 0;
	return new_time;
}

// ������� ��� �������� ���������� ��������� ������������� ��������
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

// �������, �������������� ����� ������ �������������: ���������� �������� ������� �
// ������ �� ������ �������� �����. ������� ��� ����� ������ �������� ��������� *
// ��������� ������
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

// ������� ��� �������� �������, �����, �������� �� �������
FIO GetFIOFromConsole()
{
	FIO new_fio;
	std::cout << "������� ����� �������: ";
	new_fio.surname = HandleCyrillicInput();
	std::cout << std::endl << "������� ����� ���: ";
	new_fio.name = HandleCyrillicInput();
	std::cout << std::endl << "������� ����� ��������: ";
	new_fio.middle_name = HandleCyrillicInput();
	return new_fio;
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

// ������� ���������� ����� ��� ����� ���������������� ������: ����� + ������
AuthInfo ShowAuthForm()
{
	AuthInfo form_info;

	system("cls");

	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor_position = {0,1};

	std::cout << "����������, ������� ������:" << std::endl;
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

// ��������������� ���������� ���� ����� ��� ����� ������,
// ����������� ��� ���������� ������ ������������.
// ��������� ����� ��������� �� ������������
void ShowAddNewUserForm()
{
	system("cls");
	UserInfo user_info;
	AuthInfo& user_auth_info = user_info.auth_info;
	FIO& user_fio = user_info.fio;
	std::cout << "=== ���������� ������� ������ ===" << std::endl;
	std::cout << "������� ��� ������������: ";
	user_fio.name = HandleCyrillicInput();
	std::cout << std::endl << "������� ������� ������������: ";
	user_fio.surname = HandleCyrillicInput();
	std::cout << std::endl << "������� �������� ������������: ";
	user_fio.middle_name = HandleCyrillicInput();
	while (true)
	{
		std::cout << std::endl << "������� ����� ������������: ";
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
			std::cout << std::endl << "������������ � ����� ������� ��� ����������." << std::endl;
		} else
		{
			user_auth_info.login = user_login;
			break;
		}
	}
	std::cout << std::endl << "������� ������ ������������: ";
	user_auth_info.password = HandlePasswordInput();
	std::cout << std::endl << "�������� ���� ��� ������ ������������ (0 - ������������, 1 - �����): ";
	user_info.role = GetIntFromConsoleWithBounds(0, 1);

	users.push_back(user_info);
	std::cout << std::endl << "������ ������� ���������." << std::endl;
	system("pause");
}

// ��������������� ���������� ���� ����� ��� ����� ������,
// ����������� ��� ���������� ������ ������.
// �������� admin_mode �������� �� ����� ���������� ������:
// - ���� ����� ��������� �����, �� ����� ������� ������������ ��� ����������� ������������
// - ���� ����� ��������� ������������, ��� ��� ����������� ������������ ������ ��� ���
void ShowAddNewProductForm(bool admin_mode) {
	system("cls");
	ProductEntry product;
	std::cout << "=== ���������� ������ ������ ===" << std::endl;
	std::cout << "������� �������� ������: ";
	product.name = HandleCyrillicInput();
	std::cout << std::endl << "������� ���������� ������ �� ������: ";
	product.count = GetIntFromConsole();
	std::cout << "������� ���� �� �����: ";
	product.price = GetDoubleFromConsole();
	product.time = GetSystemtimeFromConsole();
	if (admin_mode)
	{
		std::cout << "������� ��� ����������� ������������: ";
		product.added_by.name = HandleCyrillicInput();
		std::cout << std::endl << "������� ������� ����������� ������������: ";
		product.added_by.surname = HandleCyrillicInput();
		std::cout << std::endl << "������� �������� ����������� ������������: ";
		product.added_by.middle_name = HandleCyrillicInput();
	} else
	{
		product.added_by.name = current_user.fio.name;
		product.added_by.surname = current_user.fio.surname;
		product.added_by.middle_name = current_user.fio.middle_name;
	}


	products.push_back(product);
	std::cout << std::endl << "������ ������� ���������." << std::endl;
	system("pause");
}

// ���� ��������, �������� ������ �������� ������ � PrintUserEntry � PrintUserTableHeader
const int USER_NUMBER_COLUMN_WIDTH = 5;
const int USER_ROLE_COLUMN_WIDTH = 5;
const int USER_LOGIN_COLUMN_WIDTH = 20;
const int USER_PASSWORD_COLUMN_WIDTH = 30;
const int USER_SURNAME_COLUMN_WIDTH = 30;
const int USER_NAME_COLUMN_WIDTH = 30;
const int USER_MIDDLE_NAME_COLUMN_WIDTH = 30;

// �������, ���������� �� ����� � ������� ���������� ������� ������� �������������
void PrintUserTableHeader()
{
	std::cout << "_____________________________________________________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(USER_NUMBER_COLUMN_WIDTH) << "#" << "|"
		<< std::setw(USER_NUMBER_COLUMN_WIDTH) << "����" << "|"
		<< std::setw(USER_LOGIN_COLUMN_WIDTH) << "�����" << "|"
		<< std::setw(USER_PASSWORD_COLUMN_WIDTH) << "������" << "|"
		<< std::setw(USER_SURNAME_COLUMN_WIDTH) << "�������" << "|"
		<< std::setw(USER_NAME_COLUMN_WIDTH) << "���" << "|"
		<< std::setw(USER_MIDDLE_NAME_COLUMN_WIDTH) << "��������" << "|"
		<< std::endl;
	std::cout << "|_____|_____|____________________|______________________________|______________________________|______________________________|______________________________|" << std::endl;
}

// ������� ������ � ������� ������������ user
void PrintUserEntry(int index, const UserInfo& user)
{
	std::cout << "|" << std::fixed
		<< std::setw(USER_NUMBER_COLUMN_WIDTH) << index << "|"
		<< std::setw(USER_ROLE_COLUMN_WIDTH) << user.role << "|"
		<< std::setw(USER_LOGIN_COLUMN_WIDTH) << user.auth_info.login << "|"
		<< std::setw(USER_PASSWORD_COLUMN_WIDTH) << user.auth_info.password << "|"
		<< std::setw(USER_SURNAME_COLUMN_WIDTH) << user.fio.surname << "|"
		<< std::setw(USER_NAME_COLUMN_WIDTH) << user.fio.name << "|"
		<< std::setw(USER_MIDDLE_NAME_COLUMN_WIDTH) << user.fio.middle_name << "|"
		<< std::endl;
	std::cout << "|_____|_____|____________________|______________________________|______________________________|______________________________|______________________________|" << std::endl;
}

//  ���� ��������, �������� ������ �������� ������ � PrintProductEntry � PrintProductTableHeader
const int PRODUCT_NUMBER_COLUMN_WIDTH = 5;
const int PRODUCT_NAME_COLUMN_WIDTH = 20;
const int PRODUCT_COUNT_COLUMN_WIDTH = 15;
const int PRODUCT_PRICE_COLUMN_WIDTH = 15;
const int PRODUCT_DATE_COLUMN_WIDTH = 20;
const int PRODUCT_FIO_COLUMN_WIDTH = 62;

// �������, ���������� �� ����� � ������� ���������� ������� ������� �������
void PrintProductTableHeader()
{
	std::cout << "_______________________________________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(PRODUCT_NUMBER_COLUMN_WIDTH) << "#" << "|"
		<< std::setw(PRODUCT_NAME_COLUMN_WIDTH) << "��������" << "|"
		<< std::setw(PRODUCT_COUNT_COLUMN_WIDTH) << "����������" << "|"
		<< std::setw(PRODUCT_PRICE_COLUMN_WIDTH) << "����" << "|"
		<< std::setw(PRODUCT_DATE_COLUMN_WIDTH) << "����" << "|"
		<< std::setw(PRODUCT_FIO_COLUMN_WIDTH) << "��� ����������� ������������" << "|"
		<< std::endl;
	std::cout << "|_____|____________________|_______________|_______________|____________________|______________________________________________________________|" << std::endl;
}

// ������� ������ � ������� � ������ product
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

// ������� ��� ������ ������� � ������� � �������������
void PrintUsersTable()
{
	system("cls");
	std::cout << "=== ������������ ===" << std::endl;

	if (!users.empty())
	{
		PrintUserTableHeader();
		for (int i = 0; i < users.size(); i++)
		{
			PrintUserEntry(i + 1, users[i]);
		}
	} else
	{
		std::cout << "�� ������ ������ ��� �������." << std::endl;
	}

	system("pause");
}

// ������� ��� ������ ������� � ������� � �������
void PrintProductsTable(const std::vector<ProductEntry>& products_vector)
{
	system("cls");
	std::cout << "=== ������ ===" << std::endl;

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
		std::cout << "�� ������ ������ ��� �������." << std::endl;
	}

	system("pause");
}

// ������� ������ ������������ (����������� ��� �������������� � ��������)
int SelectUserRow()
{
	std::cout << "������� ������ ������������: ";
	return GetIntFromConsole();
}

// ������� ������ ������ (����������� ��� �������������� � ��������)
int SelectProductRow()
{
	std::cout << "������� ������ ��������: ";
	return GetIntFromConsole();
}

// ���������� ���� � ������� ����� ������������, ��������� � ��������������
void ShowEditUserMenu(UserInfo& user)
{
	int selected_index = -1;
	do
	{
		system("cls");
		std::cout << "���� ��������� �����:\n";
		std::cout << "1. ���.\n";
		std::cout << "2. �������.\n";
		std::cout << "3. ��������.\n";
		std::cout << "4. �����.\n";
		std::cout << "5. ������.\n";
		std::cout << "0. ���������.\n";
		std::cout << "�������� �����: ";
		selected_index = GetIntFromConsole();

		system("cls");
		switch(selected_index)
		{
		case 1:
			std::cout << "������� ����� ���: ";
			user.fio.name = HandleCyrillicInput();
			continue;
		case 2:
			std::cout << "������� ����� �������: ";
			user.fio.surname = HandleCyrillicInput();
			continue;
		case 3:
			std::cout << "������� ����� ��������: ";
			user.fio.middle_name = HandleCyrillicInput();
			continue;
		case 4:
			while (true)
			{
				std::cout << "������� ����� �����: ";
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
					std::cout << std::endl << "������������ � ����� ������� ��� ����������." << std::endl;
				}
				else
				{
					user.auth_info.login = user_login;
					break;
				}
			}
			continue;
		case 5:
			std::cout << "������� ����� ������: ";
			user.auth_info.password = HandlePasswordInput();
			continue;
		}
	} while (selected_index != 0);
}

// ���������� ���� � ������� ����� ������, ��������� � ��������������
void ShowEditProductMenu(ProductEntry& product)
{
	int selected_index = -1;
	do
	{
		system("cls");
		std::cout << "���� ��������� �����:\n";
		std::cout << "1. ��������.\n";
		std::cout << "2. ����������.\n";
		std::cout << "3. ����.\n";
		std::cout << "4. ���� ����������.\n";
		std::cout << "5. ��� ����������� ������������.\n";
		std::cout << "0. ���������.\n";
		std::cout << "�������� �����: ";
		selected_index = GetIntFromConsole();

		system("cls");
		switch (selected_index)
		{
		case 1:
			std::cout << "������� ����� ��������: ";
			product.name = HandleCyrillicInput();
			continue;
		case 2:
			std::cout << "������� ����� ����������: ";
			product.count = GetIntFromConsole();
			continue;
		case 3:
			std::cout << "������� ����� ����: ";
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

// ��������� ������� ������������, ������ �������� ����� �������������,
// �������� ������� ��� ������ ���� � ���������� ������� ��� ����������
// � ���, ��� ������� �����/������ ������������ ������������
void EditUserEntry() {
	system("cls");
	if(!users.empty())
	{
		std::cout << "=== �������������� ������� ������ ===" << std::endl;
		int selectedIndex = -1;
		selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			ShowEditUserMenu(users[selectedIndex]);
			// ���� ���� ��������������� ����������� ������
			if(users[selectedIndex].auth_info.login == current_user.auth_info.login)
			{
				current_user = users[selectedIndex];
			}
		}
		else
		{
			std::cout << "������ �������������� ������������." << std::endl;
			system("pause");
		}
	} else
	{
		std::cout << "�� ������ ������ ��� �������, ������� ����� �������������." << std::endl;
	}
}

// ��������� ������� �����, ������ �������� ����� �������������,
// �������� ������� ��� ������ ���� � ���������� ������� ��� ����������
// � ���, ��� ������� �����/������ ������������ �����
void EditProductEntry() {
	system("cls");
	if (!products.empty())
	{
		std::cout << "=== �������������� ������ ===" << std::endl;
		int selectedIndex = -1;
		selectedIndex = SelectProductRow();
		if (selectedIndex >= 0 && selectedIndex < products.size())
		{
			ShowEditProductMenu(products[selectedIndex]);
		}
		else
		{
			std::cout << "������ �������������� �����." << std::endl;
			system("pause");
		}
	}
	else
	{
		std::cout << "�� ������ ������ ��� �������, ������� ����� �������������." << std::endl;
	}
}

// ��������� ������� ������������, �������� ����� �������
// ��� ���������� � ���, ��� ������� �����/������ ������������ ������������
void DeleteUserEntry()
{
	system("cls");
	if(!users.empty())
	{
		std::cout << "=== �������� ������������ ===" << std::endl;
		int selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			if(users[selectedIndex].auth_info.login != current_user.auth_info.login)
			{
				users.erase(users.begin() + selectedIndex);
			} else
			{
				std::cout << "������ ������� ���� �������." << std::endl;
				system("pause");
			}
		}
		else
		{
			std::cout << "������ �������������� ������������." << std::endl;
			system("pause");
		}
	} else
	{
		std::cout << "������� ������������� �� ������ ������ �����." << std::endl;
		system("pause");
	}
}

// ��������� ������� �����, ������� ����� �������
// ��� ���������� � ���, ��� ������� �����/������ ������������ �����
void DeleteProductEntry()
{
	system("cls");
	if(!products.empty())
	{
		std::cout << "=== �������� ������ ===" << std::endl;
		int selectedIndex = SelectProductRow();
		if (selectedIndex >= 0 && selectedIndex < products.size())
		{
			products.erase(products.begin() + selectedIndex);
		}
		else
		{
			std::cout << "������ �������������� �����." << std::endl;
			system("pause");
		}
	} else
	{
		std::cout << "������� ������� �� ������ ������ �����." << std::endl;
		system("pause");
	}
}

// �������, ������������ ������ �������, ��������������� �� ��������:
// - �� �����
std::vector<ProductEntry> GetProductsSortedByName()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
	{
			return lhs.name < rhs.name;
	});
	return new_products;
}

// - �� ����
std::vector<ProductEntry> GetProductsSortedByPrice()
{
	std::vector<ProductEntry> new_products = products;
	std::sort(new_products.begin(), new_products.end(), [](const ProductEntry& lhs, const ProductEntry& rhs)
		{
			return lhs.price < rhs.price;
		});
	return new_products;
}

// - �� ���� ����������
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

// �������, ������������ ������ �������, ���������������:
// - �������� ����� ��������� name
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

// - ������� ����������� ����� ������������ ����� ��������� surname
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

// - ����� ��� �������� � ���� date (� ��������� �� ���-�����-����)
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

// ���������� ���� � ���������� ������ �� �������
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
			std::cout << "=== ����� �� ������� ===\n";
			std::cout << "1. ����� �� ��������.\n";
			std::cout << "2. ����� �� ���� ����������.\n";
			std::cout << "3. ����� �� ������� �����������.\n";
			std::cout << "0. �����.\n";
			selected_option = GetIntFromConsole();

			switch (selected_option)
			{
			case 1:
				system("cls");
				std::cout << "������� �������� ������: ";
				user_str_input = HandleCyrillicInput();
				PrintProductsTable(GetProductsByName(user_str_input));
				continue;
			case 2:
				system("cls");
				std::cout << "������� ��� (2020-2022): ";
				date_input.wYear = GetIntFromConsoleWithBounds(2020, 2022);
				std::cout << "������� ����� (1-12): ";
				date_input.wMonth = GetIntFromConsoleWithBounds(1, 12);
				std::cout << "������� ���� (1-31): ";
				date_input.wDay = GetIntFromConsoleWithBounds(1, 31);
				PrintProductsTable(GetProductsByDate(date_input));
				continue;
			case 3:
				system("cls");
				std::cout << "������� �������: ";
				user_str_input = HandleCyrillicInput();
				PrintProductsTable(GetProductsByAdderSurname(user_str_input));
				continue;
			}
		} while (selected_option != 0);
	}
	else
	{
		std::cout << "������� ������� �� ������ ������ �����." << std::endl;
		system("pause");
	}
}

// ���������� ���� � ���������� ���������� �������
void ShowSortingOptionsMenu()
{
	system("cls");
	if (!products.empty())
	{
		int selected_option = 0;
		do
		{
			system("cls");
			std::cout << "=== ����������� ������ ===\n";
			std::cout << "1. ����������� �� ��������.\n";
			std::cout << "2. ����������� �� ����.\n";
			std::cout << "3. ����������� �� ���� ����������.\n";
			std::cout << "0. �����.\n";
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
		std::cout << "������� ������� �� ������ ������ �����." << std::endl;
		system("pause");
	}
}

// ���������� ������ ������� �������� ���������� ��������������� �������:
// ������� � ���������� ������� ������ �������,
// ���������� ����� x �������,
// ��������� ������� ��������� y ������(x, y �������� � ����������)
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

// ���������� ����� ����, ��������� ��� ������������
void ShowUserMenu()
{
	int selected_option = 0;
	do
	{
		system("cls");
		std::cout << "=== ���� ������������ ===\n";
		std::cout << "1. ������� ������ � �������.\n";
		std::cout << "2. �������� ������ � ������.\n";
		std::cout << "3. ����������.\n";
		std::cout << "4. ����� �� �������.\n";
		std::cout << "5. �������������� �������.\n";
		std::cout << "0. �����.\n";
		std::cout << "�������� �����: ";
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

// ���������� ����� ����, ��������� ��� ��������������
void ShowAdminMenu()
{
	int selected_option = 0;
	do
	{
		system("cls");
		std::cout << "=== ���� �������������� ===\n";
		std::cout << "1. �������� ������� ������.\n";
		std::cout << "2. ������� ������� ������ �������������.\n";
		std::cout << "3. ������������� ������� ������ ������������.\n";
		std::cout << "4. ������� ������� ������ ������������.\n";
		std::cout << "5. �������� ������ � ������.\n";
		std::cout << "6. ������� ������ � �������.\n";
		std::cout << "7. ������������� ������ � �������.\n";
		std::cout << "8. ������� �����.\n";
		std::cout << "9. ����������.\n";
		std::cout << "10. ����� �� �������.\n";
		std::cout << "11. �������������� �������.\n";
		std::cout << "0. �����.\n";
		std::cout << "�������� �����: ";
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

// ��������� ��������� ������������� � ����� ��� �����������
// ������ �, ���� ����� ������������ � ��������������� �������
// ������� ������, ���������� ������������ � ���������� true
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

// ��������� ������ ������������� � �� ������ ������� �� �����
// ��� ����������, ��� �� ������� ������� ����
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
		std::cout << "�� ������� ������� ���� � ������� �������������." << std::endl;
		system("pause");
	}
}

// ��������� ������ � ������� �� �����
// ��� ����������, ��� �� ������� ������� ����
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
		std::cout << "�� ������� ������� ���� � ������� � �������." << std::endl;
		system("pause");
	}
}

// ��������� ��������� �� ����� � ���������� � �������� ������ ��������� ������ ������� � ����
// ��� ���������� � ������������� ������� ����
void SaveUsersToFile()
{
	std::ofstream ofs;
	ofs.open(USERS_CREDENTIALS_FILENAME);
	if (ofs.is_open())
	{
		for(int i = 0; i < users.size(); i++)
		{
			const UserInfo& user_info = users[i];
			// TODO: ���������� �������
			ofs << user_info.auth_info.login << " " << user_info.auth_info.password << " " << user_info.role << " "
			<< user_info.fio.name << " " << user_info.fio.surname << " " << user_info.fio.middle_name << std::endl;
		}
	} else
	{
		std::cout << "�� ������� ������� ���� ��� ���������� ������ �������������." << std::endl;
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
			// TODO: ���������� �������
			ofs << product.name << " " << product.count << " " << product.price << " "
				<< time.wYear << " " << time.wMonth << " " << time.wDay << " " << time.wHour << " " << time.wMinute << " " << time.wSecond << " "
				<< fio.name << " " << fio.surname << " " << fio.middle_name << std::endl;
		}
	}
	else
	{
		std::cout << "�� ������� ������� ���� ��� ���������� ������ � �������." << std::endl;
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