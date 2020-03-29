#include "DateTime.h"

#include <time.h>
#include <string.h>
#include <stdio.h>

char *DateTime::dayNames[7] =
{ (char*) "Dimanche", (char*) "Lundi", (char*) "Mardi", (char*) "Mercredi",
		(char*) "Jeudi", (char*) "Vendredi", (char*) "Samedi" };
char *DateTime::monthNames[12] =
{ (char*) "Janvier", (char*) "Février", (char*) "Mars", (char*) "Avril",
		(char*) "Mai", (char*) "Juin", (char*) "Juillet", (char*) "Août",
		(char*) "Septembre", (char*) "Octobre", (char*) "Novembre",
		(char*) "Décembre" };

DateTime::DateTime() :
		timeValue(0)
{
}

DateTime::~DateTime()
{
}

char* DateTime::parseGmtIso8601(const char *timeString)
{
	struct tm timeData;
	char *returnCode;

	memset(&timeData, 0, sizeof(struct tm));
#ifndef __WIN32__
	returnCode = strptime(timeString, "%FT%T", &timeData);
	timeData.tm_isdst = 1;
	timeValue = timegm(&timeData);
#else
    sscanf(timeString, "%d-%d-%dT%d%*c%d", &timeData.tm_year,
           &timeData.tm_mon, &timeData.tm_mday, &timeData.tm_hour,
           &timeData.tm_min);
    timeData.tm_year -= 1900;
    timeData.tm_mon -= 1;
    timeData.tm_isdst = 0;
    timeValue = _mkgmtime(&timeData);
    returnCode = (char *)1;
#endif
	return (returnCode);
}

void DateTime::formatGmtIso8601(char *timeString, int maxLength)
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	strftime(timeString, maxLength, "%FT%TZ", timeData);
}

void DateTime::setToCurrentTime()
{
	time_t tv;

	time(&tv);
	timeValue = tv;
}

int DateTime::getLocalYear()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_year + 1900);
}

int DateTime::getGmtYear()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_year + 1900);
}

int DateTime::getLocalMonth()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_mon + 1);
}

int DateTime::getGmtMonth()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_mon + 1);
}

const char* DateTime::getLocalMonthName()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (monthNames[timeData->tm_mon]);
}

const char* DateTime::getGmtMonthName()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (monthNames[timeData->tm_mon]);
}

int DateTime::getLocalDay()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_mday);
}

int DateTime::getGmtDay()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_mday);
}

const char* DateTime::getLocalDayName()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (dayNames[timeData->tm_wday]);
}

const char* DateTime::getGmtDayName()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (dayNames[timeData->tm_wday]);
}

int DateTime::getLocalHour()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_hour);
}

int DateTime::getGmtHour()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_hour);
}

int DateTime::getLocalMinute()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_min);
}

int DateTime::getGmtMinute()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_min);
}

void DateTime::addHours(int hours)
{
	timeValue += (3600 * hours);
}

int DateTime::isLaterThan(DateTime *dateTime)
{
	return (timeValue > dateTime->timeValue);
}

unsigned int DateTime::getTimeCode()
{
	unsigned int timeCode;
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	timeCode = timeData->tm_year << 20;
	timeCode |= timeData->tm_mon << 16;
	timeCode |= timeData->tm_mday << 11;
	timeCode |= timeData->tm_hour << 6;
	timeCode |= timeData->tm_min;

	return (timeCode);
}

void DateTime::setTimeCode(unsigned int code)
{
	struct tm timeData;

	memset(&timeData, 0, sizeof(timeData));
	timeData.tm_year = code >> 20;
	timeData.tm_mon = (code >> 16) & 0x0f;
	timeData.tm_mday = (code >> 11) & 0x1f;
	timeData.tm_hour = (code >> 6) & 0x1f;
	timeData.tm_min = code & 0x3f;
	timeData.tm_isdst = 0;
#ifndef __WIN32__
	timeValue = timegm(&timeData);
#else
	timeValue = _mkgmtime(&timeData);
#endif
}
