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

#include <stdint.h>
#include <MVPrefDialog.h>
#include <wx/dirdlg.h>

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

MVPrefDialog::MVPrefDialog(wxWindow *parent, ConfigContainer *config, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) :
		wxDialog(parent, id, title, pos, size, style)
{
	wxString windUnits[] =
	{ _("kt"), _("bft"), _("m/s"), _("kph"), _("mph") };
	wxString tempUnits[] =
	{ _("Celsius"), _("Farenheit") };

	this->config = config;
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer *globalSizer = new wxBoxSizer(wxVERTICAL);

	// Unit configuration
	wxStaticBoxSizer *unitSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Units"));
	wxBoxSizer *windUnitSizer = new wxBoxSizer(wxHORIZONTAL);
	windUnitLabel = new wxStaticText(unitSizer->GetStaticBox(), wxID_ANY, _("Wind unit"), wxDefaultPosition, wxDefaultSize, 0);
	windUnitLabel->Wrap(-1);
	windUnitSizer->Add(windUnitLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	windUnitSelection = new wxComboBox(unitSizer->GetStaticBox(), wxID_ANY, _("kt"), wxDefaultPosition, wxDefaultSize, 5, windUnits, 0);
	windUnitSizer->Add(windUnitSelection, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	unitSizer->Add(windUnitSizer, 0, wxEXPAND, 5);

	wxBoxSizer *tempUnitSizer = new wxBoxSizer(wxHORIZONTAL);
	tempUnitLabel = new wxStaticText(unitSizer->GetStaticBox(), wxID_ANY, _("Temperature unit"), wxDefaultPosition, wxDefaultSize, 0);
	tempUnitLabel->Wrap(-1);
	tempUnitSizer->Add(tempUnitLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	tempUnitSelection = new wxComboBox(unitSizer->GetStaticBox(), wxID_ANY, _("Celsius"), wxDefaultPosition, wxDefaultSize, 2, tempUnits, 0);
	tempUnitSizer->Add(tempUnitSelection, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	unitSizer->Add(tempUnitSizer, 0, wxEXPAND, 5);
	unitSizer->Add(0, 1, wxEXPAND, 5);
	globalSizer->Add(unitSizer, 0, wxALL | wxEXPAND, 5);

	// Display
	wxStaticBoxSizer *displaySizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Display")), wxHORIZONTAL);
	timeZoneLabel = new wxStaticText(displaySizer->GetStaticBox(), wxID_ANY, _("Time zone"), wxDefaultPosition, wxDefaultSize, 0);
	timeZoneLabel->Wrap(-1);
	displaySizer->Add(timeZoneLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	timeZoneSelection = new wxComboBox(displaySizer->GetStaticBox(), wxID_ANY, _("Local / system"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	timeZoneSelection->Append(_("Local / system"));
	timeZoneSelection->Append(_("UTC"));
	displaySizer->Add(timeZoneSelection, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	globalSizer->Add(displaySizer, 0, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Auto save configuration
	wxStaticBoxSizer *autosaveSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Auto save")), wxVERTICAL);
	wxBoxSizer *autosavePathSizer = new wxBoxSizer(wxHORIZONTAL);
	autosavePathLabel = new wxStaticText(autosaveSizer->GetStaticBox(), wxID_ANY, _("Save directory"), wxDefaultPosition, wxDefaultSize, 0);
	autosavePathLabel->Wrap(-1);
	autosavePathSizer->Add(autosavePathLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	autosavePathEdit = new wxTextCtrl(autosaveSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	autosavePathSizer->Add(autosavePathEdit, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	autosavePathEditButton = new wxButton(autosaveSizer->GetStaticBox(), wxID_ANY, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT);
	autosavePathSizer->Add(autosavePathEditButton, 0, wxALIGN_CENTER_VERTICAL, 5);
	autosaveSizer->Add(autosavePathSizer, 0, wxEXPAND, 5);

	wxBoxSizer *autosaveCheckboxSizer = new wxBoxSizer(wxHORIZONTAL);
	autosaveCheckboxSizer->Add(0, 0, 1, wxEXPAND, 5);
	autosaveEnableCheckbox = new wxCheckBox(autosaveSizer->GetStaticBox(), wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0);
	autosaveCheckboxSizer->Add(autosaveEnableCheckbox, 0, wxALL, 5);
	autosaveColumnCheckbox = new wxCheckBox(autosaveSizer->GetStaticBox(), wxID_ANY, _("Column format"), wxDefaultPosition, wxDefaultSize, 0);
	autosaveCheckboxSizer->Add(autosaveColumnCheckbox, 0, wxALL, 5);
	autosaveCompressCheckbox = new wxCheckBox(autosaveSizer->GetStaticBox(), wxID_ANY, _("Compress"), wxDefaultPosition, wxDefaultSize, 0);
	autosaveCheckboxSizer->Add(autosaveCompressCheckbox, 0, wxALL, 5);
	autosaveSizer->Add(autosaveCheckboxSizer, 1, wxEXPAND, 5);
	globalSizer->Add(autosaveSizer, 0, wxBOTTOM | wxEXPAND | wxLEFT | wxRIGHT, 5);

	// Server preferences
	wxStaticBoxSizer *serverSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Server")), wxVERTICAL);
	wxBoxSizer *serverPrefSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *serverLabelSizer = new wxBoxSizer(wxVERTICAL);
	serverNameLabel = new wxStaticText(serverSizer->GetStaticBox(), wxID_ANY, _("Server name"), wxDefaultPosition, wxDefaultSize, 0);
	serverNameLabel->Wrap(-1);
	serverLabelSizer->Add(serverNameLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	serverPortLabel = new wxStaticText(serverSizer->GetStaticBox(), wxID_ANY, _("Server port"), wxDefaultPosition, wxDefaultSize, 0);
	serverPortLabel->Wrap(-1);
	serverLabelSizer->Add(serverPortLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	serverPrefSizer->Add(serverLabelSizer, 1, wxEXPAND, 5);
	wxBoxSizer *serverPortSizer = new wxBoxSizer(wxVERTICAL);
	serverNameEdit = new wxTextCtrl(serverSizer->GetStaticBox(), wxID_ANY, "meteovache.dyndns.org", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
	serverPortSizer->Add(serverNameEdit, 1, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5);
	serverPortEdit = new wxTextCtrl(serverSizer->GetStaticBox(), wxID_ANY, "31837", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
	serverPortSizer->Add(serverPortEdit, 1, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5);
	serverPrefSizer->Add(serverPortSizer, 3, wxEXPAND, 5);
	serverSizer->Add(serverPrefSizer, 0, wxEXPAND, 5);
	serverSizer->Add(0, 1, wxEXPAND, 5);
	globalSizer->Add(serverSizer, 0, wxALL | wxEXPAND, 5);
	globalSizer->Add(0, 1, wxEXPAND, 5);

	serverNameEdit->Disable();
	serverPortEdit->Disable();

	// Standard buttons
	wxStdDialogButtonSizer *stdButtonsSizer = new wxStdDialogButtonSizer();
	wxButton *prefOkButton = new wxButton(this, wxID_OK);
	stdButtonsSizer->AddButton(prefOkButton);
	wxButton *prefCancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"));
	stdButtonsSizer->AddButton(prefCancelButton);
	stdButtonsSizer->Realize();
	globalSizer->Add(stdButtonsSizer, 0, wxBOTTOM, 5);

	this->SetSizer(globalSizer);

	windUnitSelection->SetStringSelection(_(config->windUnitString));
	tempUnitSelection->SetStringSelection(_(config->tempUnitString));
	timeZoneSelection->SetStringSelection(_(config->timeZoneString));
	autosavePathEdit->SetValue(config->autosavePath);
	autosaveEnableCheckbox->SetValue(config->autoSaveEnable);
	autosaveColumnCheckbox->SetValue(config->autoSaveColumn);
	autosaveCompressCheckbox->SetValue(config->autoSaveCompress);

	this->Layout();
	globalSizer->Fit(this);

	this->Centre(wxBOTH);

	autosavePathEditButton->Connect(wxEVT_BUTTON, wxCommandEventHandler(MVPrefDialog::onAutosavePathBrowse), NULL, this);
}

MVPrefDialog::~MVPrefDialog()
{
}

void MVPrefDialog::UpdateConfig()
{
	// wind unit
	wxString translatedString = windUnitSelection->GetStringSelection();
	if (translatedString.IsSameAs(_("kt")))
	{
		config->windUnitString = "kt";
	} else if (translatedString.IsSameAs(_("bft")))
	{
		config->windUnitString = "bft";
	} else if (translatedString.IsSameAs(_("m/s")))
	{
		config->windUnitString = "m/s";
	} else if (translatedString.IsSameAs(_("kph")))
	{
		config->windUnitString = "kph";
	} else if (translatedString.IsSameAs(_("mph")))
	{
		config->windUnitString = "mph";
	} else
	{
		config->windUnitString = "kt";
	}
	// Temperature unit
	translatedString = tempUnitSelection->GetStringSelection();
	if (translatedString.IsSameAs(_("Celsius")))
	{
		config->tempUnitString = "Celsius";
	} else if (translatedString.IsSameAs(_("Farenheit")))
	{
		config->tempUnitString = "Farenheit";
	} else
	{
		config->tempUnitString = "Celsius";
	}
	// Time zone selection
	translatedString = timeZoneSelection->GetStringSelection();
	if (translatedString.IsSameAs(_("Local / system")))
	{
		config->timeZoneString = "Local / system";
	} else if (translatedString.IsSameAs(_("UTC")))
	{
		config->timeZoneString = "UTC";
	} else
	{
		config->timeZoneString = "Local / system";
	}
	// Autosave options
	config->autosavePath = autosavePathEdit->GetValue();
	config->autoSaveEnable = autosaveEnableCheckbox->GetValue();
	config->autoSaveColumn = autosaveColumnCheckbox->GetValue();
	config->autoSaveCompress = autosaveCompressCheckbox->GetValue();
}

void MVPrefDialog::onAutosavePathBrowse(wxCommandEvent&)
{
	wxDirDialog selectDirDialog(NULL, _("Select automatic save directory"), autosavePathEdit->GetValue(), wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (selectDirDialog.ShowModal() == wxID_OK)
	{
		autosavePathEdit->SetValue(selectDirDialog.GetPath());
	}
}
