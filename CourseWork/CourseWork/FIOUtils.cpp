#include "FIOUtils.h"

// получить фио в формате строки для вывода в таблице
std::string GetFIOString(const FIO& fio)
{
	return fio.surname + " " + fio.name + " " + fio.middle_name;
}