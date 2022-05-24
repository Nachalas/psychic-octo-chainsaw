#pragma once

#include <string>
#include <Windows.h>

struct AuthInfo
{
	std::string login;
	std::string password;
};

struct FIO {
	std::string surname; // �������
	std::string name; // ���
	std::string middle_name; // ��������
};

struct UserInfo
{
	AuthInfo auth_info;
	FIO fio;
	int role; // 0 - ������������, 1 - �����
};

struct ProductEntry
{
	std::string name; // ������������ ������
	int count; // ���������� ������ �� ������
	double price; // ���� �� ���� �����
	SYSTEMTIME time; // ����� ���������� ������
	FIO added_by; // ��� ������������������� �����
};