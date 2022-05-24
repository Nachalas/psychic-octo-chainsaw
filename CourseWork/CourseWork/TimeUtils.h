#pragma once

#include <tuple>
#include <ctime>
#include <string>
#include <Windows.h>

// ������� ��� ��������� ���� ���
bool IsTimeLess(SYSTEMTIME lhs, SYSTEMTIME rhs);

// ������������ ���� ������� SYSTEMTIME � time_t
time_t system2utc(const SYSTEMTIME& time);

// ���������, ������ �� � ���� time months �������
bool isNMonthsBeforeNow(SYSTEMTIME time, int months);

// �������� ���� � ������� DD/MM/YYYY HH:MM
std::string GetTimeString(SYSTEMTIME time);
