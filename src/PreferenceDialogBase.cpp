///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6-dirty)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "PreferenceDialogBase.h"

///////////////////////////////////////////////////////////////////////////

PreferenceDialogBase::PreferenceDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* globalSizer;
	globalSizer = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* unitSizer;
	unitSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Units") ), wxVERTICAL );

	wxBoxSizer* windUnitSizer;
	windUnitSizer = new wxBoxSizer( wxHORIZONTAL );

	windUnitLabel = new wxStaticText( unitSizer->GetStaticBox(), wxID_ANY, wxT("Wind unit"), wxDefaultPosition, wxDefaultSize, 0 );
	windUnitLabel->Wrap( -1 );
	windUnitSizer->Add( windUnitLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	windUnitSelection = new wxComboBox( unitSizer->GetStaticBox(), wxID_ANY, wxT("nd"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	windUnitSelection->Append( wxT("nd") );
	windUnitSelection->Append( wxT("bft") );
	windUnitSelection->Append( wxT("m/s") );
	windUnitSelection->Append( wxT("km/h") );
	windUnitSelection->Append( wxT("mi/h") );
	windUnitSizer->Add( windUnitSelection, 0, wxALL, 5 );


	unitSizer->Add( windUnitSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* tempUnitSizer;
	tempUnitSizer = new wxBoxSizer( wxHORIZONTAL );

	tempUnitLabel = new wxStaticText( unitSizer->GetStaticBox(), wxID_ANY, wxT("Temperature unit"), wxDefaultPosition, wxDefaultSize, 0 );
	tempUnitLabel->Wrap( -1 );
	tempUnitSizer->Add( tempUnitLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	tempUnitSelection = new wxComboBox( unitSizer->GetStaticBox(), wxID_ANY, wxT("Celsius"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	tempUnitSelection->Append( wxT("Celsius") );
	tempUnitSelection->Append( wxT("Farenheit") );
	tempUnitSizer->Add( tempUnitSelection, 0, wxALL, 5 );


	unitSizer->Add( tempUnitSizer, 1, wxEXPAND, 5 );


	globalSizer->Add( unitSizer, 0, wxALL|wxEXPAND, 5 );

	wxStaticBoxSizer* displaySizer;
	displaySizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Display") ), wxVERTICAL );

	wxBoxSizer* displaySubSizer;
	displaySubSizer = new wxBoxSizer( wxHORIZONTAL );

	timeZoneLabel = new wxStaticText( displaySizer->GetStaticBox(), wxID_ANY, wxT("Time zone"), wxDefaultPosition, wxDefaultSize, 0 );
	timeZoneLabel->Wrap( -1 );
	displaySubSizer->Add( timeZoneLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	timeZoneSelection = new wxComboBox( displaySizer->GetStaticBox(), wxID_ANY, wxT("Local / system"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	timeZoneSelection->Append( wxT("Local / system") );
	timeZoneSelection->Append( wxT("UTC") );
	displaySubSizer->Add( timeZoneSelection, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	displaySizer->Add( displaySubSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* disableToolbarIconSizer;
	disableToolbarIconSizer = new wxBoxSizer( wxHORIZONTAL );

	disableToolbarIconCheckBox = new wxCheckBox( displaySizer->GetStaticBox(), wxID_ANY, wxT("Disable toolbar icon"), wxDefaultPosition, wxDefaultSize, 0 );
	disableToolbarIconCheckBox->SetValue(true);
	disableToolbarIconSizer->Add( disableToolbarIconCheckBox, 1, 0, 5 );


	displaySizer->Add( disableToolbarIconSizer, 1, wxALIGN_RIGHT, 5 );


	globalSizer->Add( displaySizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* autosaveSizer;
	autosaveSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Auto save") ), wxVERTICAL );

	wxBoxSizer* autosavePathSizer;
	autosavePathSizer = new wxBoxSizer( wxHORIZONTAL );

	autoSavePathLabel = new wxStaticText( autosaveSizer->GetStaticBox(), wxID_ANY, wxT("Save directory"), wxDefaultPosition, wxDefaultSize, 0 );
	autoSavePathLabel->Wrap( -1 );
	autosavePathSizer->Add( autoSavePathLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	autoSavePathEdit = new wxTextCtrl( autosaveSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	autosavePathSizer->Add( autoSavePathEdit, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	autoSavePathEditButton = new wxButton( autosaveSizer->GetStaticBox(), wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	autosavePathSizer->Add( autoSavePathEditButton, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );


	autosaveSizer->Add( autosavePathSizer, 0, wxEXPAND, 5 );

	wxBoxSizer* autosaveCheckboxSizer;
	autosaveCheckboxSizer = new wxBoxSizer( wxHORIZONTAL );


	autosaveCheckboxSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	autoSaveEnableCheckbox = new wxCheckBox( autosaveSizer->GetStaticBox(), wxID_ANY, wxT("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	autosaveCheckboxSizer->Add( autoSaveEnableCheckbox, 0, wxALL, 5 );

	autoSaveColumnCheckbox = new wxCheckBox( autosaveSizer->GetStaticBox(), wxID_ANY, wxT("Column format"), wxDefaultPosition, wxDefaultSize, 0 );
	autoSaveColumnCheckbox->SetValue(true);
	autosaveCheckboxSizer->Add( autoSaveColumnCheckbox, 0, wxALL, 5 );

	autoSaveCompressCheckbox = new wxCheckBox( autosaveSizer->GetStaticBox(), wxID_ANY, wxT("Compress"), wxDefaultPosition, wxDefaultSize, 0 );
	autoSaveCompressCheckbox->SetValue(true);
	autosaveCheckboxSizer->Add( autoSaveCompressCheckbox, 0, wxALL, 5 );


	autosaveSizer->Add( autosaveCheckboxSizer, 1, wxEXPAND, 5 );


	globalSizer->Add( autosaveSizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* serverSizer;
	serverSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Server") ), wxHORIZONTAL );

	wxBoxSizer* serverLabelSizer;
	serverLabelSizer = new wxBoxSizer( wxVERTICAL );

	serverNameLabel = new wxStaticText( serverSizer->GetStaticBox(), wxID_ANY, wxT("Server name"), wxDefaultPosition, wxDefaultSize, 0 );
	serverNameLabel->Wrap( -1 );
	serverLabelSizer->Add( serverNameLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	serverPortLabel = new wxStaticText( serverSizer->GetStaticBox(), wxID_ANY, wxT("Server port"), wxDefaultPosition, wxDefaultSize, 0 );
	serverPortLabel->Wrap( -1 );
	serverLabelSizer->Add( serverPortLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	serverSizer->Add( serverLabelSizer, 1, wxEXPAND|wxTOP, 5 );

	wxBoxSizer* serverPrefSizer;
	serverPrefSizer = new wxBoxSizer( wxVERTICAL );

	serverNameEdit = new wxTextCtrl( serverSizer->GetStaticBox(), wxID_ANY, wxT("meteovache.dynv6.org"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	serverNameEdit->Enable( false );

	serverPrefSizer->Add( serverNameEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	serverPortEdit = new wxTextCtrl( serverSizer->GetStaticBox(), wxID_ANY, wxT("31837"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	serverPortEdit->Enable( false );

	serverPrefSizer->Add( serverPortEdit, 0, wxALL|wxEXPAND, 5 );


	serverSizer->Add( serverPrefSizer, 3, wxEXPAND, 5 );


	globalSizer->Add( serverSizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	globalSizer->Add( 0, 0, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	stdButtonsSizer = new wxStdDialogButtonSizer();
	stdButtonsSizerOK = new wxButton( this, wxID_OK );
	stdButtonsSizer->AddButton( stdButtonsSizerOK );
	stdButtonsSizerCancel = new wxButton( this, wxID_CANCEL );
	stdButtonsSizer->AddButton( stdButtonsSizerCancel );
	stdButtonsSizer->Realize();

	globalSizer->Add( stdButtonsSizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	this->SetSizer( globalSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

PreferenceDialogBase::~PreferenceDialogBase()
{
}
