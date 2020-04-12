#ifndef _MVPREFDIALOG_H_
#define _MVPREFDIALOG_H_

#include <ConfigContainer.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/button.h>

class PreferenceDialog: public wxDialog
{
protected:
	wxStaticText *windUnitLabel;
	wxComboBox *windUnitSelection;
	wxStaticText *tempUnitLabel;
	wxComboBox *tempUnitSelection;
	wxStaticText* timeZoneLabel;
	wxComboBox* timeZoneSelection;
	wxStaticText *serverNameLabel;
	wxStaticText *serverPortLabel;
	wxTextCtrl *serverNameEdit;
	wxTextCtrl *serverPortEdit;
	wxStaticText *autoSavePathLabel;
	wxTextCtrl *autoSavePathEdit;
	wxButton *autoSavePathEditButton;
	wxCheckBox *autoSaveEnableCheckbox;
	wxCheckBox *autoSaveColumnCheckbox;
	wxCheckBox *autoSaveCompressCheckbox;

	ConfigContainer *config;

public:
	PreferenceDialog(wxWindow *parent, ConfigContainer *config, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString, const wxPoint &pos = wxDefaultPosition, const wxSize &size =
			wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	~PreferenceDialog();
	void UpdateConfig();
	void OnAutoSavePathBrowse(wxCommandEvent &evt);
};

#endif //_MVPREFDIALOG_H_
