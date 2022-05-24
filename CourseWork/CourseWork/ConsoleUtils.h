#pragma once

#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "Model.h"

#undef min
#undef max

// ������������ ����� ��� ����� ����� � ������, ���
const int MAX_LOGIN_LENGTH = 12;
const int MAX_PASSWORD_LENGTH = 12;
const int MAX_USER_INPUT_LENGTH = 20;

// ��������� ����� ���� int �� �������, ��� �������� ������� ������ �������� ������� ����� ������
int GetIntFromConsole();

// ������ GetIntFromConsole(), �� � �������������� ��������� �� ��������� � ��������
int GetIntFromConsoleWithBounds(int left, int right);

// ��������� ����� ���� double �� �������, ��� �������� ������� ������ �������� ������� ����� ������
double GetDoubleFromConsole();

// �������, �������������� ����� ������ �������������: ���������� �������� ������� �
// ������ �� ������ �������� �����. ������� ��� ����� ������ �������� ��������� *
// ��������� ������
std::string HandleLoginInput();
std::string HandlePasswordInput();
std::string HandleCyrillicInput();
// -------

// ������� ��� �������� ���������� ��������� ������������� ��������
bool IsLoginCharInvalid(char c);
bool IsPasswordCharInvalid(char c);
bool IsCyrillicCharInvalid(char c);
// ------------

// ������� ��������������� ������������ ����������� ������� ���� � �������
// � ��������� ������ �� �����
SYSTEMTIME GetSystemtimeFromConsole();

// ������� ��� �������� �������, �����, �������� �� �������
FIO GetFIOFromConsole();

// ������� ���������� ����� ��� ����� ���������������� ������: ����� + ������
AuthInfo ShowAuthForm();