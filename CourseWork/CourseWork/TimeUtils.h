#pragma once

#include <tuple>
#include <ctime>
#include <string>
#include <Windows.h>

// функция для сравнения двух дат
bool IsTimeLess(SYSTEMTIME lhs, SYSTEMTIME rhs);

// Конвертирует дату формата SYSTEMTIME в time_t
time_t system2utc(const SYSTEMTIME& time);

// Проверяет, прошло ли с даты time months месяцев
bool isNMonthsBeforeNow(SYSTEMTIME time, int months);

// получить дату в формате DD/MM/YYYY HH:MM
std::string GetTimeString(SYSTEMTIME time);
