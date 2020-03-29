/*
 * SpotForecasts.h
 *
 *  Created on: 17 nov. 2019
 *      Author: ronan
 */

#ifndef SPOTFORECASTS_H_
#define SPOTFORECASTS_H_

#include "Forecast.h"
#include <wx/thread.h>

class SpotForecasts {
public:
	SpotForecasts();
	virtual ~SpotForecasts();

	void Lock();
	void Unlock();
	void Reset(int nbForecasts);
	Forecast &Get(int i);
	void SetPosition(float latitudeDeg, float longitudeDeg);
	float GetLatitude();
	float GetLongitude();
	int GetNumberOfForecast();

private:
	float latitudeDeg;
	float longitudeDeg;
	int numberOfForecasts;
	Forecast *forecastList;
	wxCriticalSection locker;
};

#endif /* SPOTFORECASTS_H_ */
