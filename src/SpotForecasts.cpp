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

#include <SpotForecasts.h>

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

SpotForecasts::SpotForecasts() :
		latitudeDeg(0), longitudeDeg(0), forecastList()
{
}

SpotForecasts::~SpotForecasts()
{
}

void SpotForecasts::Lock()
{
	locker.Enter();
}

void SpotForecasts::Unlock()
{
	locker.Leave();
}

void SpotForecasts::Reset()
{
	forecastList.clear();
}

Forecast& SpotForecasts::Get(uint32_t i)
{
	return (forecastList.at(i));
}

void SpotForecasts::Add(Forecast &forecast)
{
	forecastList.push_back(forecast);
}

void SpotForecasts::SetPosition(float latitudeDeg, float longitudeDeg)
{
	this->latitudeDeg = latitudeDeg;
	this->longitudeDeg = longitudeDeg;
}

float SpotForecasts::GetLatitude()
{
	return (latitudeDeg);
}

float SpotForecasts::GetLongitude()
{
	return (longitudeDeg);
}

uint32_t SpotForecasts::GetNumberOfForecast()
{
	return (forecastList.size());
}
