/*
 * SpotForecast.cpp
 *
 *  Created on: 13 nov. 2019
 *      Author: ronan
 */

#include <Forecast.h>
#include <string.h>

Forecast::Forecast() : runTimeCode(0), timeStepInHours(1), numberOfSteps(0), forecastData(0)
{
    memset(providerName, 0, sizeof(providerName));
    memset(modelName, 0, sizeof(modelName));
}

Forecast::~Forecast()
{
    if (forecastData != 0)
    {
        delete [] forecastData;
    }
}

Forecast &Forecast::operator=(const Forecast &spot)
{
    // Check if we are assigning object to itself
    if (this != &spot)
    {
        memcpy(providerName, spot.providerName, sizeof(providerName));
        memcpy(modelName, spot.modelName, sizeof(modelName));

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

Forecast::Forecast(const Forecast &spot) : runTimeCode(spot.runTimeCode), timeStepInHours(spot.timeStepInHours), numberOfSteps(spot.numberOfSteps), forecastData(0)
{
    memcpy(providerName, spot.providerName, sizeof(providerName));
    memcpy(modelName, spot.modelName, sizeof(modelName));


    if (spot.forecastData != 0)
    {
    	forecastData = new WeatherData[numberOfSteps];
        memcpy(forecastData, spot.forecastData, numberOfSteps * sizeof(WeatherData));
    }
}

void Forecast::Create(int prediction_length)
{
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

WeatherData &Forecast::getForecastData(int index)
{
    return(forecastData[index]);
}

unsigned int Forecast::getBinarySize()
{
    unsigned int binarySize = 0;

    binarySize += strlen(providerName) + 1;
    binarySize += strlen(modelName) + 1;
    binarySize += sizeof(runTimeCode);
    binarySize += sizeof(timeStepInHours);
    binarySize += sizeof(numberOfSteps);
    binarySize += sizeof(char) * numberOfSteps * 8;

    return(binarySize);
}

unsigned short Forecast::getNumberOfSteps()
{
    return(numberOfSteps);
}

int Forecast::writeBinary(void *outputBuffer)
{
    char *pOutput = (char *)outputBuffer;
    int index;

    strcpy((char *)pOutput, providerName);
    pOutput += strlen(providerName) + 1;

    strcpy((char *)pOutput, modelName);
    pOutput += strlen(modelName) + 1;

    *((int *)pOutput) = runTimeCode;
    pOutput += sizeof(int);

    *((unsigned short *)pOutput) = timeStepInHours;
    pOutput += sizeof(unsigned short);

    *((unsigned short *)pOutput) = numberOfSteps;
    pOutput += sizeof(unsigned short);

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)((forecastData[index].windSpeedKt / 1.94384f) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)((forecastData[index].windDirectionDeg / 2) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)((forecastData[index].gustSpeedKt / 1.94384f) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)(forecastData[index].lowCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)(forecastData[index].midCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)(forecastData[index].highCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)((forecastData[index].precipitationMmH * 10.0f) + 0.5f);
    }

    for (index = 0; index < numberOfSteps; index++)
    {
        *(pOutput++) = (int)(forecastData[index].TemperatureC + 0.5f);
    }

    return(pOutput - (char *)outputBuffer);
}

int Forecast::readBinary(void *inputBuffer)
{
    unsigned char *pInput = (unsigned char *)inputBuffer;
    int index;

    this->Clear();

    memset(providerName, 0, sizeof(providerName));
    strcpy(providerName, (char *)pInput);
    pInput += strlen((char *)pInput) + 1;

    memset(modelName, 0, sizeof(modelName));
    strcpy(modelName, (char *)pInput);
    pInput += strlen((char *)pInput) + 1;

    runTimeCode = *((int *)pInput);
    pInput += sizeof(int);

    timeStepInHours = *((unsigned short *)pInput);
    pInput += sizeof(unsigned short);

    numberOfSteps = *((unsigned short *)pInput);
    pInput += sizeof(unsigned short);

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
    	forecastData[index].TemperatureC = (*(pInput++));
    }

    return(pInput - (unsigned char *)inputBuffer);
}

void Forecast::setProviderName(const char *model_provider)
{
    strncpy(this->providerName, model_provider, sizeof(this->providerName) - 1);
}

void Forecast::setModelName(const char *model_name)
{
    strncpy(this->modelName, model_name, sizeof(this->modelName) - 1);
}

void Forecast::setRunTimeCode(int model_run)
{
    this->runTimeCode = model_run;
}

void Forecast::setTimeStepInHours(int prediction_step)
{
    this->timeStepInHours = prediction_step;
}

float Forecast::getMinWindDirectionDeg()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].windDirectionDeg;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windDirectionDeg;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxWindDirectionDeg()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].windDirectionDeg;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windDirectionDeg;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinWindSpeedKt()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].windSpeedKt;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windSpeedKt;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxWindSpeedKt()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].windSpeedKt;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].windSpeedKt;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinGustSpeedKt()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].gustSpeedKt;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].gustSpeedKt;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxGustSpeedKt()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].gustSpeedKt;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].gustSpeedKt;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinLowCloudCoverPer()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].lowCloudCoverPer;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].lowCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxLowCloudCoverPer()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].lowCloudCoverPer;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].lowCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinMidCloudCoverPer()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].midCloudCoverPer;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].midCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxMidCloudCoverPer()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].midCloudCoverPer;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].midCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinHighCloudCoverPer()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].highCloudCoverPer;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].highCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxHighCloudCoverPer()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].highCloudCoverPer;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].highCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinPrecipitationMmH()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].precipitationMmH;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].precipitationMmH;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxPrecipitationMmH()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].precipitationMmH;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].precipitationMmH;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float Forecast::getMinTemperatureC()
{
    int index;
    float minValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    minValue = forecastData[0].TemperatureC;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].TemperatureC;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float Forecast::getMaxTemperatureC()
{
    int index;
    float maxValue, value;

    if (numberOfSteps <= 0) return(0.0f);

    maxValue = forecastData[0].TemperatureC;
    for (index = 1; index < numberOfSteps; index++)
    {
        value = forecastData[index].TemperatureC;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

char *Forecast::getModelName() {
	return(modelName);
}

char *Forecast::getProviderName() {
	return(providerName);
}

int Forecast::getRunTimeCode() {
	return(runTimeCode);
}

int Forecast::getTimeStepInHours() {
	return(timeStepInHours);
}
