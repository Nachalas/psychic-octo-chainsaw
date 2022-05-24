#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "ConsoleUtils.h"
#include "TimeUtils.h"
#include "FIOUtils.h"

// Блок констант, задающих ширину колонкам данных в PrintUserEntry и PrintUserTableHeader
const int USER_NUMBER_COLUMN_WIDTH = 5;
const int USER_ROLE_COLUMN_WIDTH = 5;
const int USER_LOGIN_COLUMN_WIDTH = 20;
const int USER_PASSWORD_COLUMN_WIDTH = 30;
const int USER_SURNAME_COLUMN_WIDTH = 30;
const int USER_NAME_COLUMN_WIDTH = 30;
const int USER_MIDDLE_NAME_COLUMN_WIDTH = 30;

//  Блок констант, задающих ширину колонкам данных в PrintProductEntry и PrintProductTableHeader
const int PRODUCT_NUMBER_COLUMN_WIDTH = 5;
const int PRODUCT_NAME_COLUMN_WIDTH = 20;
const int PRODUCT_COUNT_COLUMN_WIDTH = 15;
const int PRODUCT_PRICE_COLUMN_WIDTH = 15;
const int PRODUCT_DATE_COLUMN_WIDTH = 20;
const int PRODUCT_FIO_COLUMN_WIDTH = 62;

struct UserInfo;
struct ProductEntry;

// функция для печати таблицы с данными о пользователях
void PrintUsersTable();

// функция для печати таблицы с данными о товарах
void PrintProductsTable(const std::vector<ProductEntry>& products_vector);

// Функция, отвечающая за вывод в консоль заголовков колонок таблицы пользователей
void PrintUserTableHeader();

// Функция, отвечающая за вывод в консоль заголовков колонок таблицы товаров
void PrintProductTableHeader();

// Выводит строку с данными пользователя user
void PrintUserEntry(int index, const UserInfo& user);

// Выводит строку с данными о товаре product
void PrintProductEntry(int index, const ProductEntry& product);

// функция выбора пользователя (применяется при редактировании и удалении)
int SelectUserRow();

// функция выбора товара (применяется при редактировании и удалении)
int SelectProductRow();