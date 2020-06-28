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

#include <NetworkThread.h>
#include <JobQueue.h>
#include <SpotForecasts.h>
#include <ConfigContainer.h>
#include <ForecastDisplay.h>
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

class ReportWindow: public wxDialog
{
private:
	DECLARE_EVENT_TABLE()

	// Widgets
	wxStaticText *statusLabel;
	wxComboBox *modelSelector;
	ForecastDisplay *reportTextArea;
	wxButton *saveButton;

	ConfigContainer *config;	  // Configuration parameters
	NetworkThread *workerThread;  // Thread making the network job
	SpotForecasts spotForecast;   // Forecast data retrieved by the network thread
	JobQueue *jobQueue;           // JobQueue used to dialog safely with the network thread
	int progressCount;            // A simple counter to animate the status message in case of delayed server response

	wxString GetLatitudeString(float latitude);        // Change a float latitude in a display-able string
	wxString GetLongitudeString(float longitude);      // Change a float longitude in a display-able string
	wxString GetTextDirection(float windDirectionDeg); // Returns a display-able string of direction from a float angle
	wxString GetReportBaseName();                      // Generate a report filename from the current date and time
	void AutoSaveReport();                             // Save the current report with the appropriate options
	wxString PrintWeatherReport(int model);            // Print a single forecast model in a string
	wxString PrintWeatherReports();                    // Print all forecast models in a serialized string
	wxString PrintWeatherColumnReports();              // Print all forecast models in column formatted string
	wxString GetConvertedWind(float windSpeedKt);      // Get wind speed string converted in the appropriate unit
	wxString GetConvertedTemp(float tempC);            // Get temperature string converted in the appropriate unit
	wxString GetConvertedTempUnit();                   // Get the temperature unit symbol
	char GetNextWaitingChar();                         // Get the next animated char for out low-cost progress indicator

	void StartThread(); // Start the network thread
	void StopThread();  // Stop the network thread

	// Callback
	void OnThreadEvent(wxCommandEvent&);               // Called each time network thread has something to tell to the report window
	virtual void OnClose(wxCloseEvent &event);         // Called when the close button of the report window is clicked
	virtual void OnModelSelect(wxCommandEvent &event); // Called when the model is changed in the combo box
	void OnSaveAs(wxCommandEvent &event);              // Called when the "Save As" buton is clicked

public:
	ReportWindow(wxWindow *parent, ConfigContainer *config, wxWindowID id = wxID_ANY, const wxString &title = _("MeteoVache"), const wxPoint &pos =
			wxDefaultPosition, const wxSize &size = wxSize(497, 445),
			long style = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxSTAY_ON_TOP | wxTAB_TRAVERSAL);
	~ReportWindow();

	void UpdateConfig();                                    // Save all parameters related to the report window into the configuration object
	void SetReportForecast(int modelSelection);             // Change the text displayed by the report window
	void RequestForecast(float latitude, float longitude);  // Start a new forecast query

};

#endif //_MVREPORTFRAME_H__
