#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "ConsoleUtils.h"
#include "TimeUtils.h"
#include "FIOUtils.h"

// ���� ��������, �������� ������ �������� ������ � PrintUserEntry � PrintUserTableHeader
const int USER_NUMBER_COLUMN_WIDTH = 5;
const int USER_ROLE_COLUMN_WIDTH = 5;
const int USER_LOGIN_COLUMN_WIDTH = 20;
const int USER_PASSWORD_COLUMN_WIDTH = 30;
const int USER_SURNAME_COLUMN_WIDTH = 30;
const int USER_NAME_COLUMN_WIDTH = 30;
const int USER_MIDDLE_NAME_COLUMN_WIDTH = 30;

//  ���� ��������, �������� ������ �������� ������ � PrintProductEntry � PrintProductTableHeader
const int PRODUCT_NUMBER_COLUMN_WIDTH = 5;
const int PRODUCT_NAME_COLUMN_WIDTH = 20;
const int PRODUCT_COUNT_COLUMN_WIDTH = 15;
const int PRODUCT_PRICE_COLUMN_WIDTH = 15;
const int PRODUCT_DATE_COLUMN_WIDTH = 20;
const int PRODUCT_FIO_COLUMN_WIDTH = 62;

struct UserInfo;
struct ProductEntry;

// ������� ��� ������ ������� � ������� � �������������
void PrintUsersTable();

// ������� ��� ������ ������� � ������� � �������
void PrintProductsTable(const std::vector<ProductEntry>& products_vector);

// �������, ���������� �� ����� � ������� ���������� ������� ������� �������������
void PrintUserTableHeader();

// �������, ���������� �� ����� � ������� ���������� ������� ������� �������
void PrintProductTableHeader();

// ������� ������ � ������� ������������ user
void PrintUserEntry(int index, const UserInfo& user);

// ������� ������ � ������� � ������ product
void PrintProductEntry(int index, const ProductEntry& product);

// ������� ������ ������������ (����������� ��� �������������� � ��������)
int SelectUserRow();

// ������� ������ ������ (����������� ��� �������������� � ��������)
int SelectProductRow();