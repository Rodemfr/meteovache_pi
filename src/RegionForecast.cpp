#include <RegionForecast.h>
#include <string.h>

RegionForecast::RegionForecast() : runDate(0), hoursPerTimeStep(1), numberOfTimeSteps(0), forecastData(0)
{
    memset(modelProvider, 0, sizeof(modelProvider));
    memset(modelName, 0, sizeof(modelName));
}

RegionForecast::~RegionForecast()
{
    if (forecastData != 0)
    {
        delete [] forecastData;
    }
}

RegionForecast &RegionForecast::operator=(const RegionForecast &regionForecast)
{
    if (this != &regionForecast)
    {
        memcpy(modelProvider, regionForecast.modelProvider, sizeof(modelProvider));
        memcpy(modelName, regionForecast.modelName, sizeof(modelName));

        runDate = regionForecast.runDate;
        hoursPerTimeStep = regionForecast.hoursPerTimeStep;
        numberOfTimeSteps = regionForecast.numberOfTimeSteps;
        forecastData = regionForecast.forecastData;

        if (forecastData != 0)
        {
            forecastData = new ForecastData[numberOfTimeSteps];
            memcpy(forecastData, regionForecast.forecastData, numberOfTimeSteps * sizeof(ForecastData));
        }
    }

    return (*this);
}

RegionForecast::RegionForecast(const RegionForecast &regionForecast) : runDate(regionForecast.runDate), hoursPerTimeStep(regionForecast.hoursPerTimeStep), numberOfTimeSteps(regionForecast.numberOfTimeSteps), forecastData(0)
{
    memcpy(modelProvider, regionForecast.modelProvider, sizeof(modelProvider));
    memcpy(modelName, regionForecast.modelName, sizeof(modelName));


    if (regionForecast.forecastData != 0)
    {
        forecastData = new ForecastData[numberOfTimeSteps];
        memcpy(forecastData, regionForecast.forecastData, numberOfTimeSteps * sizeof(ForecastData));
    }
}

void RegionForecast::Create(int numberOfTimeSteps)
{
    this->Destroy();

    if (numberOfTimeSteps > 0)
    {
        forecastData = new ForecastData[numberOfTimeSteps];
        this->numberOfTimeSteps = numberOfTimeSteps;
    }
}

void RegionForecast::Destroy()
{
    if (forecastData != 0)
    {
        delete [] forecastData;
        forecastData = 0;
    }
    numberOfTimeSteps = 0;
}

ForecastData &RegionForecast::GetForecast(int index)
{
    return(forecastData[index]);
}

unsigned int RegionForecast::getBinarySize()
{
    unsigned int binarySize = 0;

    binarySize += strlen(modelProvider) + 1;
    binarySize += strlen(modelName) + 1;
    binarySize += sizeof(runDate);
    binarySize += sizeof(hoursPerTimeStep);
    binarySize += sizeof(numberOfTimeSteps);
    binarySize += sizeof(char) * numberOfTimeSteps * 8;

    return(binarySize);
}

unsigned short RegionForecast::getNumberOfTimeSteps()
{
    return(numberOfTimeSteps);
}

void *RegionForecast::writeBinary(void *buffer)
{
    char *outputData = (char *)buffer;
    int index;

    strcpy((char *)outputData, modelProvider);
    outputData += strlen(modelProvider) + 1;

    strcpy((char *)outputData, modelName);
    outputData += strlen(modelName) + 1;

    *((int *)outputData) = runDate;
    outputData += sizeof(int);

    *((unsigned short *)outputData) = hoursPerTimeStep;
    outputData += sizeof(unsigned short);

    *((unsigned short *)outputData) = numberOfTimeSteps;
    outputData += sizeof(unsigned short);

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)(forecastData[index].windSpeedMS + 0.5f);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)((forecastData[index].windDirectionDeg / 2) + 0.5f);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)(forecastData[index].gustSpeedMS + 0.5f);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)(forecastData[index].lowCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)(forecastData[index].midCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)(forecastData[index].highCloudCoverPer + 0.5f);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)((forecastData[index].precipitationMmH * 10.0f) + 0.5f);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        *(outputData++) = (int)(forecastData[index].temperatureC + 0.5f);
    }

    return(outputData);
}

void RegionForecast::readBinary(void *buffer)
{
    unsigned char *inputData = (unsigned char *)buffer;
    int index;

    this->Destroy();

    memset(modelProvider, 0, sizeof(modelProvider));
    strcpy(modelProvider, (char *)inputData);
    inputData += strlen((char *)inputData) + 1;

    memset(modelName, 0, sizeof(modelName));
    strcpy(modelName, (char *)inputData);
    inputData += strlen((char *)inputData) + 1;

    runDate = *((int *)inputData);
    inputData += sizeof(int);

    hoursPerTimeStep = *((unsigned short *)inputData);
    inputData += sizeof(unsigned short);

    numberOfTimeSteps = *((unsigned short *)inputData);
    inputData += sizeof(unsigned short);

    if (numberOfTimeSteps > 0)
    {
        forecastData = new ForecastData[numberOfTimeSteps];
    }
    else
    {
        return;
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].windDirectionDeg = (*(inputData++)) * 2;
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].windSpeedMS = *(inputData++);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].gustSpeedMS = *(inputData++);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].lowCloudCoverPer = *(inputData++);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].midCloudCoverPer = *(inputData++);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].highCloudCoverPer = *(inputData++);
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].precipitationMmH = *(inputData++) / 10.0f;
    }

    for (index = 0; index < numberOfTimeSteps; index++)
    {
        forecastData[index].temperatureC = (*(inputData++));
    }
}

void RegionForecast::setProviderName(const char *providerName)
{
	strncpy(this->modelProvider, providerName, sizeof(this->modelProvider));
}

void RegionForecast::setModelName(const char *modelName)
{
    strncpy(this->modelName, modelName, sizeof(this->modelName));
}

void RegionForecast::setRunDate(int runDate)
{
    this->runDate = runDate;
}

void RegionForecast::setHoursPerTimeStep(int hoursPerTimeStep)
{
    this->hoursPerTimeStep = hoursPerTimeStep;
}

float RegionForecast::getMinWindDirectionDeg()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].windDirectionDeg;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].windDirectionDeg;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxWindDirectionDeg()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].windDirectionDeg;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].windDirectionDeg;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float RegionForecast::getMinWindSpeedMS()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].windSpeedMS;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].windSpeedMS;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxWindSpeedMS()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].windSpeedMS;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].windSpeedMS;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float RegionForecast::getMinGustSpeedMS()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].gustSpeedMS;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].gustSpeedMS;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxGustSpeedMS()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].gustSpeedMS;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].gustSpeedMS;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float RegionForecast::getMinLowCloudCoverPer()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].lowCloudCoverPer;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].lowCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxLowCloudCoverPer()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].lowCloudCoverPer;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].lowCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float RegionForecast::getMinMidCloudCoverPer()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].midCloudCoverPer;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].midCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxMidCloudCoverPer()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].midCloudCoverPer;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].midCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float RegionForecast::getMinHighCloudCoverPer()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].highCloudCoverPer;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].highCloudCoverPer;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxHighCloudCoverPer()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].highCloudCoverPer;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].highCloudCoverPer;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float RegionForecast::getMinPrecipitationMmH()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].precipitationMmH;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].precipitationMmH;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxPrecipitationMmH()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].precipitationMmH;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].precipitationMmH;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}

float RegionForecast::getMinTemperatureC()
{
    int index;
    float minValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    minValue = forecastData[0].temperatureC;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].temperatureC;
        if (value < minValue) minValue = value;
    }

    return(minValue);
}

float RegionForecast::getMaxTemperatureC()
{
    int index;
    float maxValue, value;

    if (numberOfTimeSteps <= 0) return(0.0f);

    maxValue = forecastData[0].temperatureC;
    for (index = 1; index < numberOfTimeSteps; index++)
    {
        value = forecastData[index].temperatureC;
        if (value > maxValue) maxValue = value;
    }

    return(maxValue);
}
