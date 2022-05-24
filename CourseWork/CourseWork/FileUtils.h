#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>

#include "Model.h"

// Названия служебных файлов
const std::string USERS_CREDENTIALS_FILENAME = "users.txt";
const std::string PRODUCTS_DATA_FILENAME = "products.txt";

// Считывает данные пользователей и их уровни доступа из файла
// или уведомляет, что не удалось открыть файл
void ReadUsersFromFile();

// Считывает данные о товарах из файла
// или уведомляет, что не удалось открыть файл
void ReadProductsFromFile();

// Сохраняет считанные из файла и измененные в процессе работы программы данные обратно в файл
// или уведомляет о невозможности открыть файл
void SaveUsersToFile();

void SaveProductsToFile();