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

#include <MeteoVachePlugin.h>
#include <MVPrefDialog.h>
#include <icons.h>
#include <wx/mstream.h>
#include <wx/menuitem.h>
#include <wx/stdpaths.h>

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
void MVLogMessage1(wxString s)
{
	wxLogMessage
	("METEOVACHE: " + s);
}

extern "C" void MVLogMessage(const char *s)
{
	MVLogMessage1(wxString::FromAscii(s));
}

extern "C" DECL_EXP opencpn_plugin* create_pi(void *pluginManager)
{
	return new MeteoVachePlugin(pluginManager);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *pluginInstance)
{
	delete pluginInstance;
}

//---------------------------------------------------------------------------------------------------------
//
//    MeteoVache plug-in Implementation
//
//---------------------------------------------------------------------------------------------------------

MeteoVachePlugin::MeteoVachePlugin(void *pluginManager) :
		opencpn_plugin_18(pluginManager)
{
	toolBarIconId = 0;
	contextMenuId = 0;
	weatherReportFrame = nullptr;
	ocpnParentWindow = nullptr;
	contextMenu = nullptr;
	cursorLat = 0.0f;
	cursorLon = 0.0f;
	ocpnConfig = nullptr;
	pluginConfig.windowWidth = 500;
	pluginConfig.windowHeight = 500;
	pluginConfig.windowXPos = 500;
	pluginConfig.windowYPos = 500;

	// Create bitmap of the plug-in icon
	initialize_images();
	mvPluginIcon = _img_meteovache_pi;
	mvToolbarIcon = _img_meteovache_tb;

	pluginConfig.windUnitString = "kt";
	pluginConfig.tempUnitString = "C";

	wxStandardPaths standardPaths = wxStandardPaths::Get();
	pluginConfig.autosavePath = standardPaths.GetDocumentsDir();
}

MeteoVachePlugin::~MeteoVachePlugin()
{
	delete _img_meteovache_tb;
	delete _img_meteovache_pi;
}

int MeteoVachePlugin::Init(void)
{
	AddLocaleCatalog(PLUGIN_CATALOG_NAME);

	int plugInFlags = (WANTS_OVERLAY_CALLBACK |
	WANTS_CURSOR_LATLON |
	WANTS_TOOLBAR_CALLBACK |
	WANTS_CONFIG |
	WANTS_PREFERENCES);

	// Load configuration from OpenCPN configuration interface
	ocpnConfig = GetOCPNConfigObject();
	LoadConfig();

	// plug-in icon for the plug-in manager
	toolBarIconId = InsertPlugInTool(_("MeteoVache"), mvPluginIcon, mvPluginIcon, wxITEM_NORMAL, _("MeteoVache"), _("MeteoVache plug-in"),
	NULL, -1, 0, this);

	// Tool-bar icon
	SetToolbarToolBitmaps(toolBarIconId, mvToolbarIcon, mvToolbarIcon);
	plugInFlags |= INSTALLS_TOOLBAR_TOOL;

	// Create the context menu item
	contextMenu = new wxMenuItem(NULL, wxID_ANY, wxString(_("Weather forecast")),
			wxString(_("Request weather forecast from MeteoVache server at cursor location")), wxITEM_NORMAL,
			NULL);
	contextMenuId = AddCanvasContextMenuItem(contextMenu, this);
	plugInFlags |= INSTALLS_CONTEXTMENU_ITEMS;

	// Create the weather report window
	ocpnParentWindow = GetOCPNCanvasWindow();
	weatherReportFrame = new MVReportFrame(ocpnParentWindow, wxID_ANY, wxString(_("MeteoVache")), wxPoint(pluginConfig.windowXPos, pluginConfig.windowYPos),
			wxSize(pluginConfig.windowWidth, pluginConfig.windowHeight),
			wxDEFAULT_FRAME_STYLE);
	weatherReportFrame->SetPosition(wxPoint(pluginConfig.windowXPos, pluginConfig.windowYPos));
	weatherReportFrame->SetReportText(wxString(_("Right click on the map and select \"Weather forecast\" to get forecast at this point.")));
	weatherReportFrame->SetSelectedModelName(pluginConfig.selectedModelName);
	weatherReportFrame->SetWindUnitString(pluginConfig.windUnitString);
	weatherReportFrame->SetTempUnitString(pluginConfig.tempUnitString);
	weatherReportFrame->SetAutosavePreferences(pluginConfig.autosavePath, pluginConfig.autosaveEnable, pluginConfig.autosaveColumn, pluginConfig.autoSaveCompress);
	weatherReportFrame->SetManualSavePath(pluginConfig.manualSavePath);
	weatherReportFrame->SetManualSaveFormat(pluginConfig.manualSaveFormat);

	return plugInFlags;
}

bool MeteoVachePlugin::DeInit(void)
{
	// Save configuration to OpenCPN configuration interface
	SaveConfig();

	// Delete every allocated object
	delete weatherReportFrame;
	delete contextMenu;

	return true;
}

int MeteoVachePlugin::GetAPIVersionMajor()
{
	return PI_API_VERSION_MAJOR;
}

int MeteoVachePlugin::GetAPIVersionMinor()
{
	return PI_API_VERSION_MINOR;
}

int MeteoVachePlugin::GetPlugInVersionMajor()
{
	return PLUGIN_VERSION_MAJOR;
}

int MeteoVachePlugin::GetPlugInVersionMinor()
{
	return PLUGIN_VERSION_MINOR;
}

wxBitmap* MeteoVachePlugin::GetPlugInBitmap()
{
	return mvPluginIcon;
}

wxString MeteoVachePlugin::GetCommonName()
{
	return _("MeteoVache");
}

wxString MeteoVachePlugin::GetShortDescription()
{
	return _("MeteoVache weather plug-in for OpenCPN");
}

wxString MeteoVachePlugin::GetLongDescription()
{
	return _(
			"MeteoVache is a weather plug-in for OpenCPN.\n\
It provides weather forecasts everywhere in the world with only\n\
a low bandwidth Internet connection.");
}

void MeteoVachePlugin::OnToolbarToolCallback(int id)
{
	(void) id;
	// Clicking on the tool-bar icon switches report window visibility
	weatherReportFrame->Show(!weatherReportFrame->IsShown());
	weatherReportFrame->Layout();
}

int MeteoVachePlugin::GetToolbarToolCount(void)
{
	return 1;
}

void MeteoVachePlugin::OnContextMenuItemCallback(int id)
{
	(void) id;
	wxString text;

	// If MeteoVache context menu item has been clicked, we request a new forecast for the last recorded cursor position
	if (id == contextMenuId)
	{
		weatherReportFrame->RequestForecast(cursorLat, cursorLon);
	}
}

void MeteoVachePlugin::SetCursorLatLon(double lat, double lon)
{
	// Called each time mouse position changes. We store the latest value
	cursorLat = (float) lat;
	cursorLon = (float) lon;
}

bool MeteoVachePlugin::LoadConfig(void)
{
	wxFileConfig *pConf = (wxFileConfig*) ocpnConfig;

	if (pConf)
	{
		pConf->SetPath("/Settings/MeteoVache");
		pConf->Read("WindowWidth", &(pluginConfig.windowWidth), 500);
		pConf->Read("WindowHeight", &(pluginConfig.windowHeight), 500);
		pConf->Read("WindowXPos", &(pluginConfig.windowXPos), 250);
		pConf->Read("WindowYPos", &(pluginConfig.windowYPos), 250);
		pConf->Read("ModelName", &(pluginConfig.selectedModelName), "");
		pConf->Read("WindUnitString", &(pluginConfig.windUnitString), "kt");
		pConf->Read("TempUnitString", &(pluginConfig.tempUnitString), "C");
		pConf->Read("AutoSavePath", &(pluginConfig.autosavePath), pluginConfig.autosavePath);
		pConf->Read("AutoSaveEnable", &(pluginConfig.autosaveEnable), false);
		pConf->Read("AutoSaveColumn", &(pluginConfig.autosaveColumn), false);
		pConf->Read("AutoSaveCompress", &(pluginConfig.autoSaveCompress), true);
		pConf->Read("ManualSavePath", &(pluginConfig.manualSavePath), "");
		pConf->Read("ManualSaveFormat", &(pluginConfig.manualSaveFormat), 0);

		return true;;
	} else
		return false;
}

bool MeteoVachePlugin::SaveConfig(void)
{
	wxFileConfig *pConf = (wxFileConfig*) ocpnConfig;

	if (pConf)
	{
		pConf->SetPath("/Settings/MeteoVache");
		pConf->Write("WindowWidth", weatherReportFrame->GetSize().x);
		pConf->Write("WindowHeight", weatherReportFrame->GetSize().y);
		pConf->Write("WindowXPos", weatherReportFrame->GetPosition().x);
		pConf->Write("WindowYPos", weatherReportFrame->GetPosition().y);
		pConf->Write("ModelName", weatherReportFrame->GetSelectedModelName());
		pConf->Write("WindUnitString", pluginConfig.windUnitString);
		pConf->Write("TempUnitString", pluginConfig.tempUnitString);
		pConf->Write("AutoSavePath", pluginConfig.autosavePath);
		pConf->Write("AutoSaveEnable", pluginConfig.autosaveEnable);
		pConf->Write("AutoSaveColumn", pluginConfig.autosaveColumn);
		pConf->Write("AutoSaveCompress", pluginConfig.autoSaveCompress);
		pConf->Write("ManualSavePath", weatherReportFrame->GetManualSavePath());
		pConf->Write("ManualSaveFormat", weatherReportFrame->GetManualSaveFormat());
		return true;
	} else
		return false;
}

void MeteoVachePlugin::ShowPreferencesDialog(wxWindow *parent)
{
	MVPrefDialog *prefDialog = new MVPrefDialog(parent, wxID_ANY, _("MeteoVache preferences"), wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_DIALOG_STYLE);

	prefDialog->SetUnitPreferences(pluginConfig.windUnitString, pluginConfig.tempUnitString);
	prefDialog->SetAutosavePreferences(pluginConfig.autosavePath, pluginConfig.autosaveEnable, pluginConfig.autosaveColumn, pluginConfig.autoSaveCompress);
	prefDialog->Fit();
	prefDialog->Layout();

	if (prefDialog->ShowModal() == wxID_OK)
	{
		pluginConfig.windUnitString = prefDialog->GetWindUnitString();
		pluginConfig.tempUnitString = prefDialog->GetTempUnitString();
		weatherReportFrame->SetWindUnitString(pluginConfig.windUnitString);
		weatherReportFrame->SetTempUnitString(pluginConfig.tempUnitString);
		pluginConfig.autosavePath = prefDialog->GetAutosavePath();
		pluginConfig.autosaveEnable = prefDialog->GetAutosaveEnable();
		pluginConfig.autosaveColumn = prefDialog->GetAutosaveColumn();
		pluginConfig.autoSaveCompress = prefDialog->GetAutosaveCompress();
		weatherReportFrame->SetAutosavePreferences(pluginConfig.autosavePath, pluginConfig.autosaveEnable, pluginConfig.autosaveColumn, pluginConfig.autoSaveCompress);
	}

	delete prefDialog;
}

