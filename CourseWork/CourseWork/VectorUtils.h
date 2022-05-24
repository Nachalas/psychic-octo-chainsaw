#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
#include <Windows.h>

#include "TimeUtils.h"

struct ProductEntry;

// Возвращает вектор товаров согласно требований индивидуального задания:
// вывести в алфавитном порядке список товаров,
// хранящихся более x месяцев,
// стоимость которых превышает y рублей(x, y вводятся с клавиатуры)
std::vector<ProductEntry> GetProductsByDurationAndPrice(int min_months, double min_price);

// Функции, возвращающие вектор товаров, соответствующих:
// - название равно параметру name
std::vector<ProductEntry> GetProductsByName(const std::string& name);

// - фамилия добавившего товар пользователя равна параметру surname
std::vector<ProductEntry> GetProductsByAdderSurname(const std::string& surname);

// - товар был добавлен в дату date (с точностью до год-месяц-день)
std::vector<ProductEntry> GetProductsByDate(SYSTEMTIME date);
// -------

// Функции, возвращающие вектор товаров, отсортированных по признаку:
// - по имени
std::vector<ProductEntry> GetProductsSortedByName();

// - по цене
std::vector<ProductEntry> GetProductsSortedByPrice();

// - по дате добавления
std::vector<ProductEntry> GetProductsSortedByDate();
// -------