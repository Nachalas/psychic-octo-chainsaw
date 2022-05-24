#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
#include <Windows.h>

#include "TimeUtils.h"

struct ProductEntry;

// ���������� ������ ������� �������� ���������� ��������������� �������:
// ������� � ���������� ������� ������ �������,
// ���������� ����� x �������,
// ��������� ������� ��������� y ������(x, y �������� � ����������)
std::vector<ProductEntry> GetProductsByDurationAndPrice(int min_months, double min_price);

// �������, ������������ ������ �������, ���������������:
// - �������� ����� ��������� name
std::vector<ProductEntry> GetProductsByName(const std::string& name);

// - ������� ����������� ����� ������������ ����� ��������� surname
std::vector<ProductEntry> GetProductsByAdderSurname(const std::string& surname);

// - ����� ��� �������� � ���� date (� ��������� �� ���-�����-����)
std::vector<ProductEntry> GetProductsByDate(SYSTEMTIME date);
// -------

// �������, ������������ ������ �������, ��������������� �� ��������:
// - �� �����
std::vector<ProductEntry> GetProductsSortedByName();

// - �� ����
std::vector<ProductEntry> GetProductsSortedByPrice();

// - �� ���� ����������
std::vector<ProductEntry> GetProductsSortedByDate();
// -------