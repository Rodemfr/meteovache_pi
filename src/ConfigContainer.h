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

#ifndef _CONFIGCONTAINER_H_
#define _CONFIGCONTAINER_H_

/***************************************************************************/
/*                              Includes                                   */
/***************************************************************************/

#include <wx/wx.h>
#include <wx/fileconf.h>

/***************************************************************************/
/*                              Constants                                  */
/***************************************************************************/

/***************************************************************************/
/*                                Types                                    */
/***************************************************************************/

/***************************************************************************/
/*                               Classes                                   */
/***************************************************************************/

class ConfigContainer
{
private:
	wxFileConfig *configObject;    // Pointer to the OpenCPN configuration object

public:
	int windowWidth, windowHeight; // Current report window size
	int windowXPos, windowYPos;    // Current report window position
	wxString selectedModelName;    // Currently selected weather model
	wxString windUnitString;       // String of the wind unit to use
	wxString tempUnitString;       // String of the temperature unit to use
	wxString timeZoneString;       // Selected time zone for display
	wxString autoSavePath;         // Path where to automatically save weather reports
	bool autoSaveEnable;           // Enable autosave
	bool autoSaveColumn;           // Enable column format for autosaved reports
	bool autoSaveCompress;         // Enable compression for autosaved reports
	wxString manualSavePath;       // Last used path for manual saving
	int manualSaveFormat;          // Last used format for manual saving

	ConfigContainer();
	virtual ~ConfigContainer();

	void SetConfigObject(wxFileConfig *configObject);
	bool LoadConfig(void);
	bool SaveConfig(void);
};

#endif /* _CONFIGCONTAINER_H_ */
