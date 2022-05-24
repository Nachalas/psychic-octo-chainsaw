#include "TableUtils.h"

#include "Global.h"

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
	}
	else
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

void PrintUserTableHeader()
{
	std::cout << "_____________________________________________________________________________________________________________________________________________________________" << std::endl;
	std::cout << "|" << std::fixed
		<< std::setw(USER_NUMBER_COLUMN_WIDTH) << "#" << "|"
		<< std::setw(USER_NUMBER_COLUMN_WIDTH) << "Роль" << "|"
		<< std::setw(USER_LOGIN_COLUMN_WIDTH) << "Логин" << "|"
		<< std::setw(USER_PASSWORD_COLUMN_WIDTH) << "Пароль" << "|"
		<< std::setw(USER_SURNAME_COLUMN_WIDTH) << "Фамилия" << "|"
		<< std::setw(USER_NAME_COLUMN_WIDTH) << "Имя" << "|"
		<< std::setw(USER_MIDDLE_NAME_COLUMN_WIDTH) << "Отчество" << "|"
		<< std::endl;
	std::cout << "|_____|_____|____________________|______________________________|______________________________|______________________________|______________________________|" << std::endl;
}

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

int SelectUserRow()
{
	std::cout << "Введите индекс пользователя: ";
	return GetIntFromConsole();
}

int SelectProductRow()
{
	std::cout << "Введите индекс продукта: ";
	return GetIntFromConsole();
}