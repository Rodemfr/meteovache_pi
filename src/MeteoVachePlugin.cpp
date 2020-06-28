/***************************************************************************
 #include <Icons.h>
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
#include <PreferenceDialog.h>
#include <Icons.h>
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

extern "C" DECL_EXP opencpn_plugin* create_pi(void *pluginManager)
{
	return new MeteoVachePlugin(pluginManager);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *pluginInstance)
{
	delete pluginInstance;
}

MeteoVachePlugin::MeteoVachePlugin(void *pluginManager) :
		opencpn_plugin_18(pluginManager)
{
	toolBarIconId = 0;
	contextMenuId = 0;
	weatherWindow = nullptr;
	ocpnParentWindow = nullptr;
	contextMenu = nullptr;
	cursorLat = 0.0f;
	cursorLon = 0.0f;

	// Create bitmap of the plug-in icon
	initialize_images();
	pluginIcon = _img_meteovache_pi;
	toolbarIcon = _img_meteovache_tb;
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
	config.SetConfigObject(GetOCPNConfigObject());
	config.LoadConfig();

	// plug-in icon for the plug-in manager
	toolBarIconId = InsertPlugInTool(_("MeteoVache"), pluginIcon, pluginIcon, wxITEM_NORMAL, _("MeteoVache"), _("MeteoVache plug-in"),
	NULL, -1, 0, this);

	SetToolbarToolViz(toolBarIconId, !config.disableToolbarIcon);

	// Tool-bar icon
	SetToolbarToolBitmaps(toolBarIconId, toolbarIcon, toolbarIcon);
	plugInFlags |= INSTALLS_TOOLBAR_TOOL;

	// Create the context menu item
	contextMenu = new wxMenuItem(NULL, wxID_ANY, wxString(_("Weather forecast")),
			wxString(_("Request weather forecast from MeteoVache server at cursor location")), wxITEM_NORMAL,
			NULL);
	contextMenuId = AddCanvasContextMenuItem(contextMenu, this);
	plugInFlags |= INSTALLS_CONTEXTMENU_ITEMS;

	// Create the weather report window
	ocpnParentWindow = GetOCPNCanvasWindow();
	weatherWindow = new ReportWindow(ocpnParentWindow, &config, wxID_ANY, wxString(_("MeteoVache")), wxPoint(config.windowXPos, config.windowYPos),
			wxSize(config.windowWidth, config.windowHeight),
			wxDEFAULT_FRAME_STYLE);
	weatherWindow->SetPosition(wxPoint(config.windowXPos, config.windowYPos));
	weatherWindow->SetReportForecast(-1);

	return plugInFlags;
}

bool MeteoVachePlugin::DeInit(void)
{
	// Save configuration to OpenCPN configuration interface
	weatherWindow->UpdateConfig();
	config.SaveConfig();

	// Delete every allocated object
	delete weatherWindow;
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
	return pluginIcon;
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
	weatherWindow->Show(!weatherWindow->IsShown());
	weatherWindow->Layout();
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
		weatherWindow->Show(true);
		weatherWindow->RequestForecast(cursorLat, cursorLon);
	}
}

void MeteoVachePlugin::SetCursorLatLon(double lat, double lon)
{
	// Called each time mouse position changes. We store the latest value
	cursorLat = (float) lat;
	cursorLon = (float) lon;
}

void MeteoVachePlugin::ShowPreferencesDialog(wxWindow *parent)
{
	PreferenceDialog *prefDialog = new PreferenceDialog(parent, &config, wxID_ANY, _("MeteoVache preferences"), wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_DIALOG_STYLE);

	prefDialog->Fit();
	prefDialog->Layout();

	if (prefDialog->ShowModal() == wxID_OK)
	{
		prefDialog->UpdateConfig();
		SetToolbarToolViz(toolBarIconId, config.disableToolbarIcon == false);
	}

	delete prefDialog;
}

