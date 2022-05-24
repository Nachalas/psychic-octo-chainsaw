#include "Menu.h"

#include "Global.h"

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
		switch (selected_index)
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