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
	unitSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Units") ), wxVERTICAL );

	wxBoxSizer* windUnitSizer;
	windUnitSizer = new wxBoxSizer( wxHORIZONTAL );

	windUnitLabel = new wxStaticText( unitSizer->GetStaticBox(), wxID_ANY, _("Wind unit"), wxDefaultPosition, wxDefaultSize, 0 );
	windUnitLabel->Wrap( -1 );
	windUnitSizer->Add( windUnitLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	windUnitSelection = new wxComboBox( unitSizer->GetStaticBox(), wxID_ANY, _("kt"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	windUnitSelection->Append( _("kt") );
	windUnitSelection->Append( _("bft") );
	windUnitSelection->Append( _("m/s") );
	windUnitSelection->Append( _("kph") );
	windUnitSelection->Append( _("mph") );
	windUnitSizer->Add( windUnitSelection, 0, wxALL, 5 );


	unitSizer->Add( windUnitSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* tempUnitSizer;
	tempUnitSizer = new wxBoxSizer( wxHORIZONTAL );

	tempUnitLabel = new wxStaticText( unitSizer->GetStaticBox(), wxID_ANY, _("Temperature unit"), wxDefaultPosition, wxDefaultSize, 0 );
	tempUnitLabel->Wrap( -1 );
	tempUnitSizer->Add( tempUnitLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	tempUnitSelection = new wxComboBox( unitSizer->GetStaticBox(), wxID_ANY, _("Celsius"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	tempUnitSelection->Append( _("Celsius") );
	tempUnitSelection->Append( _("Farenheit") );
	tempUnitSizer->Add( tempUnitSelection, 0, wxALL, 5 );


	unitSizer->Add( tempUnitSizer, 1, wxEXPAND, 5 );


	globalSizer->Add( unitSizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* displaySizer;
	displaySizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Display") ), wxVERTICAL );

	wxBoxSizer* displaySubSizer;
	displaySubSizer = new wxBoxSizer( wxHORIZONTAL );

	timeZoneLabel = new wxStaticText( displaySizer->GetStaticBox(), wxID_ANY, _("Time zone"), wxDefaultPosition, wxDefaultSize, 0 );
	timeZoneLabel->Wrap( -1 );
	displaySubSizer->Add( timeZoneLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	timeZoneSelection = new wxComboBox( displaySizer->GetStaticBox(), wxID_ANY, _("Local / system"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	timeZoneSelection->Append( _("Local / system") );
	timeZoneSelection->Append( _("UTC") );
	displaySubSizer->Add( timeZoneSelection, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	displaySizer->Add( displaySubSizer, 1, wxEXPAND, 5 );

	wxBoxSizer* disableToolbarIconSizer;
	disableToolbarIconSizer = new wxBoxSizer( wxHORIZONTAL );


	disableToolbarIconSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	disableToolbarIconCheckBox = new wxCheckBox( displaySizer->GetStaticBox(), wxID_ANY, _("Disable toolbar icon"), wxDefaultPosition, wxDefaultSize, 0 );
	disableToolbarIconSizer->Add( disableToolbarIconCheckBox, 0, wxALL, 5 );

	graphicalWindArrows = new wxCheckBox( displaySizer->GetStaticBox(), wxID_ANY, _("Graphical wind arrows"), wxDefaultPosition, wxDefaultSize, 0 );
	graphicalWindArrows->SetValue(true);
	disableToolbarIconSizer->Add( graphicalWindArrows, 0, wxALL, 5 );


	displaySizer->Add( disableToolbarIconSizer, 1, wxEXPAND, 5 );


	globalSizer->Add( displaySizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* autosaveSizer;
	autosaveSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Auto save") ), wxVERTICAL );

	wxBoxSizer* autosavePathSizer;
	autosavePathSizer = new wxBoxSizer( wxHORIZONTAL );

	autoSavePathLabel = new wxStaticText( autosaveSizer->GetStaticBox(), wxID_ANY, _("Save directory"), wxDefaultPosition, wxDefaultSize, 0 );
	autoSavePathLabel->Wrap( -1 );
	autosavePathSizer->Add( autoSavePathLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	autoSavePathEdit = new wxTextCtrl( autosaveSizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	autosavePathSizer->Add( autoSavePathEdit, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	autoSavePathEditButton = new wxButton( autosaveSizer->GetStaticBox(), wxID_ANY, _("..."), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT );
	autosavePathSizer->Add( autoSavePathEditButton, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );


	autosaveSizer->Add( autosavePathSizer, 0, wxEXPAND, 5 );

	wxBoxSizer* autosaveCheckboxSizer;
	autosaveCheckboxSizer = new wxBoxSizer( wxHORIZONTAL );


	autosaveCheckboxSizer->Add( 0, 0, 1, wxEXPAND, 5 );

	autoSaveEnableCheckbox = new wxCheckBox( autosaveSizer->GetStaticBox(), wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	autosaveCheckboxSizer->Add( autoSaveEnableCheckbox, 0, wxALL, 5 );

	autoSaveColumnCheckbox = new wxCheckBox( autosaveSizer->GetStaticBox(), wxID_ANY, _("Column format"), wxDefaultPosition, wxDefaultSize, 0 );
	autoSaveColumnCheckbox->SetValue(true);
	autosaveCheckboxSizer->Add( autoSaveColumnCheckbox, 0, wxALL, 5 );

	autoSaveCompressCheckbox = new wxCheckBox( autosaveSizer->GetStaticBox(), wxID_ANY, _("Compress"), wxDefaultPosition, wxDefaultSize, 0 );
	autoSaveCompressCheckbox->SetValue(true);
	autosaveCheckboxSizer->Add( autoSaveCompressCheckbox, 0, wxALL, 5 );


	autosaveSizer->Add( autosaveCheckboxSizer, 1, wxEXPAND, 5 );


	globalSizer->Add( autosaveSizer, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxStaticBoxSizer* serverSizer;
	serverSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Server") ), wxHORIZONTAL );

	wxBoxSizer* serverLabelSizer;
	serverLabelSizer = new wxBoxSizer( wxVERTICAL );

	serverNameLabel = new wxStaticText( serverSizer->GetStaticBox(), wxID_ANY, _("Server name"), wxDefaultPosition, wxDefaultSize, 0 );
	serverNameLabel->Wrap( -1 );
	serverLabelSizer->Add( serverNameLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

	serverPortLabel = new wxStaticText( serverSizer->GetStaticBox(), wxID_ANY, _("Server port"), wxDefaultPosition, wxDefaultSize, 0 );
	serverPortLabel->Wrap( -1 );
	serverLabelSizer->Add( serverPortLabel, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );


	serverSizer->Add( serverLabelSizer, 1, wxEXPAND|wxTOP, 5 );

	wxBoxSizer* serverPrefSizer;
	serverPrefSizer = new wxBoxSizer( wxVERTICAL );

	serverNameEdit = new wxTextCtrl( serverSizer->GetStaticBox(), wxID_ANY, _("meteovache.dynv6.org"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	serverNameEdit->Enable( false );

	serverPrefSizer->Add( serverNameEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );

	serverPortEdit = new wxTextCtrl( serverSizer->GetStaticBox(), wxID_ANY, _("31837"), wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
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
