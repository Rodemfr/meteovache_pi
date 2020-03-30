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

#ifndef _SPOTFORECASTS_H_
#define _SPOTFORECASTS_H_

#include <stdint.h>
#include <Forecast.h>
#include <vector>
#include <wx/thread.h>

class SpotForecasts {
public:
	SpotForecasts();
	virtual ~SpotForecasts();

	void Lock();
	void Unlock();
	void Reset();
	Forecast &Get(uint32_t i);
	void Add(Forecast &i);
	void SetPosition(float latitudeDeg, float longitudeDeg);
	float GetLatitude();
	float GetLongitude();
	uint32_t GetNumberOfForecast();

private:
	float latitudeDeg;
	float longitudeDeg;
	std::vector<Forecast> forecastList;
	wxCriticalSection locker;
};

#endif /* _SPOTFORECASTS_H_ */
