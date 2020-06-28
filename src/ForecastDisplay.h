/*
 * wxColoredtext.h
 *
 *  Created on: 21 juin 2020
 *      Author: Ronan
 */

#ifndef SRC_FORECASTDISPLAY_H_
#define SRC_FORECASTDISPLAY_H_

#include "SpotForecasts.h"
#include "ConfigContainer.h"
#include "ColorGradient.h"

#include <wx/wx.h>
#include <wx/collpane.h>

class ForecastDisplay: public wxScrolledWindow
{
public:
	ForecastDisplay(wxWindow *parent, ConfigContainer *config, wxWindowID winId, const wxString &label = wxEmptyString, const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxDefaultSize, long style = 0, const wxValidator &validator = wxDefaultValidator, const wxString &name = wxTextCtrlNameStr);
	virtual ~ForecastDisplay();

	void SetForecast(SpotForecasts &spotForecasts, int modelIndex);

protected:
	void OnPaint(wxPaintEvent&);
	void OnSize(wxSizeEvent &event);
	wxString GetLatitudeString(float latitude);        // Change a float latitude in a display-able string
	wxString GetLongitudeString(float longitude);      // Change a float longitude in a display-able string
	wxString GetConvertedTempUnit();
	wxString GetTextDirection(float windDirectionDeg);
	wxString GetConvertedWind(float windSpeedKt);
	wxString GetConvertedTemp(float tempC);
	wxColour GetContrastedColor(wxColour &color);
	void DrawCenteredText(wxPaintDC &dc, wxString text, int x, int y, int width);
	void DrawArrow(wxPaintDC &dc, double x, double y, float angle);
	int GetRequestedVerticalSize();
	int GetRequestedHorizontalSize();
	void UpdateScrollBar();

private:
	ConfigContainer *config;	         // Configuration parameters
	SpotForecasts spotForecasts;
	int modelIndex;
	ColorGradient windGradient;
	ColorGradient cloudGradient;
	ColorGradient tempGradient;
	ColorGradient precipitationGradient;
	wxFont reportFont;
	int verticalFontSize;
	int horizontalFontSize;
};

#endif /* SRC_FORECASTDISPLAY_H_ */
