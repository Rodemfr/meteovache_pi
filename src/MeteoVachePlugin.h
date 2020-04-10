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

#ifndef _METEOVACHEPLUGIN_H_
#define _METEOVACHEPLUGIN_H_

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <wx/wxprec.h>
#ifndef  WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/fileconf.h>

#include <version.h>
#include <wxWTranslateCatalog.h>
#include <ocpn_plugin.h>

#include <MVReportFrame.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

// This plug-in uses OpenCPN Plug-in API v1.8
#define PI_API_VERSION_MAJOR 1
#define PI_API_VERSION_MINOR 8

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

typedef struct {
	int windowWidth, windowHeight; // Current report window size
	int windowXPos, windowYPos;    // Current report window position
	wxString selectedModelName;    // Currently selected weather model
	wxString windUnitString;       // String of the wind unit to use
	wxString tempUnitString;       // String of the temperature unit to use
	wxString autosavePath;         // Path where to automatically save weather reports
	bool autosaveEnable;           // Enable autosave
	bool autosaveColumn;           // Enable column format for autosaved reports
	bool autoSaveCompress;         // Enable compression for autosaved reports
	wxString manualSavePath;       // Last used path for manual saving
	int manualSaveFormat;          // Last used format for manual saving
} configData_t;

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class MeteoVachePlugin: public opencpn_plugin_18
{
public:
	MeteoVachePlugin(void *ppimgr);
	~MeteoVachePlugin();

	// The required PlugIn Methods
	int Init(void);
	bool DeInit(void);
	void ShowPreferencesDialog(wxWindow *parent);

	int GetAPIVersionMajor();
	int GetAPIVersionMinor();
	int GetPlugInVersionMajor();
	int GetPlugInVersionMinor();
	wxBitmap* GetPlugInBitmap();
	wxString GetCommonName();
	wxString GetShortDescription();
	wxString GetLongDescription();

	void OnToolbarToolCallback(int id);
	int GetToolbarToolCount(void);
	void OnContextMenuItemCallback(int id);
	void SetCursorLatLon(double lat, double lon);

	wxString m_shareLocn;
	MVReportFrame *weatherReportFrame;

private:
	wxBitmap *mvPluginIcon;        // Plug-in icon for the plug-in manager (bright background)
	wxBitmap *mvToolbarIcon;       // plug-in icon for the tool-bar (dark background)
	int toolBarIconId;             // Id of the plug-in icon in the tool-bar
	int contextMenuId;             // Id of the plug-in item in context menu
	wxWindow *ocpnParentWindow;    // Pointer to the OpenCPN parent window
	wxMenuItem *contextMenu;       // Pointer to plug-in context menu item object
	float cursorLat, cursorLon;    // Latest known position of the mouse cursor
	wxFileConfig *ocpnConfig;      // Pointer to the OpenCPN configuration object
	configData_t pluginConfig;     // Configuration of the plug-in

	bool LoadConfig(void);
	bool SaveConfig(void);
};

#endif
