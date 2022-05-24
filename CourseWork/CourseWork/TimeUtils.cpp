#include "TimeUtils.h"

bool IsTimeLess(SYSTEMTIME lhs, SYSTEMTIME rhs)
{
	return std::tie(lhs.wYear, lhs.wMonth, lhs.wDay, lhs.wHour, lhs.wMinute, lhs.wSecond)
		< std::tie(rhs.wYear, rhs.wMonth, rhs.wDay, rhs.wHour, rhs.wMinute, rhs.wSecond);
}

time_t system2utc(const SYSTEMTIME& time)
{
	struct tm stm = { 0 };
	stm.tm_sec = time.wSecond;
	stm.tm_min = time.wMinute;
	stm.tm_hour = time.wHour;
	stm.tm_mday = time.wDay;
	stm.tm_mon = time.wMonth - 1;
	stm.tm_year = time.wYear - 1900;
	stm.tm_wday = time.wDayOfWeek;
	stm.tm_yday = 0;
	stm.tm_isdst = 0;

	return mktime(&stm);
}

bool isNMonthsBeforeNow(SYSTEMTIME time, int months)
{
	SYSTEMTIME now;
	GetLocalTime(&now);
	int monthsBefore = (system2utc(now) - system2utc(time)) / 2678400;
	return monthsBefore >= months;
}

std::string GetTimeString(SYSTEMTIME time)
{
	return (time.wDay < 10 ? "0" : "") + std::to_string(time.wDay) + "/"
		+ (time.wMonth < 10 ? "0" : "") + std::to_string(time.wMonth) + "/"
		+ std::to_string(time.wYear) + " "
		+ (time.wHour < 10 ? "0" : "") + std::to_string(time.wHour) + ":"
		+ (time.wMinute < 10 ? "0" : "") + std::to_string(time.wMinute) + ":"
		+ (time.wSecond < 10 ? "0" : "") + std::to_string(time.wSecond);

}