/*
 * wxColoredtext.cpp
 *
 *  Created on: 21 juin 2020
 *      Author: Ronan
 */

#include "ForecastDisplay.h"
#include "DateTime.h"

#include <wx/dcgraph.h>
#include <wx/graphics.h>
#include <wx/wx.h>

ForecastDisplay::ForecastDisplay(wxWindow *parent, ConfigContainer *config, wxWindowID winId, const wxString &label, const wxPoint &pos,
                                 const wxSize &size, long style, const wxValidator &validator, const wxString &name)
    : modelIndex(-1)
{
    (void)validator;
    (void)label;
    this->config = config;

    float    value           = 0;
    wxColour backgroundColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    wxColour foregroundColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    float    colorScale      = 255.0f;

    if (GetLuma(backgroundColor) < GetLuma(foregroundColor))
    {
        // We have a inverted color scheme (dark theme) : adapt color gradient accordingly
        colorScale = 320.0f;
    }

    windGradient.AddColorPoint(backgroundColor.Red() / 255.0f, backgroundColor.Green() / 255.0, backgroundColor.Blue() / 255.0, value += 3.0);
    windGradient.AddColorPoint(140.0 / colorScale, 255.0 / colorScale, 120.0 / colorScale, value += 4.5);
    windGradient.AddColorPoint(255.0 / colorScale, 255.0 / colorScale, 100.0 / colorScale, value += 9.0);
    windGradient.AddColorPoint(255.0 / colorScale, 128.0 / colorScale, 0.0 / colorScale, value += 11.0);
    windGradient.AddColorPoint(172.0 / colorScale, 0.0 / colorScale, 0.0 / colorScale, value += 13.0);
    windGradient.AddColorPoint(96.0 / colorScale, 0.0 / colorScale, 172.0 / colorScale, value += 15.0);
    windGradient.AddColorPoint(0.0 / colorScale, 0.0 / colorScale, 96.0 / colorScale, value += 15.0);
    windGradient.Generate();

    tempGradient.AddColorPoint(9.0 / colorScale, 247.0 / colorScale, 250.0 / colorScale, -10.0);
    tempGradient.AddColorPoint(backgroundColor.Red() / 255.0f, backgroundColor.Green() / 255.0, backgroundColor.Blue() / 255.0, 5.0);
    tempGradient.AddColorPoint(backgroundColor.Red() / 255.0f, backgroundColor.Green() / 255.0, backgroundColor.Blue() / 255.0, 20.0);
    tempGradient.AddColorPoint(255.0 / colorScale, 255.0 / colorScale, 128.0 / colorScale, 30.0);
    tempGradient.AddColorPoint(255.0 / colorScale, 32.0 / colorScale, 0.0 / colorScale, 42.0);
    tempGradient.Generate();

    precipitationGradient.AddColorPoint(backgroundColor.Red() / 255.0f, backgroundColor.Green() / 255.0, backgroundColor.Blue() / 255.0, 0.0);
    precipitationGradient.AddColorPoint(153.0 / colorScale, 254.0 / colorScale, 255.0 / colorScale, 0.5);
    precipitationGradient.AddColorPoint(64.0 / colorScale, 150.0 / colorScale, 244.0 / colorScale, 5.0);
    precipitationGradient.AddColorPoint(0.0 / colorScale, 50.0 / colorScale, 231.0 / colorScale, 10.0);
    precipitationGradient.Generate();

    cloudGradient.AddColorPoint(backgroundColor.Red() / 255.0f, backgroundColor.Green() / 255.0, backgroundColor.Blue() / 255.0, 0.0);
    cloudGradient.AddColorPoint(foregroundColor.Red() / 255.0f, foregroundColor.Green() / 255.0, foregroundColor.Blue() / 255.0, 100.0);
    cloudGradient.Generate();

    Create(parent, winId, pos, size, style | wxVSCROLL, name);

    // reportFont = wxFont(9, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Courier 10 Pitch");
    reportFont         = wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Courier");
    verticalFontSize   = reportFont.GetPixelSize().GetHeight() * 1.2;
    horizontalFontSize = reportFont.GetPixelSize().GetWidth();
    arrowSlotSize      = reportFont.GetPixelSize().GetHeight();

    Connect(wxEVT_PAINT, wxPaintEventHandler(ForecastDisplay::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(ForecastDisplay::OnSize));

    EnableScrolling(false, true);
    ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_ALWAYS);
    UpdateScrollBar();
}

void ForecastDisplay::UpdateScrollBar()
{
    SetVirtualSize(wxSize(GetRequestedHorizontalSize(), GetRequestedVerticalSize()));
    SetScrollbars(1, 1, GetRequestedHorizontalSize(), GetRequestedVerticalSize(), 0, 0, false);
    SetScrollRate(0, verticalFontSize);
}

ForecastDisplay::~ForecastDisplay()
{
}

void ForecastDisplay::SetForecast(SpotForecasts &spotForecasts, int modelIndex)
{
    this->spotForecasts = spotForecasts;
    this->modelIndex    = modelIndex;
    UpdateScrollBar();
    Refresh();
}

void ForecastDisplay::OnSize(wxSizeEvent &event)
{
    (void)event;
}

void ForecastDisplay::OnPaint(wxPaintEvent &event)
{
    (void)event;

    wxPaintDC dc(this);
    wxBitmap  arrowBitmap(arrowSlotSize, arrowSlotSize);

#if wxUSE_GRAPHICS_CONTEXT
    wxGCDC arrowDc(arrowBitmap);
#else
    wxMemoryDC arrowDc(arrowBitmap);
#endif

    DoPrepareDC(dc);
    dc.SetFont(reportFont);

    wxColour windowBgColor = wxSystemSettings::GetColour(wxSYS_COLOUR_DESKTOP);
    wxColour windowFgColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    dc.SetBackground(wxBrush(windowBgColor));
    dc.SetTextForeground(windowFgColor);
    dc.Clear();

    wxColour bgColor = windowBgColor;

    if ((modelIndex >= 0) && ((int)spotForecasts.GetNumberOfForecast() > modelIndex))
    {
        Forecast forecast    = spotForecasts.Get(modelIndex);
        int      verticalPos = 0;
        wxString stringToDraw;

        stringToDraw = wxString::Format("%-20s%s %s\n", _("Position") + " : ", GetLatitudeString(spotForecasts.GetLatitude()),
                                        GetLongitudeString(spotForecasts.GetLongitude()));
        dc.DrawText(stringToDraw, 0, verticalPos);
        verticalPos += verticalFontSize;

        stringToDraw = wxString::Format("%-20s%s\n", _("Model") + " : ", wxString::FromUTF8(forecast.GetModelName().c_str()));
        dc.DrawText(stringToDraw, 0, verticalPos);
        verticalPos += verticalFontSize;

        stringToDraw = wxString::Format("%-20s%s\n", _("Provider") + " : ", wxString::FromUTF8(forecast.GetProviderName().c_str()));
        dc.DrawText(stringToDraw, 0, verticalPos);
        verticalPos += verticalFontSize;

        DateTime runDate;
        runDate.SetTimeCode(forecast.GetRunTimeCode());
        if (config->timeZoneString.IsSameAs("UTC"))
        {
            stringToDraw = wxString::Format("%-20s%02d/%02d/%d %dh%02d\n", _("Run date") + " : ", runDate.GetGmtDay(), runDate.GetGmtMonth(),
                                            runDate.GetGmtYear(), runDate.GetGmtHour(), runDate.GetGmtMinute());
        }
        else
        {
            stringToDraw = wxString::Format("%-20s%02d/%02d/%d %dh%02d\n", _("Run date") + " : ", runDate.GetLocalDay(), runDate.GetLocalMonth(),
                                            runDate.GetLocalYear(), runDate.GetLocalHour(), runDate.GetLocalMinute());
        }
        dc.DrawText(stringToDraw, 0, verticalPos);
        verticalPos += verticalFontSize;

        stringToDraw = wxString::Format("%-20s%s\n\n", _("Time zone") + " : ", _(config->timeZoneString));
        dc.DrawText(stringToDraw, 0, verticalPos);
        verticalPos += verticalFontSize * 2;

        stringToDraw = wxString::Format("           %4s %4s %4s   %5s %5s %4s\n", _("Wind"), _("Gust"), _("Dir"), _("Rain"), _("Cloud"), _("Temp"));
        dc.DrawText(stringToDraw, 0, verticalPos);
        verticalPos += verticalFontSize;

        stringToDraw = wxString::Format("           %4s %4s %5s %5s %5s %4s\n", "", "", " ", "", "", "");
        dc.DrawText(stringToDraw, 0, verticalPos);
        DrawCenteredText(dc, _(config->windUnitString), horizontalFontSize * 11, verticalPos, horizontalFontSize * 4);
        DrawCenteredText(dc, _(config->windUnitString), horizontalFontSize * 16, verticalPos, horizontalFontSize * 4);
        DrawCenteredText(dc, _("mm/h"), horizontalFontSize * 27, verticalPos, horizontalFontSize * 5);
        DrawCenteredText(dc, _("%"), horizontalFontSize * 33, verticalPos, horizontalFontSize * 5);
        DrawCenteredText(dc, GetConvertedTempUnit(), horizontalFontSize * 39, verticalPos, horizontalFontSize * 4);
        verticalPos += verticalFontSize;

        DateTime    stepTime = runDate;
        string      dayName;
        WeatherData data;
        float       cloudCover;

        for (int step = 0; step < forecast.GetNumberOfSteps(); step += 1)
        {
            stepTime.AddHours(forecast.GetTimeStepInHours());
            dayName = stepTime.GetLocalDayName();

            stringToDraw = wxString::Format("%c%c.%02d  %2dh ", dayName[0], dayName[1], stepTime.GetLocalDay(), stepTime.GetLocalHour());

            data       = forecast.GetForecastData(step);
            cloudCover = data.lowCloudCoverPer;
            if (data.midCloudCoverPer * 0.66f > cloudCover)
                cloudCover = data.midCloudCoverPer * 0.66f;
            if (data.highCloudCoverPer * 0.33f > cloudCover)
                cloudCover = data.highCloudCoverPer * 0.33f;
            wxString precipitationString = wxString::Format("%.1f", data.precipitationMmH);
            if (data.precipitationMmH < 0.05)
            {
                precipitationString = "";
            }
            stringToDraw += wxString::Format("%4s %4s %5s %5s       %4s\n", "", "", "", "", "");

            bgColor.Set(windGradient.GetUintColor(data.windSpeedKt));
            dc.SetPen(wxPen(bgColor));
            dc.SetBrush(wxBrush(bgColor));
            dc.DrawRectangle(horizontalFontSize * 11, verticalPos - 1, horizontalFontSize * 4, verticalFontSize);
            dc.SetTextForeground(GetContrastedColor(bgColor, windowFgColor, windowBgColor));
            DrawCenteredText(dc, GetConvertedWind(data.windSpeedKt), horizontalFontSize * 11, verticalPos, horizontalFontSize * 4);

            bgColor.Set(windGradient.GetUintColor(data.gustSpeedKt));
            dc.SetPen(wxPen(bgColor));
            dc.SetBrush(wxBrush(bgColor));
            dc.DrawRectangle(horizontalFontSize * 16, verticalPos - 1, horizontalFontSize * 4, verticalFontSize);
            dc.SetTextForeground(GetContrastedColor(bgColor, windowFgColor, windowBgColor));
            DrawCenteredText(dc, GetConvertedWind(data.gustSpeedKt), horizontalFontSize * 16, verticalPos, horizontalFontSize * 4);

            // dc.SetTextForeground(GetForegroundColour());
            // DrawCenteredText(dc, GetTextDirection(data.windDirectionDeg), horizontalFontSize * 21, verticalPos, horizontalFontSize * 5);

            DrawArrow(arrowDc, arrowSlotSize / 2, arrowSlotSize / 2, data.windDirectionDeg, windowFgColor, windowBgColor);
            dc.DrawBitmap(arrowBitmap, horizontalFontSize * 22, verticalPos);

            bgColor.Set(precipitationGradient.GetUintColor(data.precipitationMmH));
            dc.SetPen(wxPen(bgColor));
            dc.SetBrush(wxBrush(bgColor));
            dc.DrawRectangle(horizontalFontSize * 27, verticalPos - 1, horizontalFontSize * 5, verticalFontSize);
            dc.SetTextForeground(GetContrastedColor(bgColor, windowFgColor, windowBgColor));
            DrawCenteredText(dc, precipitationString, horizontalFontSize * 27, verticalPos, horizontalFontSize * 5);

            cloudCover /= 100;
            cloudCover = powf(cloudCover, 1.5) * 100.0f;
            bgColor.Set(cloudGradient.GetUintColor(cloudCover));
            dc.SetPen(wxPen(bgColor));
            dc.SetBrush(wxBrush(bgColor));
            dc.DrawRectangle(horizontalFontSize * 33, verticalPos - 1, horizontalFontSize * 5, verticalFontSize);
            dc.SetTextForeground(GetContrastedColor(bgColor, windowFgColor, windowBgColor));
            DrawCenteredText(dc, wxString::Format("%.0f", cloudCover), horizontalFontSize * 33, verticalPos, horizontalFontSize * 5);

            bgColor.Set(tempGradient.GetUintColor(data.TemperatureC));
            dc.SetPen(wxPen(bgColor));
            dc.SetBrush(wxBrush(bgColor));
            dc.DrawRectangle(horizontalFontSize * 39, verticalPos - 1, horizontalFontSize * 4, verticalFontSize);
            dc.SetTextForeground(GetContrastedColor(bgColor, windowFgColor, windowBgColor));
            DrawCenteredText(dc, GetConvertedTemp(data.TemperatureC), horizontalFontSize * 39, verticalPos, horizontalFontSize * 4);

            dc.SetTextForeground(windowFgColor);
            dc.DrawText(stringToDraw, 0, verticalPos);
            verticalPos += verticalFontSize;
        }
    }
    else
    {
        dc.DrawText(_("Right click on the map and select \"Weather forecast\" to get forecast at this point."), 0, 0);
    }
}

wxColour ForecastDisplay::GetContrastedColor(wxColour &color, wxColour &fgColor1, wxColour &fgColor2)
{
    float luma, fgLuma1, fgLuma2;

    luma    = GetLuma(color);
    fgLuma1 = GetLuma(fgColor1);
    fgLuma2 = GetLuma(fgColor2);

    if (fabsf(luma - fgLuma1) < fabsf(luma - fgLuma2))
    {
        return (fgColor2);
    }
    else
    {
        return (fgColor1);
    }
}

float ForecastDisplay::GetLuma(wxColour color)
{
    return 0.2126f * color.Red() + 0.7152f * color.Green() + 0.0722f * color.Blue();
}

void ForecastDisplay::DrawCenteredText(wxDC &dc, wxString text, int x, int y, int width)
{
    wxSize size = dc.GetTextExtent(text);
    x           = x + (width / 2) - (size.x / 2);
    dc.DrawText(text, x, y);
}

wxString ForecastDisplay::GetLatitudeString(float latitude)
{
    wxString latitudeString;

    int   degs     = (int)roundf(floorf(fabsf(latitude)));
    float mins     = (fabsf(latitude) - degs) * 60.0f;
    latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (latitude < 0.0f ? _("S") : _("N")), degs, mins);

    return (latitudeString);
}

void ForecastDisplay::DrawArrow(wxDC &dc, double x, double y, float angle, wxColour &fgColor, wxColour &bgColor)
{
    double dx, dy;
    double sdx, sdy;
    double angle_rad;

    dc.SetBackground(wxBrush(bgColor));
    dc.Clear();

    dc.SetPen(wxPen(fgColor));
    dc.SetBrush(wxBrush(fgColor));

    angle_rad = angle * 3.1415f / 180.0f;
    dy        = -arrowSlotSize * cos(angle_rad) / 2.8f;
    dx        = arrowSlotSize * sin(angle_rad) / 2.8f;

    sdy = -arrowSlotSize * cos(angle_rad - M_PI / 2) / 4.0f;
    sdx = arrowSlotSize * sin(angle_rad - M_PI / 2) / 4.0f;

    wxPoint points[4] = {
        {(int)dx, (int)dy}, {(int)(sdx - dx), (int)(sdy - dy)}, {(int)(-0.6f * dx), (int)(-0.6f * dy)}, {(int)(-sdx - dx), (int)(-sdy - dy)}};
    dc.DrawPolygon(4, points, x, y);
}

int ForecastDisplay::GetRequestedVerticalSize()
{
    if ((modelIndex >= 0) && ((int)spotForecasts.GetNumberOfForecast() > modelIndex))
    {
        return (verticalFontSize * (8 + spotForecasts.Get(modelIndex).GetNumberOfSteps()));
    }

    return (1);
}

int ForecastDisplay::GetRequestedHorizontalSize()
{
    return (horizontalFontSize * 45);
}

wxString ForecastDisplay::GetLongitudeString(float longitude)
{
    wxString latitudeString;
    int      degs = (int)roundf(floorf(fabsf(longitude)));
    float    mins = (fabsf(longitude) - degs) * 60.0f;

    latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (longitude < 0.0f ? _("W") : _("E")), degs, mins);

    return (latitudeString);
}

wxString ForecastDisplay::GetTextDirection(float windDirectionDeg)
{
    const float angleStep = 22.5f;
    float       angle     = angleStep / 2;

    if (windDirectionDeg < 0.0f)
        windDirectionDeg += 360.0f;
    if (windDirectionDeg < angle)
        return (_("S"));
    else if (windDirectionDeg < angle + angleStep)
        return (_("S-SW"));
    else if (windDirectionDeg < angle + 2 * angleStep)
        return (_("SW"));
    else if (windDirectionDeg < angle + 3 * angleStep)
        return (_("W-SW"));
    else if (windDirectionDeg < angle + 4 * angleStep)
        return (_("W"));
    else if (windDirectionDeg < angle + 5 * angleStep)
        return (_("W-NW"));
    else if (windDirectionDeg < angle + 6 * angleStep)
        return (_("NW"));
    else if (windDirectionDeg < angle + 7 * angleStep)
        return (_("N-NW"));
    else if (windDirectionDeg < angle + 8 * angleStep)
        return (_("N"));
    else if (windDirectionDeg < angle + 9 * angleStep)
        return (_("N-NE"));
    else if (windDirectionDeg < angle + 10 * angleStep)
        return (_("NE"));
    else if (windDirectionDeg < angle + 11 * angleStep)
        return (_("E-NE"));
    else if (windDirectionDeg < angle + 12 * angleStep)
        return (_("E"));
    else if (windDirectionDeg < angle + 13 * angleStep)
        return (_("E-SE"));
    else if (windDirectionDeg < angle + 14 * angleStep)
        return (_("SE"));
    else if (windDirectionDeg < angle + 15 * angleStep)
        return (_("S-SE"));
    else
        return (_("N"));
}

wxString ForecastDisplay::GetConvertedWind(float windSpeedKt)
{
    if (config->windUnitString.IsSameAs("kt"))
    {
        return (wxString::Format("%d", (int)roundf(windSpeedKt)));
    }
    else if (config->windUnitString.IsSameAs("bft"))
    {
        if (windSpeedKt < 1.0f)
            return ("0");
        else if (windSpeedKt < 3.5f)
            return ("1");
        else if (windSpeedKt < 6.5f)
            return ("2");
        else if (windSpeedKt < 10.5f)
            return ("3");
        else if (windSpeedKt < 16.5f)
            return ("4");
        else if (windSpeedKt < 21.5f)
            return ("5");
        else if (windSpeedKt < 27.5f)
            return ("6");
        else if (windSpeedKt < 33.5f)
            return ("7");
        else if (windSpeedKt < 40.5f)
            return ("8");
        else if (windSpeedKt < 47.5f)
            return ("9");
        else if (windSpeedKt < 55.5f)
            return ("10");
        else if (windSpeedKt < 63.5f)
            return ("11");
        else
            return ("12");
    }
    else if (config->windUnitString.IsSameAs("m/s"))
    {
        return (wxString::Format("%d", (int)roundf(windSpeedKt / 1.94384f)));
    }
    else if (config->windUnitString.IsSameAs("kph"))
    {
        return (wxString::Format("%d", (int)roundf(windSpeedKt * 1.852f)));
    }
    else if (config->windUnitString.IsSameAs("mph"))
    {
        return (wxString::Format("%d", (int)roundf(windSpeedKt * 1.15078f)));
    }

    config->windUnitString = "kt";
    return (wxString::Format("%d", (int)roundf(windSpeedKt)));
}

wxString ForecastDisplay::GetConvertedTemp(float tempC)
{
    if (config->tempUnitString.IsSameAs("Farenheit"))
    {
        return (wxString::Format("%d", (int)roundf(tempC * (9.0f / 5.0f) + 32)));
    }

    config->tempUnitString = "Celsius";
    return (wxString::Format("%d", (int)roundf(tempC)));
}

wxString ForecastDisplay::GetConvertedTempUnit()
{
    if (config->tempUnitString.IsSameAs("Farenheit"))
    {
        return (_("F"));
    }

    return (_("C"));
}
