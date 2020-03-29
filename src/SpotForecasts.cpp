/*
 * SpotForecasts.cpp
 *
 *  Created on: 17 nov. 2019
 *      Author: ronan
 */

#include <SpotForecasts.h>

SpotForecasts::SpotForecasts() : latitudeDeg(0), longitudeDeg(0), numberOfForecasts(0), forecastList() {
}

SpotForecasts::~SpotForecasts() {
	if (forecastList != nullptr) {
		delete [] forecastList;
	}
}

void SpotForecasts::Lock() {
	locker.Enter();
}

void SpotForecasts::Unlock() {
	locker.Leave();
}

void SpotForecasts::Reset(int numberOfForecasts) {
	if (this->numberOfForecasts != 0) {
		delete [] forecastList;
	}

	this->numberOfForecasts = numberOfForecasts;
	forecastList = new Forecast[numberOfForecasts];
}

Forecast &SpotForecasts::Get(int i) {
	return(forecastList[i]);
}

void SpotForecasts::SetPosition(float latitudeDeg, float longitudeDeg) {
	this->latitudeDeg = latitudeDeg;
	this->longitudeDeg = longitudeDeg;
}

float SpotForecasts::GetLatitude() {
	return(latitudeDeg);
}

float SpotForecasts::GetLongitude() {
	return(longitudeDeg);
}

int SpotForecasts::GetNumberOfForecast() {
	return(numberOfForecasts);
}
