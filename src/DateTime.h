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

#ifndef _DATETIME_H_
#define _DATETIME_H_

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <stdint.h>
#include <time.h>
#include <string>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class DateTime {

private:
	time_t timeValue;

public:
	DateTime();
	virtual ~DateTime();

	bool ParseGmtIso8601(std::string timeString);
	std::string FormatGmtIso8601();
	void SetToCurrentTime();

	uint32_t GetLocalYear();
	uint32_t GetLocalMonth();
	std::string GetLocalMonthName();
	uint32_t GetLocalDay();
	std::string GetLocalDayName();
	uint32_t GetLocalHour();
	uint32_t GetLocalMinute();

	uint32_t GetGmtYear();
	uint32_t GetGmtMonth();
	std::string GetGmtMonthName();
	uint32_t GetGmtDay();
	std::string GetGmtDayName();
	uint32_t GetGmtHour();
	uint32_t GetGmtMinute();

	void AddHours(int hours);
	bool IsLaterThan(DateTime &dateTime);

	uint32_t GetTimeCode();
	void SetTimeCode(uint32_t code);

private:
	std::string GetMonthName(uint32_t month);
	std::string GetDayName(uint32_t day);
};

#endif /* _DATETIME_H_ */
