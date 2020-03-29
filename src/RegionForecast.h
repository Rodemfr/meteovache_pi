#ifndef _REGIONFORECAST_H_
#define _REGIONFORECAST_H_

#define WF_MAX_NAME_LENGTH 64

typedef struct
{
    float windDirectionDeg;
    float windSpeedMS;
    float gustSpeedMS;
    float lowCloudCoverPer;
    float midCloudCoverPer;
    float highCloudCoverPer;
    float precipitationMmH;
    float temperatureC;
} ForecastData;

class RegionForecast
{
private:
    char modelProvider[WF_MAX_NAME_LENGTH];
    char modelName[WF_MAX_NAME_LENGTH];
    int runDate;
    unsigned short hoursPerTimeStep;
    unsigned short numberOfTimeSteps;
    ForecastData *forecastData;

public:
    RegionForecast();
    virtual ~RegionForecast();
    RegionForecast(const RegionForecast&);
    RegionForecast &operator=(const RegionForecast&);

    void Create(int numberOfTimeSteps);
    void Destroy();

    ForecastData &GetForecast(int index);

    unsigned int getBinarySize();
    unsigned short getNumberOfTimeSteps();

    void *writeBinary(void *buffer);
    void readBinary(void *buffer);

    void setProviderName(const char *modelProvider);
    void setModelName(const char *modelName);
    void setRunDate(int runDate);
    void setHoursPerTimeStep(int hoursPerTimeStep);

private:
    float getMinWindDirectionDeg();
    float getMaxWindDirectionDeg();
    float getMinWindSpeedMS();
    float getMaxWindSpeedMS();
    float getMinGustSpeedMS();
    float getMaxGustSpeedMS();
    float getMinLowCloudCoverPer();
    float getMaxLowCloudCoverPer();
    float getMinMidCloudCoverPer();
    float getMaxMidCloudCoverPer();
    float getMinHighCloudCoverPer();
    float getMaxHighCloudCoverPer();
    float getMinPrecipitationMmH();
    float getMaxPrecipitationMmH();
    float getMinTemperatureC();
    float getMaxTemperatureC();
};

#endif /* _REGIONFORECAST_H_ */
