/*
 * SpotForecast.h
 *
 *  Created on: 13 nov. 2019
 *      Author: ronan
 */

#ifndef FORECAST_H_
#define FORECAST_H_

#define FC_MAX_NAME_LENGTH 64

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

class Forecast
{
private:
	char providerName[FC_MAX_NAME_LENGTH];
	char modelName[FC_MAX_NAME_LENGTH];
	int runTimeCode;
	unsigned short timeStepInHours;
	unsigned short numberOfSteps;
	WeatherData *forecastData;

public:
	Forecast();
	virtual ~Forecast();
	Forecast(const Forecast&);
	Forecast& operator=(const Forecast&);

	void Create(int numberOfSteps);
	void Clear();

	WeatherData& getForecastData(int index);

	unsigned int getBinarySize();
	unsigned short getNumberOfSteps();

	int writeBinary(void *buffer);
	int readBinary(void *buffer);

	void setProviderName(const char *providerName);
	void setModelName(const char *modelName);
	void setRunTimeCode(int runTimeCode);
	void setTimeStepInHours(int timeStepInHours);

	char* getModelName();
	char* getProviderName();
	int getRunTimeCode();
	int getTimeStepInHours();

private:
	float getMinWindDirectionDeg();
	float getMaxWindDirectionDeg();
	float getMinWindSpeedKt();
	float getMaxWindSpeedKt();
	float getMinGustSpeedKt();
	float getMaxGustSpeedKt();
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

#endif /* FORECAST_H_ */
