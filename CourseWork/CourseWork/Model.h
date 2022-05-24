#pragma once

#include <string>
#include <Windows.h>

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