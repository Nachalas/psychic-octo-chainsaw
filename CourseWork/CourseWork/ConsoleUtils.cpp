#include "ConsoleUtils.h"

int GetIntFromConsole()
{
	int value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "¬веден неправильный символ. ¬ведите число: ";
	}
	return value;
}

int GetIntFromConsoleWithBounds(int left, int right)
{
	int value = left - 1;
	while (!(std::cin >> value) || !(value <= right && value >= left)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (!(value <= right && value >= left))
		{
			std::cout << "¬веденное число не входит в диапазон. ¬ведите число: ";
		}
		else
		{
			std::cout << "¬веден неправильный символ. ¬ведите число: ";
		}
	}
	return value;
}

double GetDoubleFromConsole()
{
	double value = 0;
	while (!(std::cin >> value)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "¬веден неправильный символ. ¬ведите число: ";
	}
	return value;
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

	while (true)
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
			if (!password_string.empty())
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

	while (true)
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

SYSTEMTIME GetSystemtimeFromConsole()
{
	SYSTEMTIME new_time{};
	std::cout << "¬ведите год (2020-2022): ";
	new_time.wYear = GetIntFromConsoleWithBounds(2020, 2022);
	std::cout << "¬ведите мес€ц (1-12): ";
	new_time.wMonth = GetIntFromConsoleWithBounds(1, 12);
	std::cout << "¬ведите день (1-31): ";
	new_time.wDay = GetIntFromConsoleWithBounds(1, 31);
	std::cout << "¬ведите час (1-24): ";
	new_time.wHour = GetIntFromConsoleWithBounds(0, 23);
	std::cout << "¬ведите минуты (0-59): ";
	new_time.wMinute = GetIntFromConsoleWithBounds(0, 59);
	//std::cout << "¬ведите секунды (0-59): ";
	//new_time.wSecond = GetIntFromConsoleWithBounds(0, 59);
	new_time.wSecond = 0;
	return new_time;
}

FIO GetFIOFromConsole()
{
	FIO new_fio;
	std::cout << "¬ведите новую фамилию: ";
	new_fio.surname = HandleCyrillicInput();
	std::cout << std::endl << "¬ведите новое им€: ";
	new_fio.name = HandleCyrillicInput();
	std::cout << std::endl << "¬ведите новое отчество: ";
	new_fio.middle_name = HandleCyrillicInput();
	return new_fio;
}

AuthInfo ShowAuthForm()
{
	AuthInfo form_info;

	system("cls");

	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursor_position = { 0,1 };

	std::cout << "ѕожалуйста, введите данные:" << std::endl;
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