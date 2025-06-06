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

#include "ReportWindow.h"
#include "DateTime.h"
#include "JobQueue.h"
#include "NetworkThread.h"

#include <math.h>
#include <stdint.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

wxBEGIN_EVENT_TABLE(ReportWindow, wxDialog) EVT_COMMAND(wxID_ANY, wxEVT_THREAD_JOB_COMPLETED, ReportWindow::OnThreadEvent)
    EVT_COMMAND(wxID_ANY, wxEVT_THREAD_JOB_ONGOING, ReportWindow::OnThreadEvent)
        EVT_COMMAND(wxID_ANY, wxEVT_THREAD_JOB_FAILED, ReportWindow::OnThreadEvent) wxEND_EVENT_TABLE()

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

    ReportWindow::ReportWindow(wxWindow *parent, ConfigContainer *config, wxWindowID id, const wxString &title, const wxPoint &pos,
                               const wxSize &size, long style) : ReportWindowBase(parent, id, title, pos, size, style)
{
    // Initialize all useful fields
    this->config  = config;
    progressCount = 0;
    workerThread  = nullptr;
    jobQueue      = new JobQueue(GetEventHandler());

    wxFont reportFont = reportTextArea->GetFont();
    reportFont.SetFamily(wxFONTFAMILY_TELETYPE);
    reportTextArea->SetFont(reportFont);
    reportTextArea->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));
    reportTextArea->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    reportTextArea->SetConfig(config);

    // Connect Events
    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(ReportWindow::OnClose));
    modelSelector->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(ReportWindow::OnModelSelect), NULL, this);
    saveButton->Connect(wxEVT_BUTTON, wxCommandEventHandler(ReportWindow::OnSaveAs), NULL, this);

    // Start the network thread
    StartThread();
}

ReportWindow::~ReportWindow()
{
    // Stop the network thread. this is a blocking call which will wait for the actual thread end
    // This might take up to one second if the deletion is requested during a network request
    StopThread();
    // Disconnect Events
    this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(ReportWindow::OnClose));
    modelSelector->Disconnect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(ReportWindow::OnModelSelect), NULL, this);
    saveButton->Disconnect(wxEVT_BUTTON, wxCommandEventHandler(ReportWindow::OnSaveAs), NULL, this);

    delete jobQueue;
}

void ReportWindow::OnClose(wxCloseEvent &event)
{
    (void)event;

    // If the close button is pressed, we just hide the window We don't delete it because the network
    // thread needs it. The window is only deleted when the plug-in is stopped or when OpenCPN is exiting
    this->Show(false);
}

void ReportWindow::OnModelSelect(wxCommandEvent &event)
{
    (void)event;
    // When the selected model is changed by the user, we update the report window with the corresponding forecast
    config->selectedModelName = modelSelector->GetStringSelection();
    SetReportForecast(modelSelector->GetSelection());
}

void ReportWindow::UpdateConfig()
{
    // We update the configuration object with the parameters which may have changed without notification
    // This function is called before exiting the plug-in to ensure we will save the updated values to
    // the configuration file
    config->windowXPos   = this->GetPosition().x;
    config->windowYPos   = this->GetPosition().y;
    config->windowWidth  = this->GetSize().x;
    config->windowHeight = this->GetSize().y;
}

void ReportWindow::SetReportForecast(int modelSelection)
{
    // Update the report text
    spotForecast.Lock();
    reportTextArea->SetForecast(spotForecast, modelSelection);
    spotForecast.Unlock();
}

void ReportWindow::StartThread()
{
    // We first stop any already existing thread. It is not suppoed to happen anyway...
    StopThread();
    // And we start the new one
    workerThread = new NetworkThread(&spotForecast, jobQueue);
}

void ReportWindow::StopThread()
{
    // Stop the network thread
    if (workerThread != nullptr)
    {
        workerThread->RequestEnd();
        workerThread = nullptr;
    }
}

void ReportWindow::OnThreadEvent(wxCommandEvent &evt)
{
    if (evt.GetEventType() == wxEVT_THREAD_JOB_COMPLETED)
    {
        statusLabel->SetLabelText(_("Data successfully downloaded from server"));
        wxString      newString;
        SpotForecasts localForecasts;

        spotForecast.Lock();
        localForecasts = spotForecast;
        spotForecast.Unlock();

        modelSelector->Clear();

        for (uint32_t i = 0; i < localForecasts.GetNumberOfForecast(); i++)
        {
            newString = wxString::FromUTF8(wxString(localForecasts.Get(i).GetModelName()));
            modelSelector->Append(newString);
            if (i == 0)
            {
                modelSelector->SetSelection(0);
            }
            else if (newString.IsSameAs(config->selectedModelName))
            {
                modelSelector->SetSelection(i);
            }
        }

        Layout();

        config->selectedModelName = modelSelector->GetStringSelection();
        SetReportForecast(modelSelector->GetSelection());
        AutoSaveReport();
    }
    else if (evt.GetEventType() == wxEVT_THREAD_JOB_ONGOING)
    {
        statusLabel->SetLabelText(_("Contacting server ...") + " " + GetNextWaitingChar());
    }
    else if (evt.GetEventType() == wxEVT_THREAD_JOB_FAILED)
    {
        statusLabel->SetLabelText(wxString::Format("Download failed : server not responding"));
    }
    wxWakeUpIdle();
}

void ReportWindow::AutoSaveReport()
{
    if (config->autoSaveEnable)
    {
        wxString fileReport;
        if (config->autoSaveColumn)
        {
            fileReport = PrintWeatherColumnReports();
        }
        else
        {
            fileReport = PrintWeatherReports();
        }
        if (config->autoSaveCompress)
        {
            wxFileOutputStream outputStream(config->autoSavePath + wxFileName::GetPathSeparator() + GetReportBaseName() + ".zip");
            if (outputStream.IsOk())
            {
                wxZipOutputStream zipStream(outputStream);
                zipStream.SetLevel(9);
                zipStream.PutNextEntry(GetReportBaseName() + ".txt");
                zipStream.Write(fileReport.ToUTF8(), fileReport.length());
                zipStream.CloseEntry();
                zipStream.Close();
            }
        }
        else
        {
            wxFileOutputStream outputStream(config->autoSavePath + wxFileName::GetPathSeparator() + GetReportBaseName() + ".txt");
            if (outputStream.IsOk())
            {
                outputStream.Write(fileReport.ToUTF8(), fileReport.length());
                outputStream.Close();
            }
        }
    }
}

wxString ReportWindow::PrintWeatherReport(int modelIndex)
{
    wxString      modelInfo;
    Forecast     *forecast;
    WeatherData   data;
    float         cloudCover;
    SpotForecasts localForecasts;

    spotForecast.Lock();
    localForecasts = spotForecast;
    spotForecast.Unlock();

    forecast = &localForecasts.Get(modelIndex);

    DateTime runDate;
    runDate.SetTimeCode(forecast->GetRunTimeCode());
    modelInfo = modelInfo.Append(wxString::Format("%-20s%s %s\n", _("Position") + " : ", GetLatitudeString(localForecasts.GetLatitude()),
                                                  GetLongitudeString(localForecasts.GetLongitude())));
    modelInfo = modelInfo.Append(wxString::Format("%-20s%s\n", _("Model") + " : ", wxString::FromUTF8(forecast->GetModelName().c_str())));
    modelInfo = modelInfo.Append(wxString::Format("%-20s%s\n", _("Provider") + " : ", wxString::FromUTF8(forecast->GetProviderName().c_str())));

    int year, month, day, hour, minute;
    if (config->timeZoneString.IsSameAs("UTC"))
    {
        year   = runDate.GetGmtYear();
        month  = runDate.GetGmtMonth();
        day    = runDate.GetGmtDay();
        hour   = runDate.GetGmtHour();
        minute = runDate.GetGmtMinute();
    }
    else
    {
        year   = runDate.GetLocalYear();
        month  = runDate.GetLocalMonth();
        day    = runDate.GetLocalDay();
        hour   = runDate.GetLocalHour();
        minute = runDate.GetLocalMinute();
    }

    /* TRANSLATORS: Must be 17 characters max */
    modelInfo = wxString::Format("%-20s%d/%02d/%02d %dh%02d\n", _("Run date") + " : ", year, month, day, hour, minute);
    modelInfo = modelInfo.Append(wxString::Format("%-20s%s\n\n", _("Time zone") + " : ", _(config->timeZoneString)));

    modelInfo =
        modelInfo.Append(wxString::Format("           %4s %4s %5s %5s %5s %4s\n", _("Wind"), _("Gust"), _("Dir"), _("Rain"), _("Cloud"), _("Temp")));
    modelInfo = modelInfo.Append(wxString::Format("           %4s %4s %5s %5s %5s %4s\n", _(config->windUnitString), _(config->windUnitString), " ",
                                                  /* TRANSLATORS: Precipitation unit in mm/h */
                                                  _("mm/h"), "%", GetConvertedTempUnit()));

    DateTime    stepTime = runDate;
    std::string dayName;
    for (int step = 0; step < forecast->GetNumberOfSteps(); step += 1)
    {
        stepTime.AddHours(forecast->GetTimeStepInHours());
        dayName = stepTime.GetLocalDayName();

        wxString shortDayName(dayName);
        shortDayName = shortDayName.substr(0, 2);

        modelInfo  = modelInfo.Append(wxString::Format("%s.%02d  %2dh ", shortDayName.c_str(), stepTime.GetLocalDay(), stepTime.GetLocalHour()));
        data       = forecast->GetForecastData(step);
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
        modelInfo = modelInfo.Append(wxString::Format("%4s %4s %5s %5s   %3.0f %4s\n", GetConvertedWind(data.windSpeedKt).c_str(),
                                                      GetConvertedWind(data.gustSpeedKt).c_str(), GetTextDirection(data.windDirectionDeg),
                                                      precipitationString, cloudCover, GetConvertedTemp(data.TemperatureC).c_str()));
    }
    modelInfo = modelInfo.Append("\n");

    return (modelInfo);
}

wxString ReportWindow::PrintWeatherReports()
{
    wxString      report;
    SpotForecasts localForecasts;

    spotForecast.Lock();
    localForecasts = spotForecast;
    spotForecast.Unlock();

    uint32_t nbReports = localForecasts.GetNumberOfForecast();
    for (uint32_t i = 0; i < nbReports; i++)
    {
        report += PrintWeatherReport(i);
    }

    return (report);
}

wxString ReportWindow::PrintWeatherColumnReports()
{
    wxString     *reportArray = nullptr;
    uint32_t      nbReports;
    wxString      columnReport;
    SpotForecasts localForecasts;

    spotForecast.Lock();
    localForecasts = spotForecast;
    spotForecast.Unlock();

    nbReports   = localForecasts.GetNumberOfForecast();
    reportArray = new wxString[nbReports];
    for (uint32_t i = 0; i < nbReports; i++)
    {
        reportArray[i] = PrintWeatherReport(i);
    }

    bool continueLoop;
    do
    {
        continueLoop = false;
        for (uint32_t i = 0; i < nbReports; i++)
        {
            columnReport += wxString::Format("%-50s", reportArray[i].BeforeFirst('\n'));
            reportArray[i] = reportArray[i].AfterFirst('\n');
            if (reportArray[i].length() > 0)
            {
                continueLoop = true;
            }
        }
        columnReport += '\n';
    } while (continueLoop);

    delete[] reportArray;

    return (columnReport);
}

void ReportWindow::RequestForecast(float latitude, float longitude)
{
    modelSelector->Clear();
    modelSelector->SetStringSelection(_("No data"));
    spotForecast.Lock();
    spotForecast.Reset();
    spotForecast.Unlock();
    SetReportForecast(0);
    statusLabel->SetLabelText(_("Contacting server ...") + " " + GetNextWaitingChar());
    JobRequest job(JobRequest::CMD_GET_ALL_FORECASTS_AT_LOCATION, latitude, longitude);
    jobQueue->AddJobRequest(job);
}

wxString ReportWindow::GetLatitudeString(float latitude)
{
    wxString latitudeString;

    int   degs     = (int)roundf(floorf(fabsf(latitude)));
    float mins     = (fabsf(latitude) - degs) * 60.0f;
    latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (latitude < 0.0f ? _("S") : _("N")), degs, mins);

    return (latitudeString);
}

wxString ReportWindow::GetLongitudeString(float longitude)
{
    wxString latitudeString;
    int      degs = (int)roundf(floorf(fabsf(longitude)));
    float    mins = (fabsf(longitude) - degs) * 60.0f;

    latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (longitude < 0.0f ? _("W") : _("E")), degs, mins);

    return (latitudeString);
}

wxString ReportWindow::GetTextDirection(float windDirectionDeg)
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

wxString ReportWindow::GetConvertedWind(float windSpeedKt)
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

wxString ReportWindow::GetConvertedTemp(float tempC)
{
    if (config->tempUnitString.IsSameAs("Farenheit"))
    {
        return (wxString::Format("%d", (int)roundf(tempC * (9.0f / 5.0f) + 32)));
    }

    config->tempUnitString = "Celsius";
    return (wxString::Format("%d", (int)roundf(tempC)));
}

wxString ReportWindow::GetConvertedTempUnit()
{
    if (config->tempUnitString.IsSameAs("Farenheit"))
    {
        /* TRANSLATORS: One letter abbreviation of the Farenheit temperature unit */
        return (_("F"));
    }

    /* TRANSLATORS: One letter abbreviation of the Celsius temperature unit */
    return (_("C"));
}

void ReportWindow::OnSaveAs(wxCommandEvent &)
{
    wxFileDialog saveFileDialog(this, _("Save weather report"), config->manualSavePath, GetReportBaseName() + ".txt",
                                _("Text File") + " (*.txt)|*.txt|" + _("Column Text File") + " (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    saveFileDialog.SetFilterIndex(config->manualSaveFormat);

    if (saveFileDialog.ShowModal() == wxID_OK)
    {
        wxFile file(saveFileDialog.GetPath(), wxFile::write);
        if (file.IsOpened())
        {
            if (saveFileDialog.GetFilterIndex() == 1)
            {
                file.Write(PrintWeatherColumnReports());
            }
            else
            {
                file.Write(PrintWeatherReports());
            }

            file.Close();
            config->manualSavePath   = saveFileDialog.GetDirectory();
            config->manualSaveFormat = saveFileDialog.GetFilterIndex();
        }
    }
}

wxString ReportWindow::GetReportBaseName()
{
    wxDateTime dateTime = wxDateTime::Now();
    return (wxString::Format(_("WForecast") + "_%d-%02d-%02d_%02dh%02dm%02ds", dateTime.GetYear(), dateTime.GetMonth() + 1, dateTime.GetDay(),
                             dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond()));
}

char ReportWindow::GetNextWaitingChar()
{
    static char progressChar[] = {'/', '-', '\\', '|'};

    return (progressChar[(progressCount++) % sizeof(progressChar)]);
}