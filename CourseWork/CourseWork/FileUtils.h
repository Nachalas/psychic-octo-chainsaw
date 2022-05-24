#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>

#include "Model.h"

// �������� ��������� ������
const std::string USERS_CREDENTIALS_FILENAME = "users.txt";
const std::string PRODUCTS_DATA_FILENAME = "products.txt";

// ��������� ������ ������������� � �� ������ ������� �� �����
// ��� ����������, ��� �� ������� ������� ����
void ReadUsersFromFile();

// ��������� ������ � ������� �� �����
// ��� ����������, ��� �� ������� ������� ����
void ReadProductsFromFile();

// ��������� ��������� �� ����� � ���������� � �������� ������ ��������� ������ ������� � ����
// ��� ���������� � ������������� ������� ����
void SaveUsersToFile();

void SaveProductsToFile();