/***************************************************************************
 #include <PreferenceDialog.h>
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

#include "PreferenceDialog.h"

#include <stdint.h>
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

PreferenceDialog::PreferenceDialog(wxWindow *parent, ConfigContainer *config, wxWindowID id, const wxString &title, const wxPoint &pos,
                                   const wxSize &size, long style)
    : PreferenceDialogBase(parent, id, title, pos, size, style)
{
    wxString windUnits[] = {_("kt"), _("bft"), _("m/s"), _("kph"), _("mph")};
    wxString tempUnits[] = {_("Celsius"), _("Farenheit")};

    this->config = config;

    autoSavePathEditButton->Connect(wxEVT_BUTTON, wxCommandEventHandler(PreferenceDialog::OnAutoSavePathBrowse), NULL, this);
}

PreferenceDialog::~PreferenceDialog()
{
}

void PreferenceDialog::UpdateConfig()
{
    // wind unit
    wxString translatedString = windUnitSelection->GetStringSelection();
    /* TRANSLATORS: knots wind speed unit */
    if (translatedString.IsSameAs(_("kt")))
    {
        config->windUnitString = "kt";
    }
    /* TRANSLATORS: beaufort wind speed unit */
    else if (translatedString.IsSameAs(_("bft")))
    {
        config->windUnitString = "bft";
    }
    /* TRANSLATORS: m/s wind speed unit */
    else if (translatedString.IsSameAs(_("m/s")))
    {
        config->windUnitString = "m/s";
    }
    /* TRANSLATORS: kilometers per hour wind speed unit */
    else if (translatedString.IsSameAs(_("kph")))
    {
        config->windUnitString = "kph";
    }
    /* TRANSLATORS: miles per hour wind speed unit */
    else if (translatedString.IsSameAs(_("mph")))
    {
        config->windUnitString = "mph";
    }
    else
    {
        config->windUnitString = "kt";
    }
    // Temperature unit
    translatedString = tempUnitSelection->GetStringSelection();
    /* TRANSLATORS: Temperature unit */
    if (translatedString.IsSameAs(_("Celsius")))
    {
        config->tempUnitString = "Celsius";
    }
    /* TRANSLATORS: Temperature unit */
    else if (translatedString.IsSameAs(_("Farenheit")))
    {
        config->tempUnitString = "Farenheit";
    }
    else
    {
        config->tempUnitString = "Celsius";
    }
    // Time zone selection
    translatedString = timeZoneSelection->GetStringSelection();
    /* TRANSLATORS: Time zone reference */
    if (translatedString.IsSameAs(_("Local / system")))
    {
        config->timeZoneString = "Local / system";
    }
    /* TRANSLATORS: Time zone reference */
    else if (translatedString.IsSameAs(_("UTC")))
    {
        config->timeZoneString = "UTC";
    }
    else
    {
        config->timeZoneString = "Local / system";
    }
    // Toolbar icon display
    config->disableToolbarIcon = disableToolbarIconCheckBox->GetValue();
    // Autosave options
    config->autoSavePath     = autoSavePathEdit->GetValue();
    config->autoSaveEnable   = autoSaveEnableCheckbox->GetValue();
    config->autoSaveColumn   = autoSaveColumnCheckbox->GetValue();
    config->autoSaveCompress = autoSaveCompressCheckbox->GetValue();
}

void PreferenceDialog::OnAutoSavePathBrowse(wxCommandEvent &)
{
    wxDirDialog selectDirDialog(NULL, _("Select automatic save directory"), autoSavePathEdit->GetValue(), wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

    if (selectDirDialog.ShowModal() == wxID_OK)
    {
        autoSavePathEdit->SetValue(selectDirDialog.GetPath());
    }
}
