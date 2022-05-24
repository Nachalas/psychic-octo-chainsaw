#pragma once

#include <cstdlib>
#include <iostream>

#include "ConsoleUtils.h"
#include "TableUtils.h"
#include "VectorUtils.h"
#include "DataAccessUtils.h"
#include "Source.h"

struct UserInfo;
struct ProductEntry;

// ѕоказывает опции меню, доступные дл€ пользовател€
void ShowUserMenu();

// ѕоказывает опции меню, доступные дл€ администратора
void ShowAdminMenu();

// ѕоказывает меню с вариантами сортировки товаров
void ShowSortingOptionsMenu();

// ѕоказывает меню с выбором полей пользовател€, доступных к редактированию
void ShowEditUserMenu(UserInfo& user);

// ѕоказывает меню с выбором полей товара, доступных к редактированию
void ShowEditProductMenu(ProductEntry& product);

// ѕоказывает меню с вариантами поиска по товарам
void ShowSearchOptionsMenu();

// ‘ункци€ выводит в консоль меню с вариантами авторизации,
// возвращает выбранный пользователем пункт
int ShowAuthMenu();