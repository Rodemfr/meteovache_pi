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

#include <MVReportFrame.h>
#include <MeteoVacheThread.h>
#include <JobQueue.h>
#include <DateTime.h>
#include <stdint.h>
#include <wx/log.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

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
wxBEGIN_EVENT_TABLE(MVReportFrame, wxDialog) EVT_COMMAND(wxID_ANY, wxEVT_THREAD_JOB_COMPLETED, MVReportFrame::OnThreadEvent)
wxEND_EVENT_TABLE()

MVReportFrame::MVReportFrame(
		wxWindow *parent,
		wxWindowID id,
		const wxString &title,
		const wxPoint &pos,
		const wxSize &size,
		long style) :
		wxDialog(parent, id, title, pos, size, style)
{
	workerThread = nullptr;

	jobQueue = new JobQueue(GetEventHandler());

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));

	wxBoxSizer *MVReportGlobalSizer;
	MVReportGlobalSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *MVReportTopSizer;
	MVReportTopSizer = new wxBoxSizer(wxHORIZONTAL);

	MVReportModelLabel = new wxStaticText(this, wxID_ANY, _("Weather model :"), wxDefaultPosition, wxDefaultSize, 0);
	MVReportModelLabel->Wrap(-1);
	MVReportTopSizer->Add(MVReportModelLabel, 0, wxALIGN_CENTER | wxALL, 5);

	MVReportModelSelector = new wxComboBox(this, wxID_ANY, _("No data"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	MVReportTopSizer->Add(MVReportModelSelector, 0, wxALL, 5);

	MVReportGlobalSizer->Add(MVReportTopSizer, 0, wxEXPAND, 5);

	MVReportTextArea = new wxTextCtrl(this, wxID_ANY, _("Weather report"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	wxFont reportFont = MVReportTextArea->GetFont();
	reportFont.SetFamily(wxFONTFAMILY_TELETYPE);
	MVReportTextArea->SetFont(reportFont);
	MVReportGlobalSizer->Add(MVReportTextArea, 1, wxALL | wxEXPAND, 5);

	this->SetSizer(MVReportGlobalSizer);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MVReportFrame::MVReportFrameOnClose));
	MVReportModelSelector->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(MVReportFrame::MVModelOnSelect), NULL, this);

	startThread();
}

MVReportFrame::~MVReportFrame()
{
	stopThread();
	// Disconnect Events
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MVReportFrame::MVReportFrameOnClose));
	MVReportModelSelector->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(MVReportFrame::MVModelOnSelect), NULL, this);
	// FIXME : delete all objects created in constructor
	delete jobQueue;
}

void MVReportFrame::MVReportFrameOnClose(
		wxCloseEvent &event)
{
	this->Show(false);
	event.WasProcessed();
}

void MVReportFrame::MVModelOnSelect(
		wxCommandEvent &event)
{
	selectedString = MVReportModelSelector->GetStringSelection();
	PublishWeatherReport(MVReportModelSelector->GetSelection());
	event.WasProcessed();
}

void MVReportFrame::SetReportText(
		const wxString &text)
{
	MVReportTextArea->Remove(0, -1);
	MVReportTextArea->WriteText(text);
	MVReportTextArea->SetInsertionPoint(0);
}

void MVReportFrame::startThread()
{
	stopThread();
	workerThread = new MeteoVacheThread(this, jobQueue);
	if (workerThread->Run() != wxTHREAD_NO_ERROR)
	{
		delete workerThread;
		workerThread = NULL;
		wxLogError
		( "MeteoVache: Can't create the working thread!");
	}
}

void MVReportFrame::stopThread()
{
	if (workerThread != nullptr)
	{
		if (workerThread->Delete(NULL, wxTHREAD_WAIT_BLOCK) != wxTHREAD_NO_ERROR)
		{
			wxLogError
			( "MeteoVache: Can't delete background thread");
		} else
		{
			workerThread = nullptr;
		}
	}
}

void MVReportFrame::OnThreadEvent(
		wxCommandEvent&)
{
	spotForecast.Lock();
	wxString newString;

	MVReportModelSelector->Clear();
	for (uint32_t i = 0; i < spotForecast.GetNumberOfForecast(); i++)
	{
		newString = wxString(spotForecast.Get(i).getModelName());
		MVReportModelSelector->Append(newString);
		if (i == 0)
		{
			MVReportModelSelector->SetSelection(0);
		} else if (newString.IsSameAs(selectedString))
		{
			MVReportModelSelector->SetSelection(i);
		}
	}
	selectedString = MVReportModelSelector->GetStringSelection();
	PublishWeatherReport(MVReportModelSelector->GetSelection());

	spotForecast.Unlock();
}

void MVReportFrame::PublishWeatherReport(
		int model)
{
	wxString modelInfo;
	Forecast *forecast;
	WeatherData data;
	float cloudCover;

	// FIXME : Use a local copy of forecasts instead
	spotForecast.Lock();

	forecast = &spotForecast.Get(model);

	DateTime runDate;
	runDate.SetTimeCode(forecast->getRunTimeCode());

	modelInfo = modelInfo.Append(
			wxString::Format(_("Position :       %s %s\n"), getLatitudeString(spotForecast.GetLatitude()), getLongitudeString(spotForecast.GetLongitude())));
	modelInfo = modelInfo.Append(wxString::Format(_("Model :          %s (%s)\n"), forecast->getModelName(), forecast->getProviderName()));
	modelInfo = modelInfo.Append(
			wxString::Format(_("Run date :       %02d/%02d/%d %dh%02d\n\n"), runDate.GetLocalDay(), runDate.GetLocalMonth(), runDate.GetLocalYear(),
					runDate.GetLocalHour(), runDate.GetLocalMinute()));
	modelInfo = modelInfo.Append(_("           Wind Gust   Dir  Rain Cloud Temp\n"));
	modelInfo = modelInfo.Append(_("             kt   kt        mm/h     %    C\n"));

	DateTime stepTime = runDate;
	std::string dayName;
	for (int step = 0; step < forecast->getNumberOfSteps(); step += 1)
	{
		stepTime.AddHours(forecast->getTimeStepInHours());
		dayName = stepTime.GetLocalDayName();

		modelInfo = modelInfo.Append(wxString::Format("%c%c.%02d  %2dh  ", dayName[0], dayName[1], stepTime.GetLocalDay(), stepTime.GetLocalHour()));
		data = forecast->getForecastData(step);
		cloudCover = data.lowCloudCoverPer;
		if (data.midCloudCoverPer * 0.66f > cloudCover)
			cloudCover = data.midCloudCoverPer * 0.66f;
		if (data.highCloudCoverPer * 0.33f > cloudCover)
			cloudCover = data.highCloudCoverPer * 0.33f;
		wxString precipitationString = wxString::Format("%4.1f", data.precipitationMmH);
		if (data.precipitationMmH < 0.05)
		{
			precipitationString = "    ";
		}
		// TODO : convert m/s to kts
		modelInfo = modelInfo.Append(
				wxString::Format("%3.0f  %3.0f  %s  %s   %3.0f  %3.0f\n", data.windSpeedKt, data.gustSpeedKt, getTextDirection(data.windDirectionDeg),
						precipitationString, cloudCover, data.TemperatureC));
	}
	modelInfo = modelInfo.Append("\n");
	spotForecast.Unlock();

	SetReportText(modelInfo);
	Show();
	Layout();
}

void MVReportFrame::RequestForecast(
		float latitude,
		float longitude)
{
	JobRequest job(JobRequest::CMD_GET_ALL_FORECASTS_AT_LOCATION, latitude, longitude);
	jobQueue->addJob(job);
}

wxString MVReportFrame::getLatitudeString(
		float latitude)
{
	wxString latitudeString;

	int degs = (int) roundf(floorf(fabsf(latitude)));
	float mins = (fabsf(latitude) - degs) * 60.0f;
	latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (latitude < 0.0f ? _("S") : _("N")), degs, mins);

	return (latitudeString);

}

wxString MVReportFrame::getLongitudeString(
		float longitude)
{
	wxString latitudeString;
	int degs = (int) roundf(floorf(fabsf(longitude)));
	float mins = (fabsf(longitude) - degs) * 60.0f;

	latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (longitude < 0.0f ? _("W") : _("E")), degs, mins);

	return (latitudeString);
}

wxString MVReportFrame::getTextDirection(
		float windDirectionDeg)
{
	const float angleStep = 22.5f;
	float angle = angleStep / 2;

	if (windDirectionDeg < 0.0f)
		windDirectionDeg += 360.0f;
	if (windDirectionDeg < angle)
		return (_("   S"));
	else if (windDirectionDeg < angle + angleStep)
		return (_("S-SW"));
	else if (windDirectionDeg < angle + 2 * angleStep)
		return (_("  SW"));
	else if (windDirectionDeg < angle + 3 * angleStep)
		return (_("W-SW"));
	else if (windDirectionDeg < angle + 4 * angleStep)
		return (_("   W"));
	else if (windDirectionDeg < angle + 5 * angleStep)
		return (_("W-NW"));
	else if (windDirectionDeg < angle + 6 * angleStep)
		return (_("  NW"));
	else if (windDirectionDeg < angle + 7 * angleStep)
		return (_("N-NW"));
	else if (windDirectionDeg < angle + 8 * angleStep)
		return (_("   N"));
	else if (windDirectionDeg < angle + 9 * angleStep)
		return (_("N-NE"));
	else if (windDirectionDeg < angle + 10 * angleStep)
		return (_("  NE"));
	else if (windDirectionDeg < angle + 11 * angleStep)
		return (_("E-NE"));
	else if (windDirectionDeg < angle + 12 * angleStep)
		return (_("   E"));
	else if (windDirectionDeg < angle + 13 * angleStep)
		return (_("E-SE"));
	else if (windDirectionDeg < angle + 14 * angleStep)
		return (_("  SE"));
	else if (windDirectionDeg < angle + 15 * angleStep)
		return (_("S-SE"));
	else
		return (_("   N"));
}

const wxString& MVReportFrame::getSelectedModelName()
{
	return (selectedString);
}

void MVReportFrame::setSelectedModelName(
		wxString modelName)
{
	selectedString = modelName;
}
