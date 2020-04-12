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

#ifndef _FORECAST_H_
#define _FORECAST_H_

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <stdint.h>
#include <string>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

#define NB_VALUES_PER_WEATHER_DATA 8

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

typedef struct
{
	float windDirectionDeg;
	float windSpeedKt;
	float gustSpeedKt;
	float lowCloudCoverPer;
	float midCloudCoverPer;
	float highCloudCoverPer;
	float precipitationMmH;
	float TemperatureC;
} WeatherData;

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class Forecast
{
private:
	std::string providerName;
	std::string modelName;
	uint32_t runTimeCode;
	uint16_t timeStepInHours;
	uint16_t numberOfSteps;
	WeatherData *forecastData;

public:
	Forecast();
	virtual ~Forecast();
	Forecast(const Forecast&);
	Forecast& operator=(const Forecast&);

	void Create(int numberOfSteps);
	void Clear();

	WeatherData GetForecastData(uint32_t index);

	uint32_t GetBinarySize();
	uint16_t GetNumberOfSteps();

	uint32_t WriteBinary(void *buffer);
	uint32_t ReadBinary(void *buffer);

	void SetProviderName(std::string &providerName);
	void SetModelName(std::string &modelName);
	void SetRunTimeCode(uint32_t runTimeCode);
	void SetTimeStepInHours(uint32_t timeStepInHours);

	std::string GetModelName();
	std::string GetProviderName();
	uint32_t GetRunTimeCode();
	uint32_t GetTimeStepInHours();

private:
	float GetMinWindDirectionDeg();
	float GetMaxWindDirectionDeg();
	float GetMinWindSpeedKt();
	float GetMaxWindSpeedKt();
	float GetMinGustSpeedKt();
	float GetMaxGustSpeedKt();
	float GetMinLowCloudCoverPer();
	float GetMaxLowCloudCoverPer();
	float GetMinMidCloudCoverPer();
	float GetMaxMidCloudCoverPer();
	float GetMinHighCloudCoverPer();
	float GetMaxHighCloudCoverPer();
	float GetMinPrecipitationMmH();
	float GetMaxPrecipitationMmH();
	float GetMinTemperatureC();
	float GetMaxTemperatureC();
};

#endif /* _FORECAST_H_ */
