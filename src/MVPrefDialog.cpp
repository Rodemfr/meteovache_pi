#include <MVPrefDialog.h>

MVPrefDialog::MVPrefDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style) :
		wxDialog(parent, id, title, pos, size, style)
{
	wxString windUnits[] = { _("kt"), _("bft"), _("m/s"), _("kph"), _("mph") };
	wxString tempUnits[] = { _("Celsius"), _("Farenheit") };

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer *MVPrefGlobalSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer *MVPrefUnitSizer = new wxStaticBoxSizer(wxVERTICAL, this, _("Units"));

	wxBoxSizer *MVPrefWindUnitSizer = new wxBoxSizer(wxHORIZONTAL);
	MVPrefWindUnitLabel = new wxStaticText(MVPrefUnitSizer->GetStaticBox(), wxID_ANY, _("Wind unit"), wxDefaultPosition, wxDefaultSize, 0);
	MVPrefWindUnitLabel->Wrap(-1);
	MVPrefWindUnitSizer->Add(MVPrefWindUnitLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	WMPrefWindUnitBox = new wxComboBox(MVPrefUnitSizer->GetStaticBox(), wxID_ANY, _("kt"), wxDefaultPosition, wxDefaultSize, 5, windUnits, 0);
	MVPrefWindUnitSizer->Add(WMPrefWindUnitBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	MVPrefUnitSizer->Add(MVPrefWindUnitSizer, 0, wxEXPAND, 5);

	wxBoxSizer *MVPrefTempUnitSizer = new wxBoxSizer(wxHORIZONTAL);
	MVPrefTempUnitLabel = new wxStaticText(MVPrefUnitSizer->GetStaticBox(), wxID_ANY, _("Temperature unit"), wxDefaultPosition, wxDefaultSize, 0);
	MVPrefTempUnitLabel->Wrap(-1);
	MVPrefTempUnitSizer->Add(MVPrefTempUnitLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	WMPrefTempUnitBox = new wxComboBox(MVPrefUnitSizer->GetStaticBox(), wxID_ANY, _("Celsius"), wxDefaultPosition, wxDefaultSize, 2, tempUnits, 0);
	MVPrefTempUnitSizer->Add(WMPrefTempUnitBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	MVPrefUnitSizer->Add(MVPrefTempUnitSizer, 0, wxEXPAND, 5);

	wxBoxSizer *spacePadder1 = new wxBoxSizer(wxVERTICAL);
	MVPrefUnitSizer->Add(spacePadder1, 1, wxEXPAND, 5);

	MVPrefGlobalSizer->Add(MVPrefUnitSizer, 0, wxALL | wxEXPAND, 5);

	wxStaticBoxSizer *MVPrefServerSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Server")), wxVERTICAL);
	wxBoxSizer *MVPrefServerPrefSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer *MVPrefServerLabelSizer = new wxBoxSizer(wxVERTICAL);
	MVPrefServerNameLabel = new wxStaticText(MVPrefServerSizer->GetStaticBox(), wxID_ANY, wxT("Server name"), wxDefaultPosition, wxDefaultSize, 0);
	MVPrefServerNameLabel->Wrap(-1);
	MVPrefServerLabelSizer->Add(MVPrefServerNameLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	MVPrefServerPortLabel = new wxStaticText(MVPrefServerSizer->GetStaticBox(), wxID_ANY, wxT("Server port"), wxDefaultPosition, wxDefaultSize, 0);
	MVPrefServerPortLabel->Wrap(-1);
	MVPrefServerLabelSizer->Add(MVPrefServerPortLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	MVPrefServerPrefSizer->Add(MVPrefServerLabelSizer, 1, wxEXPAND, 5);

	wxBoxSizer *MVPrefServerPortSizer;
	MVPrefServerPortSizer = new wxBoxSizer(wxVERTICAL);

	MVPrefServerNameEdit = new wxTextCtrl(MVPrefServerSizer->GetStaticBox(), wxID_ANY, "meteovache.dyndns.org", wxDefaultPosition, wxDefaultSize,
	wxTE_RIGHT);
	MVPrefServerPortSizer->Add(MVPrefServerNameEdit, 1, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5);

	MVPrefServerPortEdit = new wxTextCtrl(MVPrefServerSizer->GetStaticBox(), wxID_ANY, "31837", wxDefaultPosition, wxDefaultSize,
	wxTE_RIGHT);
	MVPrefServerPortSizer->Add(MVPrefServerPortEdit, 1, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 5);

	MVPrefServerPrefSizer->Add(MVPrefServerPortSizer, 3, wxEXPAND, 5);

	MVPrefServerSizer->Add(MVPrefServerPrefSizer, 0, wxEXPAND, 5);

	wxBoxSizer *spacePadder2 = new wxBoxSizer(wxVERTICAL);

	MVPrefServerSizer->Add(spacePadder2, 1, wxEXPAND, 5);

	MVPrefGlobalSizer->Add(MVPrefServerSizer, 0, wxALL | wxEXPAND, 5);

	wxBoxSizer *spacePadder3 = new wxBoxSizer(wxVERTICAL);
	MVPrefGlobalSizer->Add(spacePadder3, 1, wxEXPAND, 5);

	wxStdDialogButtonSizer *MVPrefStdButtonsSizer = new wxStdDialogButtonSizer();
	wxButton *MVPrefOkButton = new wxButton(this, wxID_OK);
	MVPrefStdButtonsSizer->AddButton(MVPrefOkButton);
	wxButton *MVPrefCancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"));
	MVPrefStdButtonsSizer->AddButton(MVPrefCancelButton);
	MVPrefStdButtonsSizer->Realize();
	MVPrefGlobalSizer->Add(MVPrefStdButtonsSizer, 0, wxBOTTOM, 5);

	this->SetSizer(MVPrefGlobalSizer);
	this->Layout();
	MVPrefGlobalSizer->Fit(this);

	MVPrefServerNameEdit->Disable();
	MVPrefServerPortEdit->Disable();

	this->Centre(wxBOTH);
}

MVPrefDialog::~MVPrefDialog()
{
}

void MVPrefDialog::SetPreferences(wxString windUnitString, wxString tempUnitString)
{
	WMPrefWindUnitBox->SetStringSelection(_(windUnitString));
	WMPrefTempUnitBox->SetStringSelection(_(tempUnitString));
}

wxString MVPrefDialog::GetWindUnitString()
{
	wxString translatedUnit = WMPrefWindUnitBox->GetStringSelection();

	if (translatedUnit.IsSameAs(_("kt")))
	{
		return("kt");
	} else if (translatedUnit.IsSameAs(_("bft")))
	{
		return("bft");
	} else if (translatedUnit.IsSameAs(_("m/s")))
	{
		return("m/s");
	} else if (translatedUnit.IsSameAs(_("kph")))
	{
		return("kph");
	} else if (translatedUnit.IsSameAs(_("mph")))
	{
		return("mph");
	}

	return ("kt");
}

wxString MVPrefDialog::GetTempUnitString()
{
	wxString translatedUnit = WMPrefTempUnitBox->GetStringSelection();

	if (translatedUnit.IsSameAs(_("Celsius")))
	{
		return("Celsius");
	} else if (translatedUnit.IsSameAs(_("Farenheit")))
	{
		return("Farenheit");
	}

	return ("Celsius");
}
