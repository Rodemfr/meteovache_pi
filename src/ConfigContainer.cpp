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

#include <ConfigContainer.h>
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

ConfigContainer::ConfigContainer()
{
	windowWidth = 500;
	windowHeight = 500;
	windowXPos = 500;
	windowYPos = 500;

	windUnitString = "kt";
	tempUnitString = "C";

	selectedModelName = "Arpege";
	windUnitString = "nd";
	tempUnitString = "Celsius";
	timeZoneString = "Local / system";

	wxStandardPaths standardPaths = wxStandardPaths::Get();
	autosavePath = standardPaths.GetDocumentsDir();

	autoSaveEnable = false;
	autoSaveColumn = false;
	autoSaveCompress = false;
	manualSavePath = "";
	manualSaveFormat = 0;

	configObject = nullptr;
}

void ConfigContainer::setConfigObject(wxFileConfig *configObject)
{
	this->configObject = configObject;
}

ConfigContainer::~ConfigContainer()
{
	// TODO Auto-generated destructor stub
}

bool ConfigContainer::LoadConfig(void)
{
	wxFileConfig *pConf = (wxFileConfig*) configObject;

	if (pConf)
	{
		pConf->SetPath("/Settings/MeteoVache");
		pConf->Read("WindowWidth", &windowWidth, 500);
		pConf->Read("WindowHeight", &windowHeight, 500);
		pConf->Read("WindowXPos", &windowXPos, 250);
		pConf->Read("WindowYPos", &windowYPos, 250);
		pConf->Read("ModelName", &selectedModelName, "");
		pConf->Read("WindUnitString", &windUnitString, "kt");
		pConf->Read("TempUnitString", &tempUnitString, "C");
		pConf->Read("TimeZoneString", &timeZoneString, "Local / system");
		pConf->Read("AutoSavePath", &autosavePath, autosavePath);
		pConf->Read("AutoSaveEnable", &autoSaveEnable, false);
		pConf->Read("AutoSaveColumn", &autoSaveColumn, false);
		pConf->Read("AutoSaveCompress", &autoSaveCompress, true);
		pConf->Read("ManualSavePath", &manualSavePath, "");
		pConf->Read("ManualSaveFormat", &manualSaveFormat, 0);

		return true;;
	} else
		return false;
}

bool ConfigContainer::SaveConfig(void)
{
	wxFileConfig *pConf = (wxFileConfig*) configObject;

	if (pConf)
	{
		pConf->SetPath("/Settings/MeteoVache");
		pConf->Write("WindowWidth", windowWidth);
		pConf->Write("WindowHeight", windowHeight);
		pConf->Write("WindowXPos", windowXPos);
		pConf->Write("WindowYPos", windowYPos);
		pConf->Write("ModelName", selectedModelName);
		pConf->Write("WindUnitString", windUnitString);
		pConf->Write("TempUnitString", tempUnitString);
		pConf->Write("TimeZoneString", timeZoneString);
		pConf->Write("AutoSavePath", autosavePath);
		pConf->Write("AutoSaveEnable", autoSaveEnable);
		pConf->Write("AutoSaveColumn", autoSaveColumn);
		pConf->Write("AutoSaveCompress", autoSaveCompress);
		pConf->Write("ManualSavePath", manualSavePath);
		pConf->Write("ManualSaveFormat", manualSaveFormat);
		return true;
	} else
		return false;
}

