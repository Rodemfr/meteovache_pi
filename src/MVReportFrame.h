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

#ifndef _MVREPORTFRAME_H__
#define _MVREPORTFRAME_H__

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <MeteoVacheThread.h>
#include <JobQueue.h>
#include <SpotForecasts.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/event.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class MVReportFrame: public wxDialog
{
private:
	wxStaticText *modelLabel;
	wxComboBox *modelSelector;
	wxTextCtrl *reportTextArea;
	wxButton *saveButton;
	MeteoVacheThread *workerThread;
	SpotForecasts spotForecast;
	JobQueue *jobQueue;
	wxString selectedString;
	wxString windUnitString;
	wxString tempUnitString;
	wxString autosavePath;
	bool autosaveEnable;
	bool autosaveColumn;
	bool autosaveCompress;
	wxString manualSavePath;
	int manualSaveFormat;

	virtual void OnClose(wxCloseEvent &event);
	virtual void OnModelSelect(wxCommandEvent &event);
	wxString GetLatitudeString(float latitude);
	wxString GetLongitudeString(float longitude);
	wxString GetTextDirection(float windDirectionDeg);
	wxString GetReportBaseName();

	void StartThread();
	void StopThread();

public:
	friend class MeteoVacheThread;

	MVReportFrame(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _("MeteoVache"), const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxSize(497, 445),
			long style = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxSTAY_ON_TOP | wxTAB_TRAVERSAL);
	~MVReportFrame();

	void SetManualSavePath(wxString path);
	void SetManualSaveFormat(int format);
	wxString GetManualSavePath();
	int GetManualSaveFormat();
	void SetAutosavePreferences(wxString path, bool enable, bool column, bool compress);
	void SetReportText(const wxString &text);
	void OnThreadEvent(wxCommandEvent&);
	void AutoSaveReport();
	wxString PrintWeatherReport(int model);
	wxString PrintWeatherReports();
	wxString PrintWeatherColumnReports();
	void RequestForecast(float latitude, float longitude);
	const wxString& GetSelectedModelName();
	void SetSelectedModelName(wxString modelName);
	void SetWindUnitString(wxString windUnitString);
	void SetTempUnitString(wxString windUnitString);
	void OnSaveAs(wxCommandEvent &event);

DECLARE_EVENT_TABLE()

private:
	wxString GetConvertedWind(float windSpeedKt);
	wxString GetConvertedTemp(float tempC);
	wxString GetConvertedTempId();
};

#endif //_MVREPORTFRAME_H__
