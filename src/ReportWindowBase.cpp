///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6-dirty)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ReportWindowBase.h"

///////////////////////////////////////////////////////////////////////////

ReportWindowBase::ReportWindowBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	reportGlobalSizer = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* reportTopSizer;
	reportTopSizer = new wxBoxSizer( wxHORIZONTAL );

	modelLabel = new wxStaticText( this, wxID_ANY, _("Weather model :"), wxDefaultPosition, wxDefaultSize, 0 );
	modelLabel->Wrap( -1 );
	reportTopSizer->Add( modelLabel, 0, wxALIGN_CENTER|wxALL, 5 );

	modelSelector = new wxComboBox( this, wxID_ANY, _("No Data"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	reportTopSizer->Add( modelSelector, 0, wxALL, 5 );


	reportGlobalSizer->Add( reportTopSizer, 0, wxEXPAND, 5 );

	statusLabel = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	statusLabel->Wrap( -1 );
	reportGlobalSizer->Add( statusLabel, 0, wxALL, 5 );

	reportTextArea    = new ForecastDisplay(this, wxID_ANY, _("Weather report"), wxDefaultPosition, wxDefaultSize, 0);
	reportGlobalSizer->Add( reportTextArea, 1, wxALL|wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT|wxTOP, 5 );

	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxVERTICAL );

	saveButton = new wxButton( this, wxID_ANY, _("Save As..."), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( saveButton, 0, wxALL, 5 );


	reportGlobalSizer->Add( buttonSizer, 0, wxEXPAND, 5 );


	this->SetSizer( reportGlobalSizer );
	this->Layout();

	this->Centre( wxBOTH );
}

ReportWindowBase::~ReportWindowBase()
{
}
