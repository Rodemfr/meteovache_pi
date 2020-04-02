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

#include <Forecast.h>
#include <string>
#include <string.h>

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

// Constructor
Forecast::Forecast() : runTimeCode(0), timeStepInHours(1), numberOfSteps(0), forecastData(nullptr)
{
}

// Destructor
Forecast::~Forecast()
{
	// Just delete everything that may have been allocated during object lifetime
    if (forecastData != nullptr)
    {
        delete [] forecastData;
    }
}

// Copy operator
Forecast &Forecast::operator=(const Forecast &spot)
{
    // Check if we are assigning object to itself
    if (this != &spot)
    {
    	// Copy every field
        providerName = spot.providerName;
        modelName = spot.modelName;

        runTimeCode = spot.runTimeCode;
        timeStepInHours = spot.timeStepInHours;
        numberOfSteps = spot.numberOfSteps;
        forecastData = spot.forecastData;

        if (forecastData != 0)
        {
        	forecastData = new WeatherData[numberOfSteps];
            memcpy(forecastData, spot.forecastData, numberOfSteps * sizeof(WeatherData));
        }
    }

    return (*this);
}

// Copy constructor
Forecast::Forecast(const Forecast &spot) : runTimeCode(spot.runTimeCode), timeStepInHours(spot.timeStepInHours), numberOfSteps(spot.numberOfSteps), forecastData(0)
{
	// Copy every field
    providerName = spot.providerName;
    modelName = spot.modelName;

    if (spot.forecastData != 0)
    {
    	forecastData = new WeatherData[numberOfSteps];
        memcpy(forecastData, spot.forecastData, numberOfSteps * sizeof(WeatherData));
    }
}

// Create a new forecast array
//
// [in] prediction_length
//      Number of predictions in the forecast array
void Forecast::Create(int prediction_length)
{
	// TODO : This function will be made obsolete by usage of std::vector for forecast data
    this->Clear();

    if (prediction_length > 0)
    {
    	forecastData = new WeatherData[prediction_length];
        this->numberOfSteps = prediction_length;
    }
}

void Forecast::Clear()
{
    if (forecastData != 0)
    {
        delete [] forecastData;
        forecastData = 0;
    }
    numberOfSteps = 0;
}

WeatherData Forecast::getForecastData(uint32_t index)
{
    return(forecastData[index]);
}

uint32_t Forecast::getBinarySize()
{
    uint32_t binarySize = 0;

    binarySize += providerName.length() + 1;
    binarySize += modelName.length() + 1;
    binarySize += sizeof(runTimeCode);
    binarySize += sizeof(timeStepInHours);
    binarySize += sizeof(numberOfSteps);
    binarySize += sizeof(char) * numberOfSteps * NB_VALUES_PER_WEATHER_DATA;

    return(binarySize);
}

uint16_t Forecast::getNumberOfSteps()
{
    return(numberOfSteps);
}

uint32_t Forecast::writeBinary(void *outputBuffer)
{
    char *pOutput = (char *)outputBuffer;
    uint32_t index;

    strcpy((char *)pOutput, providerName.c_str());
    pOutput += providerName.length() + 1;

    strcpy((char *)pOutput, modelName.c_str());
    pOutput += modelName.length() + 1;

    *((uint32_t *)pOutput) = runTimeCode;
    pOutput += sizeof(uint32_t);

    *((uint16_t *)pOutput) = timeStepInHours;
    pOutput += sizeof(uint16_t);

    *((uint16_t *)pOutput) = numberOfSteps;
    pOutput += sizeof(uint16_t);

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)((forecastData[index].windSpeedKt / 1.94384f) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)((forecastData[index].windDirectionDeg / 2) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)((forecastData[index].gustSpeedKt / 1.94384f) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)(forecastData[index].lowCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)(forecastData[index].midCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)(forecastData[index].highCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)((forecastData[index].precipitationMmH * 10.0f) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int32_t)(forecastData[index].TemperatureC + 0.5f);
    }

    return(pOutput - (char *)outputBuffer);
}

uint32_t Forecast::readBinary(void *inputBuffer)
{
    unsigned char *pInput = (unsigned char *)inputBuffer;
    uint32_t index;

    this->Clear();

    providerName = std::string((char *)pInput);
    pInput += strlen((char *)pInput) + 1;

    modelName = std::string((char *)pInput);
    pInput += strlen((char *)pInput) + 1;

    runTimeCode = *((uint32_t *)pInput);
    pInput += sizeof(uint32_t);

    timeStepInHours = *((uint16_t *)pInput);
    pInput += sizeof(uint16_t);

    numberOfSteps = *((uint16_t *)pInput);
    pInput += sizeof(uint16_t);

    if (numberOfSteps > 0)
    {
    	forecastData = new WeatherData[numberOfSteps];
    }
    else
    {
        return(0);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].windSpeedKt = *(pInput++) * 1.94384f;
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].windDirectionDeg = (*((char *)pInput)) * 2.0f;
    	pInput++;
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].gustSpeedKt = *(pInput++) * 1.94384f;
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].lowCloudCoverPer = *(pInput++);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].midCloudCoverPer = *(pInput++);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].highCloudCoverPer = *(pInput++);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].precipitationMmH = *(pInput++) / 10.0f;
    }

    for (index = 0; index < numberOfSteps; index++)
    {
    	forecastData[index].TemperatureC = (*((char *)(pInput++)));
    }

    return(pInput - (unsigned char *)inputBuffer);
}

void Forecast::setProviderName(std::string &model_provider)
{
    this->providerName = model_provider;
}

void Forecast::setModelName(std::string &model_name)
{
    this->modelName = model_name;
}

void Forecast::setRunTimeCode(uint32_t model_run)
{
    this->runTimeCode = model_run;
}

void Forecast::setTimeStepInHours(uint32_t prediction_step)
{
    this->timeStepInHours = prediction_step;
}

float Forecast::getMinWindDirectionDeg()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].windDirectionDeg;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windDirectionDeg;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxWindDirectionDeg()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].windDirectionDeg;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windDirectionDeg;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinWindSpeedKt()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].windSpeedKt;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windSpeedKt;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxWindSpeedKt()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].windSpeedKt;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windSpeedKt;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinGustSpeedKt()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].gustSpeedKt;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].gustSpeedKt;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxGustSpeedKt()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].gustSpeedKt;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].gustSpeedKt;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinLowCloudCoverPer()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].lowCloudCoverPer;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].lowCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxLowCloudCoverPer()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].lowCloudCoverPer;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].lowCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinMidCloudCoverPer()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].midCloudCoverPer;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].midCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxMidCloudCoverPer()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].midCloudCoverPer;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].midCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinHighCloudCoverPer()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].highCloudCoverPer;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].highCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxHighCloudCoverPer()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].highCloudCoverPer;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].highCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinPrecipitationMmH()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].precipitationMmH;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].precipitationMmH;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxPrecipitationMmH()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].precipitationMmH;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].precipitationMmH;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinTemperatureC()
{
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].TemperatureC;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].TemperatureC;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxTemperatureC()
{
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].TemperatureC;
    for (uint32_t index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].TemperatureC;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

std::string Forecast::getModelName() {
	return(modelName);
}

std::string Forecast::getProviderName() {
	return(providerName);
}

uint32_t  Forecast::getRunTimeCode() {
	return(runTimeCode);
}

uint32_t  Forecast::getTimeStepInHours() {
	return(timeStepInHours);
}
