#include "FIOUtils.h"

// �������� ��� � ������� ������ ��� ������ � �������
std::string GetFIOString(const FIO& fio)
{
	return fio.surname + " " + fio.name + " " + fio.middle_name;
}