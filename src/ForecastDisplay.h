/*
 * wxColoredtext.h
 *
 *  Created on: 21 juin 2020
 *      Author: Ronan
 */

#ifndef SRC_FORECASTDISPLAY_H_
#define SRC_FORECASTDISPLAY_H_

#include "ColorGradient.h"
#include "ConfigContainer.h"
#include "SpotForecasts.h"

#include <wx/collpane.h>
#include <wx/wx.h>

class ForecastDisplay : public wxScrolledWindow
{
  public:
    ForecastDisplay(wxWindow *parent, wxWindowID winId, const wxString &label = wxEmptyString,
                    const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0,
                    const wxValidator &validator = wxDefaultValidator, const wxString &name = wxTextCtrlNameStr);
    virtual ~ForecastDisplay();

    void SetConfig(ConfigContainer *config);
    void SetForecast(SpotForecasts &spotForecasts, int modelIndex);

  protected:
    void     OnPaint(wxPaintEvent &);
    void     OnSize(wxSizeEvent &event);
    wxString GetLatitudeString(float latitude);   // Change a float latitude in a display-able string
    wxString GetLongitudeString(float longitude); // Change a float longitude in a display-able string
    wxString GetConvertedTempUnit();
    wxString GetTextDirection(float windDirectionDeg);
    wxString GetConvertedWind(float windSpeedKt);
    wxString GetConvertedTemp(float tempC);
    wxColour GetContrastedColor(wxColour &color, wxColour &fgColor1, wxColour &fgColor2);
    float    GetLuma(wxColour color);
    void     DrawCenteredText(wxDC &dc, wxString text, int x, int y, int width);
    void     DrawArrow(wxDC &dc, double x, double y, float angle, wxColour &fgColor, wxColour &bgColor);
    int      GetRequestedVerticalSize();
    int      GetRequestedHorizontalSize();
    void     UpdateScrollBar();
    void     SetReportFontSize();

  private:
    ConfigContainer *config; // Configuration parameters
    SpotForecasts    spotForecasts;
    int              modelIndex;
    ColorGradient    windGradient;
    ColorGradient    cloudGradient;
    ColorGradient    tempGradient;
    ColorGradient    precipitationGradient;
    wxFont           reportFont;
    int              verticalFontSize;
    int              horizontalFontSize;
    int              arrowSlotSize;
    wxColour         windowBgColor;
    wxColour         windowFgColor;
};

#endif /* SRC_FORECASTDISPLAY_H_ */
