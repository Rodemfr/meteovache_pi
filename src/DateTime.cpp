/***************************************************************************
 *                                                                         *
 * Project:  meteovache_pi                                                 *
 * Purpose:  Weather forecast plugin for OpenCPN                           *
 * Author:   Ronan Demoment                                                *
 *                                                                         *
 ***************************************************************************
 *   Copyright (C) 2020 by Ronan Demoment                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <DateTime.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <wx/translation.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

/***************************************************************************/
/*                             Local types                                 */
/***************************************************************************/

/***************************************************************************/
/*                               Globals                                   */
/***************************************************************************/

/***************************************************************************/
/*                           Local prototypes                              */
/***************************************************************************/

/***************************************************************************/
/*                              Functions                                  */
/***************************************************************************/

DateTime::DateTime() :
		timeValue(0)
{
}

DateTime::~DateTime()
{
}

bool DateTime::ParseGmtIso8601(std::string timeString)
{
	struct tm timeData;
	char *returnCode;

	memset(&timeData, 0, sizeof(struct tm));
#ifndef __WIN32__
	returnCode = strptime(timeString.c_str(), "%FT%T", &timeData);
	timeData.tm_isdst = 1;
	timeValue = timegm(&timeData);
#else
	sscanf(timeString.c_str(), "%d-%d-%dT%d%*c%d", &timeData.tm_year, &timeData.tm_mon, &timeData.tm_mday, &timeData.tm_hour, &timeData.tm_min);
	timeData.tm_year -= 1900;
	timeData.tm_mon -= 1;
	timeData.tm_isdst = 0;
	timeValue = _mkgmtime(&timeData);
	returnCode = (char*) 1;
#endif
	return (returnCode != NULL);
}

std::string DateTime::FormatGmtIso8601()
{
	struct tm *timeData;
	time_t tv = timeValue;
	char timeString[64];

	timeData = gmtime(&tv);
	strftime(timeString, sizeof(timeString), "%FT%TZ", timeData);

	return (std::string(timeString));
}

void DateTime::SetToCurrentTime()
{
	time_t tv;

	time(&tv);
	timeValue = tv;
}

uint32_t DateTime::GetLocalYear()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_year + 1900);
}

uint32_t DateTime::GetGmtYear()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_year + 1900);
}

uint32_t DateTime::GetLocalMonth()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_mon + 1);
}

uint32_t DateTime::GetGmtMonth()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_mon + 1);
}

std::string DateTime::GetLocalMonthName()
{
	struct tm *timeData;
	time_t tv = timeValue;
	std::string monthName;

	timeData = localtime(&tv);

	return GetMonthName(timeData->tm_mon);
}

std::string DateTime::GetGmtMonthName()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return GetMonthName(timeData->tm_mon);
}

uint32_t DateTime::GetLocalDay()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_mday);
}

uint32_t DateTime::GetGmtDay()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_mday);
}

std::string DateTime::GetLocalDayName()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return GetDayName(timeData->tm_wday);
}

std::string DateTime::GetGmtDayName()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return GetDayName(timeData->tm_wday);
}

uint32_t DateTime::GetLocalHour()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_hour);
}

uint32_t DateTime::GetGmtHour()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_hour);
}

uint32_t DateTime::GetLocalMinute()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = localtime(&tv);

	return (timeData->tm_min);
}

uint32_t DateTime::GetGmtMinute()
{
	struct tm *timeData;
	time_t tv = timeValue;

	timeData = gmtime(&tv);

	return (timeData->tm_min);
}

void DateTime::AddHours(int hours)
{
	timeValue += (3600 * hours);
}

bool DateTime::IsLaterThan(DateTime &dateTime)
{
	return (timeValue > dateTime.timeValue);
}

uint32_t DateTime::GetTimeCode()
{
	uint32_t timeCode;
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

void DateTime::SetTimeCode(uint32_t code)
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

std::string DateTime::GetMonthName(uint32_t month)
{
	switch (month)
	{
	case 0:
		return _("January").ToStdString();
		break;
	case 1:
		return _("February").ToStdString();
		break;
	case 2:
		return _("March").ToStdString();
		break;
	case 3:
		return _("April").ToStdString();
		break;
	case 4:
		return _("May").ToStdString();
		break;
	case 5:
		return _("June").ToStdString();
		break;
	case 6:
		return _("July").ToStdString();
		break;
	case 7:
		return _("August").ToStdString();
		break;
	case 8:
		return _("September").ToStdString();
		break;
	case 9:
		return _("October").ToStdString();
		break;
	case 10:
		return _("November").ToStdString();
		break;
	case 11:
		return _("December").ToStdString();
		break;
	}

	return (std::string(""));
}

std::string DateTime::GetDayName(uint32_t day)
{
	switch (day)
	{
	case 0:
		return _("Sunday").ToStdString();
		break;
	case 1:
		return _("Monday").ToStdString();
		break;
	case 2:
		return _("Tuesday").ToStdString();
		break;
	case 3:
		return _("Wednesday").ToStdString();
		break;
	case 4:
		return _("Thursday").ToStdString();
		break;
	case 5:
		return _("Friday").ToStdString();
		break;
	case 6:
		return _("Saturday").ToStdString();
		break;
	}

	return (std::string(""));
}

