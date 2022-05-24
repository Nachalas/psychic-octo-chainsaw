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

// ���������� ����� ����, ��������� ��� ������������
void ShowUserMenu();

// ���������� ����� ����, ��������� ��� ��������������
void ShowAdminMenu();

// ���������� ���� � ���������� ���������� �������
void ShowSortingOptionsMenu();

// ���������� ���� � ������� ����� ������������, ��������� � ��������������
void ShowEditUserMenu(UserInfo& user);

// ���������� ���� � ������� ����� ������, ��������� � ��������������
void ShowEditProductMenu(ProductEntry& product);

// ���������� ���� � ���������� ������ �� �������
void ShowSearchOptionsMenu();

// ������� ������� � ������� ���� � ���������� �����������,
// ���������� ��������� ������������� �����
int ShowAuthMenu();