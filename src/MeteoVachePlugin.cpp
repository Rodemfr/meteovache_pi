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

#include "Icons.h"
#include "MeteoVachePlugin.h"
#include "PreferenceDialog.h"

#include <wx/display.h>
#include <wx/menuitem.h>
#include <wx/mstream.h>
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

extern "C" DECL_EXP opencpn_plugin *create_pi(void *pluginManager)
{
    return new MeteoVachePlugin(pluginManager);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *pluginInstance)
{
    delete pluginInstance;
}

MeteoVachePlugin::MeteoVachePlugin(void *pluginManager) : opencpn_plugin_118(pluginManager)
{
    toolBarIconId    = 0;
    contextMenuId    = 0;
    weatherWindow    = nullptr;
    ocpnParentWindow = nullptr;
    contextMenu      = nullptr;
    cursorLat        = 0.0f;
    cursorLon        = 0.0f;

    // Create bitmap of the plug-in icon
    initialize_images();
    pluginIcon = _img_meteovache_pi;
}

MeteoVachePlugin::~MeteoVachePlugin()
{
    delete_images();
}

int MeteoVachePlugin::Init(void)
{
    AddLocaleCatalog(_T("opencpn-meteovache_pi"));

    wxInitAllImageHandlers();

    int plugInFlags = (WANTS_OVERLAY_CALLBACK | WANTS_CURSOR_LATLON | WANTS_TOOLBAR_CALLBACK | WANTS_CONFIG | WANTS_PREFERENCES);

    // Load configuration from OpenCPN configuration interface
    config.SetConfigObject(GetOCPNConfigObject());
    config.LoadConfig();

    toolBarIconId = InsertPlugInToolSVG(_("MeteoVache"), gMeteoVacheSvgToolbarIcon, gMeteoVacheSvgToolbarIcon, gMeteoVacheSvgToolbarIcon,
                                        wxITEM_CHECK, _("MeteoVache"), _T( "MeteoVache plug-in" ), NULL, -1, 0, this);

    SetToolbarToolViz(toolBarIconId, !config.disableToolbarIcon);

    // Create the context menu item
    /* TRANSLATORS: Right click menu entry */
    contextMenu   = new wxMenuItem(NULL, wxID_ANY, wxString(_("Weather forecast")),
                                   wxString(_("Request weather forecast from MeteoVache server at "
                                                "cursor location")),
                                   wxITEM_NORMAL, NULL);
    contextMenuId = AddCanvasContextMenuItem(contextMenu, this);
    plugInFlags |= INSTALLS_CONTEXTMENU_ITEMS;

    // Ensure that window dimensions are not out of the screen
    ocpnParentWindow = GetOCPNCanvasWindow();
    wxDisplay display(wxDisplay::GetFromWindow(ocpnParentWindow));
    wxRect    clientArea = display.GetGeometry();

    if ((config.windowWidth > clientArea.GetWidth()) || (config.windowWidth < 0))
    {
        config.windowWidth = clientArea.GetWidth() / 2;
    }
    if ((config.windowHeight > clientArea.GetHeight()) || (config.windowHeight < 0))
    {
        config.windowHeight = clientArea.GetHeight() / 2;
    }
    if ((config.windowXPos > clientArea.GetWidth()) || (config.windowXPos + config.windowWidth < 0))
    {
        config.windowXPos = clientArea.GetWidth() / 4;
    }
    if ((config.windowYPos > clientArea.GetHeight()) || (config.windowYPos + config.windowHeight < 0))
    {
        config.windowYPos = clientArea.GetHeight() / 4;
    }

// Create the weather report window
#ifdef __MACOS__
    // On MacOS the window needs wxSTAY_ON_TOP flag no to pass behinf OCPN's
    // window
    weatherWindow = new ReportWindow(ocpnParentWindow, &config, wxID_ANY, wxString(_("MeteoVache")), wxPoint(config.windowXPos, config.windowYPos),
                                     wxSize(config.windowWidth, config.windowHeight), wxCLOSE_BOX | wxCAPTION | wxRESIZE_BORDER | wxSTAY_ON_TOP);
#else
    weatherWindow = new ReportWindow(ocpnParentWindow, &config, wxID_ANY, wxString(_("MeteoVache")), wxPoint(config.windowXPos, config.windowYPos),
                                     wxSize(config.windowWidth, config.windowHeight), wxCLOSE_BOX | wxCAPTION | wxRESIZE_BORDER);
#endif

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

int MeteoVachePlugin::GetPlugInVersionPatch()
{
    return PLUGIN_VERSION_PATCH;
}

int MeteoVachePlugin::GetPlugInVersionPost()
{
    return PLUGIN_VERSION_TWEAK;
}

const char *MeteoVachePlugin::GetPlugInVersionPre()
{
    return PKG_PRERELEASE;
}

const char *MeteoVachePlugin::GetPlugInVersionBuild()
{
    return PKG_BUILD_INFO;
}

wxBitmap *MeteoVachePlugin::GetPlugInBitmap()
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
    return _("MeteoVache is a weather plug-in for OpenCPN. It provides weather "
             "forecasts everywhere in the world even with a very low bandwidth "
             "Internet connection.");
}

void MeteoVachePlugin::OnToolbarToolCallback(int id)
{
    (void)id;
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
    (void)id;
    wxString text;

    // If MeteoVache context menu item has been clicked, we request a new
    // forecast for the last recorded cursor position
    if (id == contextMenuId)
    {
        weatherWindow->Show(true);
        weatherWindow->RequestForecast(cursorLat, cursorLon);
    }
}

void MeteoVachePlugin::SetCursorLatLon(double lat, double lon)
{
    // Called each time mouse position changes. We store the latest value
    cursorLat = (float)lat;
    cursorLon = (float)lon;
}

void MeteoVachePlugin::ShowPreferencesDialog(wxWindow *parent)
{
    PreferenceDialog *prefDialog =
        new PreferenceDialog(parent, &config, wxID_ANY, _("MeteoVache preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);

    prefDialog->Fit();
    prefDialog->Layout();
    prefDialog->LoadConfig();

    if (prefDialog->ShowModal() == wxID_OK)
    {
        prefDialog->UpdateConfig();
        SetToolbarToolViz(toolBarIconId, config.disableToolbarIcon == false);
    }

    delete prefDialog;
}
