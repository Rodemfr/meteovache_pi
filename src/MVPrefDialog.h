#ifndef _MVPREFDIALOG_H_
#define _MVPREFDIALOG_H_

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

class MVPrefDialog: public wxDialog
{
protected:
	wxStaticText *windUnitLabel;
	wxComboBox *windUnitSelection;
	wxStaticText *tempUnitLabel;
	wxComboBox *tempUnitSelection;
	wxStaticText *serverNameLabel;
	wxStaticText *serverPortLabel;
	wxTextCtrl *serverNameEdit;
	wxTextCtrl *serverPortEdit;
	wxStaticText *autosavePathLabel;
	wxTextCtrl *autosavePathEdit;
	wxButton *autosavePathEditButton;
	wxCheckBox *autosaveEnableCheckbox;
	wxCheckBox *autosaveColumnCheckbox;
	wxCheckBox *autosaveCompressCheckbox;

public:
	MVPrefDialog(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString, const wxPoint &pos = wxDefaultPosition, const wxSize &size =
			wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	~MVPrefDialog();
	void SetUnitPreferences(wxString windUnitString, wxString tempUnitString);
	void SetAutosavePreferences(wxString path, bool enable, bool column, bool compress);
	wxString GetWindUnitString();
	wxString GetTempUnitString();
	wxString GetAutosavePath();
	bool GetAutosaveEnable();
	bool GetAutosaveColumn();
	bool GetAutosaveCompress();
	void onAutosavePathBrowse(wxCommandEvent&);
};

#endif //_MVPREFDIALOG_H_