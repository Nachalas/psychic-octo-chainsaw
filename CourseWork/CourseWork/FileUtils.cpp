#include "FileUtils.h"

#include "Global.h"

void ReadUsersFromFile()
{
	std::ifstream ifs;
	ifs.open(USERS_CREDENTIALS_FILENAME);
	if (ifs.is_open())
	{
		std::string file_line;
		while (std::getline(ifs, file_line))
		{
			UserInfo user_info;
			std::istringstream oss(file_line);
			oss >> user_info.auth_info.login >> user_info.auth_info.password >> user_info.role >> user_info.fio.name >> user_info.fio.surname >> user_info.fio.middle_name;
			users.push_back(user_info);
		}
	}
	else
	{
		std::cout << "Не удалось открыть файл с данными пользователей." << std::endl;
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

void SaveUsersToFile()
{
	std::ofstream ofs;
	ofs.open(USERS_CREDENTIALS_FILENAME);
	if (ofs.is_open())
	{
		for (int i = 0; i < users.size(); i++)
		{
			const UserInfo& user_info = users[i];
			// TODO: нормальные колонки
			ofs << user_info.auth_info.login << " " << user_info.auth_info.password << " " << user_info.role << " "
				<< user_info.fio.name << " " << user_info.fio.surname << " " << user_info.fio.middle_name << std::endl;
		}
	}
	else
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