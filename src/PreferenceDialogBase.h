///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6-dirty)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

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
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class PreferenceDialogBase
///////////////////////////////////////////////////////////////////////////////
class PreferenceDialogBase : public wxDialog
{
	private:

	protected:
		wxStaticText* windUnitLabel;
		wxComboBox* windUnitSelection;
		wxStaticText* tempUnitLabel;
		wxComboBox* tempUnitSelection;
		wxStaticText* timeZoneLabel;
		wxComboBox* timeZoneSelection;
		wxCheckBox* disableToolbarIconCheckBox;
		wxStaticText* autoSavePathLabel;
		wxTextCtrl* autoSavePathEdit;
		wxButton* autoSavePathEditButton;
		wxCheckBox* autoSaveEnableCheckbox;
		wxCheckBox* autoSaveColumnCheckbox;
		wxCheckBox* autoSaveCompressCheckbox;
		wxStaticText* serverNameLabel;
		wxStaticText* serverPortLabel;
		wxTextCtrl* serverNameEdit;
		wxTextCtrl* serverPortEdit;
		wxStdDialogButtonSizer* stdButtonsSizer;
		wxButton* stdButtonsSizerOK;
		wxButton* stdButtonsSizerCancel;

	public:

		PreferenceDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("MeteoVache preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 657,530 ), long style = wxDEFAULT_DIALOG_STYLE );

		~PreferenceDialogBase();

};

