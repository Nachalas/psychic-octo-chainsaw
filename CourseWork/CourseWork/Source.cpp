#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
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
	std::string surname;
	std::string name;
	std::string middle_name;
};

std::string GetFIOString(const FIO& fio)
{
	return fio.surname + " " + fio.name + " " + fio.middle_name;
}

struct UserInfo
{
	AuthInfo auth_info;
	FIO fio;
};

struct ProductEntry
{
	std::string name; // ������������ ������
	int count; // ���������� ������ �� ������
	double price; // ���� �� ���� �����
	SYSTEMTIME time; // ����� ���������� ������
	FIO added_by; // ��� ������������������� �����
};

bool IsTimeLess(SYSTEMTIME lhs, SYSTEMTIME rhs)
{
	return std::tie(lhs.wMonth, lhs.wDay, lhs.wHour, lhs.wMinute, lhs.wSecond)
	< std::tie(rhs.wMonth, rhs.wDay, rhs.wHour, rhs.wMinute, rhs.wSecond);
}

std::string GetTimeString(SYSTEMTIME time)
{
	return (time.wDay < 10 ? "0" : "") + std::to_string(time.wDay) + "/"
		+ (time.wMonth < 10 ? "0" : "") + std::to_string(time.wMonth) + " "
		+ (time.wHour < 10 ? "0" : "") + std::to_string(time.wHour) + ":"
		+ (time.wMinute < 10 ? "0" : "") + std::to_string(time.wMinute) + ":"
		+ (time.wSecond < 10 ? "0" : "") + std::to_string(time.wSecond);
	
}

// ������������ ����� ��� ����� ����� � ������, ���
const int MAX_LOGIN_LENGTH = 12;
const int MAX_PASSWORD_LENGTH = 12;
const int MAX_USER_INPUT_LENGTH = 20;
// �������� ��������� ������
const std::string ADMIN_CREDENTIALS_FILENAME = "admin.txt";
const std::string USERS_CREDENTIALS_FILENAME = "users.txt";
const std::string PRODUCTS_DATA_FILENAME = "products.txt";
// ������, ���������� ������ � �������������
std::vector<UserInfo> users;
// ������, ���������� ������ � �������
std::vector<ProductEntry> products;


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

SYSTEMTIME GetSystemtimeFromConsole()
{
	SYSTEMTIME new_time{};
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

void ShowUserMenu()
{
	
}

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

	users.push_back(user_info);
	std::cout << std::endl << "������ ������� ���������." << std::endl;
	system("pause");
}

//std::string name; // ������������ ������
//int count; // ���������� ������ �� ������
//double price; // ���� �� ���� �����
//SYSTEMTIME time; // ����� ���������� ������
//FIO added_by; // ��� ������������������� �����
void ShowAddNewProductForm() {
	system("cls");
	ProductEntry product;
	std::cout << "=== ���������� ������ ������ ===" << std::endl;
	std::cout << "������� �������� ������: ";
	product.name = HandleCyrillicInput();
	std::cout << std::endl << "������� ���������� ������ �� ������: ";
	product.count = GetIntFromConsole();
	std::cout << "������� ���� �� �����: ";
	product.price = GetDoubleFromConsole();
	GetLocalTime(&product.time);
	product.added_by.name = "�������";
	product.added_by.surname = "��������";
	product.added_by.middle_name = "�����������";

	products.push_back(product);
	std::cout << std::endl << "������ ������� ���������." << std::endl;
	system("pause");
}

void PrintUserTableHeader()
{
	std::cout << "_______________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(5) << "#" << "|"
		<< std::setw(20) << "�����" << "|"
		<< std::setw(30) << "������" << "|"
		<< std::setw(30) << "�������" << "|"
		<< std::setw(30) << "���" << "|"
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

void PrintProductTableHeader()
{
	std::cout << "__________________________________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(5) << "#" << "|"
		<< std::setw(20) << "��������" << "|"
		<< std::setw(15) << "����������" << "|"
		<< std::setw(15) << "����" << "|"
		<< std::setw(15) << "����" << "|"
		<< std::setw(62) << "��� ����������� ������������" << "|"
		<< std::endl;
	std::cout << "|_____|____________________|_______________|_______________|_______________|______________________________________________________________" << std::endl;
}

void PrintProductEntry(int index, const ProductEntry& product)
{
	std::cout << "|" << std::fixed
		<< std::setw(5) << index << "|"
		<< std::setw(20) << product.name << "|"
		<< std::setw(15) << product.count << "|"
		<< std::setw(15) << std::setprecision(2) << product.price << "|"
		<< std::setw(15) << GetTimeString(product.time) << "|"
		<< std::setw(62) << GetFIOString(product.added_by) << "|"
		<< std::endl;
	std::cout << "|_____|____________________|_______________|_______________|_______________|______________________________________________________________|" << std::endl;
}

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

void PrintProductsTable()
{
	system("cls");
	std::cout << "=== ������ ===" << std::endl;

	if (!products.empty())
	{
		PrintProductTableHeader();
		for (int i = 0; i < products.size(); i++)
		{
			PrintProductEntry(i + 1, products[i]);
		}
	}
	else
	{
		std::cout << "�� ������ ������ ��� �������." << std::endl;
	}

	system("pause");
}

int SelectUserRow()
{
	std::cout << "������� ������ ������������: ";
	return GetIntFromConsole();
}

int SelectProductRow()
{
	std::cout << "������� ������ ��������: ";
	return GetIntFromConsole();
}

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

void DeleteUserEntry()
{
	system("cls");
	if(!users.empty())
	{
		std::cout << "=== �������� ������������ ===" << std::endl;
		int selectedIndex = SelectUserRow();
		if (selectedIndex >= 0 && selectedIndex < users.size())
		{
			users.erase(users.begin() + selectedIndex);
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
			ShowAddNewProductForm();
			continue;
		case 6:
			PrintProductsTable();
			continue;
		case 7:
			EditProductEntry();
		case 8:
			DeleteProductEntry();
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
		std::cout << "��������� ������ ��� �������� �����." << std::endl;
	}
	return found_credentials;
}

bool CheckUserCredentials(const AuthInfo& form)
{
	bool found_credentials = false;
	auto it = std::find_if(users.begin(), users.end(), [form](const UserInfo& user)
	{
			return user.auth_info.login == form.login;
	});
	if(it != users.end())
	{
		if(it->auth_info.password == form.password)
		{
			found_credentials = true;
		}
	}
	return found_credentials;
}

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
			oss >> user_info.auth_info.login >> user_info.auth_info.password >> user_info.fio.name >> user_info.fio.surname >> user_info.fio.middle_name;
			users.push_back(user_info);
		}
	} else
	{
		std::cout << "�� ������� ������� ���� � ������� �������������." << std::endl;
		system("pause");
	}
}

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
				>> time.wMonth >> time.wDay >> time.wHour >> time.wMinute >> time.wSecond
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
			ofs << user_info.auth_info.login << " " << user_info.auth_info.password << " " << user_info.fio.name << " " << user_info.fio.surname << " " << user_info.fio.middle_name << std::endl;
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
				<< time.wMonth << " " << time.wDay << " " << time.wHour << " " << time.wMinute << " " << time.wSecond << " "
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
			if (CheckAdminCredentials(form_info))
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
			if(CheckUserCredentials(form_info))
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