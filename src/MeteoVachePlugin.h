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

#include <ReportWindow.h>
#include <ConfigContainer.h>
#include <wx/wx.h>
#include <config.h>
#include <ocpn_plugin.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

// This plug-in uses OpenCPN Plug-in API v1.8
#define PI_API_VERSION_MAJOR 1
#define PI_API_VERSION_MINOR 16

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class MeteoVachePlugin: public opencpn_plugin_116
{
public:
	MeteoVachePlugin(void *ppimgr);
	~MeteoVachePlugin();

	// Plug-in methods for OpenCPN main program
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

private:
	ReportWindow *weatherWindow; // Dialog display weather report
	ConfigContainer config;      // Configuration of the plug-in
	wxBitmap *pluginIcon;        // Plug-in icon for the plug-in manager (bright background)
	wxBitmap *toolbarIcon;       // plug-in icon for the tool-bar (dark background)
	int toolBarIconId;           // Id of the plug-in icon in the tool-bar
	int contextMenuId;           // Id of the plug-in item in context menu
	wxWindow *ocpnParentWindow;  // Pointer to the OpenCPN parent window
	wxMenuItem *contextMenu;     // Pointer to plug-in context menu item object
	float cursorLat, cursorLon;  // Latest known position of the mouse cursor
};

#endif
