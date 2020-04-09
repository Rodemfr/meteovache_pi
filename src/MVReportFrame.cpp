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
#include <wx/File.h>
#include <wx/filedlg.h>
#include <math.h>

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
EVT_BUTTON ( BUTTON_SaveAs, MVReportFrame::OnSaveAs )
wxEND_EVENT_TABLE()

MVReportFrame::MVReportFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) :
		wxDialog(parent, id, title, pos, size, style)
{
	workerThread = nullptr;
	windUnitString = _("kt");

	jobQueue = new JobQueue(GetEventHandler());

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT));

	wxBoxSizer *MVReportGlobalSizer;
	MVReportGlobalSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *MVReportTopSizer = new wxBoxSizer(wxHORIZONTAL);

	MVReportModelLabel = new wxStaticText(this, wxID_ANY, _("Weather model :"), wxDefaultPosition, wxDefaultSize, 0);
	MVReportModelLabel->Wrap(-1);
	MVReportTopSizer->Add(MVReportModelLabel, 0, wxALIGN_CENTER | wxALL, 5);

	MVReportModelSelector = new wxComboBox(this, wxID_ANY, _("No data"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	MVReportTopSizer->Add(MVReportModelSelector, 0, wxLEFT | wxRIGHT | wxTOP, 5);

	MVReportGlobalSizer->Add(MVReportTopSizer, 0, wxEXPAND, 5);

	MVReportTextArea = new wxTextCtrl(this, wxID_ANY, _("Weather report"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	wxFont reportFont = MVReportTextArea->GetFont();
	reportFont.SetFamily(wxFONTFAMILY_TELETYPE);
	MVReportTextArea->SetFont(reportFont);
	MVReportGlobalSizer->Add(MVReportTextArea, 1, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 5);

	wxBoxSizer *MVButtonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton *MVReportSaveButton = new wxButton(this, BUTTON_SaveAs, _("Save As..."));
	MVButtonSizer->Add(MVReportSaveButton, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 5);
	MVReportGlobalSizer->Add(MVButtonSizer, 0, wxEXPAND, 5);

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

void MVReportFrame::MVReportFrameOnClose(wxCloseEvent &event)
{
	this->Show(false);
	event.WasProcessed();
}

void MVReportFrame::MVModelOnSelect(wxCommandEvent &event)
{
	selectedString = MVReportModelSelector->GetStringSelection();
	SetReportText(PublishWeatherReport(MVReportModelSelector->GetSelection()));
	Show();
	Layout();
	event.WasProcessed();
}

void MVReportFrame::SetReportText(const wxString &text)
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

void MVReportFrame::OnThreadEvent(wxCommandEvent&)
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
	SetReportText(PublishWeatherReport(MVReportModelSelector->GetSelection()));
	Show();
	Layout();

	spotForecast.Unlock();
}

wxString MVReportFrame::PublishWeatherReport(int model)
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
	modelInfo = modelInfo.Append(wxString::Format("           %4s %4s %5s %5s %5s %4s\n", _("Wind"), _("Gust"), _("Dir"), _("Rain"), _("Cloud"), _("Temp")));
	modelInfo = modelInfo.Append(
			wxString::Format("           %4s %4s %5s %5s %5s %4s\n", _(windUnitString), _(windUnitString), " ", _("mm/h"), "%", GetConvertedTempId()));

	DateTime stepTime = runDate;
	std::string dayName;
	for (int step = 0; step < forecast->getNumberOfSteps(); step += 1)
	{
		stepTime.AddHours(forecast->getTimeStepInHours());
		dayName = stepTime.GetLocalDayName();

		modelInfo = modelInfo.Append(wxString::Format("%c%c.%02d  %2dh ", dayName[0], dayName[1], stepTime.GetLocalDay(), stepTime.GetLocalHour()));
		data = forecast->getForecastData(step);
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
		modelInfo = modelInfo.Append(
				wxString::Format("%4s %4s %5s %5s   %3.0f %4s\n", GetConvertedWind(data.windSpeedKt).c_str(), GetConvertedWind(data.gustSpeedKt).c_str(),
						getTextDirection(data.windDirectionDeg), precipitationString, cloudCover, GetConvertedTemp(data.TemperatureC).c_str()));
	}
	modelInfo = modelInfo.Append("\n");
	spotForecast.Unlock();

	return (modelInfo);
}

void MVReportFrame::RequestForecast(float latitude, float longitude)
{
	JobRequest job(JobRequest::CMD_GET_ALL_FORECASTS_AT_LOCATION, latitude, longitude);
	jobQueue->addJob(job);
}

wxString MVReportFrame::getLatitudeString(float latitude)
{
	wxString latitudeString;

	int degs = (int) roundf(floorf(fabsf(latitude)));
	float mins = (fabsf(latitude) - degs) * 60.0f;
	latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (latitude < 0.0f ? _("S") : _("N")), degs, mins);

	return (latitudeString);

}

wxString MVReportFrame::getLongitudeString(float longitude)
{
	wxString latitudeString;
	int degs = (int) roundf(floorf(fabsf(longitude)));
	float mins = (fabsf(longitude) - degs) * 60.0f;

	latitudeString = wxString::Format(wxString::FromUTF8("%s%d°%02.1f'"), (longitude < 0.0f ? _("W") : _("E")), degs, mins);

	return (latitudeString);
}

wxString MVReportFrame::getTextDirection(float windDirectionDeg)
{
	const float angleStep = 22.5f;
	float angle = angleStep / 2;

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

const wxString& MVReportFrame::GetSelectedModelName()
{
	return (selectedString);
}

void MVReportFrame::SetSelectedModelName(wxString modelName)
{
	selectedString = modelName;
}

void MVReportFrame::SetWindUnitString(wxString windUnitString)
{
	this->windUnitString = windUnitString;
}

void MVReportFrame::SetTempUnitString(wxString tempUnitString)
{
	this->tempUnitString = tempUnitString;
}

wxString MVReportFrame::GetConvertedWind(float windSpeedKt)
{
	if (this->windUnitString.IsSameAs("kt"))
	{
		return (wxString::Format("%d", (int) roundf(windSpeedKt)));
	} else if (this->windUnitString.IsSameAs("bft"))
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
	} else if (this->windUnitString.IsSameAs("m/s"))
	{
		return (wxString::Format("%d", (int) roundf(windSpeedKt / 1.94384f)));
	} else if (this->windUnitString.IsSameAs("kph"))
	{
		return (wxString::Format("%d", (int) roundf(windSpeedKt * 1.852f)));
	} else if (this->windUnitString.IsSameAs("mph"))
	{
		return (wxString::Format("%d", (int) roundf(windSpeedKt * 1.15078f)));
	}

	this->windUnitString = "kt";
	return (wxString::Format("%d", (int) roundf(windSpeedKt)));
}

wxString MVReportFrame::GetConvertedTemp(float tempC)
{
	if (this->tempUnitString.IsSameAs("Farenheit"))
	{
		return (wxString::Format("%d", (int) roundf(tempC * (9.0f / 5.0f) + 32)));
	}

	this->tempUnitString = "Celsius";
	return (wxString::Format("%d", (int) roundf(tempC)));
}

wxString MVReportFrame::GetConvertedTempId()
{
	if (this->tempUnitString.IsSameAs("Farenheit"))
	{
		return (_("F"));
	}

	return (_("C"));
}

void MVReportFrame::OnSaveAs(wxCommandEvent&)
{
	wxDateTime dateTime = wxDateTime::Now();
	wxString defaultFileName = wxString::Format(_("Forecast") + "_%d-%02d-%02d_%02dh%02dm%02ds.txt", dateTime.GetYear(), dateTime.GetMonth(), dateTime.GetDay(),
			dateTime.GetHour(), dateTime.GetMinute(), dateTime.GetSecond());
	wxFileDialog saveFileDialog(this, _("Save weather report"), "", defaultFileName,
	_("Text File") + " (*.txt)|*.txt|" + _("Column Text File") + " (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	uint32_t nbReports = spotForecast.GetNumberOfForecast();
	wxString *reportArray = new wxString[nbReports];

	if (saveFileDialog.ShowModal() == wxID_OK)
	{
		wxFile file(saveFileDialog.GetPath(), wxFile::write);
		if (file.IsOpened())
		{
			spotForecast.Lock();
			for (uint32_t i = 0; i < nbReports; i++)
			{
				reportArray[i] = PublishWeatherReport(i);
			}
			spotForecast.Unlock();

			if (saveFileDialog.GetFilterIndex() == 1)
			{
				bool continueLoop;
				int crPos;
				do
				{
					continueLoop = false;
					for (uint32_t i = 0; i < nbReports; i++)
					{
						file.Write(wxString::Format("%-55s", reportArray[i].BeforeFirst('\n')));
						reportArray[i] = reportArray[i].AfterFirst('\n');
						if (reportArray[i].length() > 0)
						{
							continueLoop = true;
						}
					}
					file.Write('\n');
				} while (continueLoop);
			} else
			{
				for (uint32_t i = 0; i < nbReports; i++)
				{
					file.Write(reportArray[i]);
				}
			}

			file.Close();
		}
	}

	delete[] reportArray;

}

