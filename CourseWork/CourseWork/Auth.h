#pragma once

#include <algorithm>

#include "Model.h"

// ��������� ��������� ������������� � ����� ��� �����������
// ������ �, ���� ����� ������������ � ��������������� �������
// ������� ������, ���������� ������������ � ���������� true
bool CheckAdminCredentialsAndLogIn(const AuthInfo& form);

bool CheckUserCredentialsAndLogIn(const AuthInfo& form);
// --------
