#ifndef _MVPREFDIALOG_H_
#define _MVPREFDIALOG_H_

#include "ConfigContainer.h"
#include "PreferenceDialogBase.h"

#include <wx/artprov.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/colour.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/xrc/xmlres.h>

class PreferenceDialog : public PreferenceDialogBase
{
  protected:
    ConfigContainer *config;

  public:
    PreferenceDialog(wxWindow *parent, ConfigContainer *config, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString,
                     const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
    ~PreferenceDialog();
    void LoadConfig();
    void UpdateConfig();
    void OnAutoSavePathBrowse(wxCommandEvent &evt);
};

#endif //_MVPREFDIALOG_H_
