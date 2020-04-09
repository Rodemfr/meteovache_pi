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
	wxStaticText *MVPrefWindUnitLabel;
	wxComboBox *WMPrefWindUnitBox;
	wxStaticText *MVPrefTempUnitLabel;
	wxComboBox *WMPrefTempUnitBox;
	wxStaticText *MVPrefServerNameLabel;
	wxStaticText *MVPrefServerPortLabel;
	wxTextCtrl *MVPrefServerNameEdit;
	wxTextCtrl *MVPrefServerPortEdit;
	wxStaticText* m_staticText5;
	wxTextCtrl* m_textCtrl3;
	wxButton* m_button1;
	wxCheckBox* m_checkBox1;
	wxCheckBox* m_checkBox3;

public:
	MVPrefDialog(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = wxEmptyString, const wxPoint &pos = wxDefaultPosition, const wxSize &size =
			wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
	~MVPrefDialog();
	void SetPreferences(wxString windUnitString, wxString tempUnitString);
	wxString GetWindUnitString();
	wxString GetTempUnitString();
};

#endif //_MVPREFDIALOG_H_
