#pragma once

#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "Model.h"

#undef min
#undef max

// Максимальные длины для полей логин и пароль, ФИО
const int MAX_LOGIN_LENGTH = 12;
const int MAX_PASSWORD_LENGTH = 12;
const int MAX_USER_INPUT_LENGTH = 20;

// Считывает число типа int из консоли, при неверных входных данных пытается считать число заново
int GetIntFromConsole();

// Аналог GetIntFromConsole(), но с дополнительной проверкой на вхождение в диапазон
int GetIntFromConsoleWithBounds(int left, int right);

// Считывает число типа double из консоли, при неверных входных данных пытается считать число заново
double GetDoubleFromConsole();

// Функции, контролирующие набор данных пользователем: валидируют вводимые символы и
// следят за длиной вводимых строк. Функция для ввода пароля скрывает символами *
// введенные данные
std::string HandleLoginInput();
std::string HandlePasswordInput();
std::string HandleCyrillicInput();
// -------

// Функции для проверки валидности введенных пользователем символов
bool IsLoginCharInvalid(char c);
bool IsPasswordCharInvalid(char c);
bool IsCyrillicCharInvalid(char c);
// ------------

// Функция предоставляющая пользователю возможность вводить дату с консоли
// с точностью вплоть до минут
SYSTEMTIME GetSystemtimeFromConsole();

// Функция для введения фамилии, имени, отчества из консоли
FIO GetFIOFromConsole();

// Функция показывает форму для ввода пользовательских данных: логин + пароль
AuthInfo ShowAuthForm();