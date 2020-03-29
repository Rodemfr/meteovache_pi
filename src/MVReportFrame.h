#ifndef __MVREPORTFRAME_H__
#define __MVREPORTFRAME_H__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/event.h>

#include "MeteoVacheThread.h"
#include "JobQueue.h"
#include "SpotForecasts.h"

class MVReportFrame: public wxDialog {
private:
	wxStaticText *MVReportModelLabel;
	wxComboBox *MVReportModelSelector;
	wxTextCtrl *MVReportTextArea;
	MeteoVacheThread *workerThread;
	SpotForecasts spotForecast;
	JobQueue *jobQueue;
	wxString selectedString;

	virtual void MVReportFrameOnClose(wxCloseEvent &event);
	virtual void MVModelOnSelect(wxCommandEvent &event);
    wxString getLatitudeString(float latitude);
    wxString getLongitudeString(float longitude);
    wxString getTextDirection(float windDirectionDeg);

    void startThread();
	void stopThread();

public:
	friend class MeteoVacheThread;

	MVReportFrame(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString &title = _("MeteoVache"), const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxSize(497, 445), long style = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxSTAY_ON_TOP | wxTAB_TRAVERSAL);
	~MVReportFrame();

	void SetReportText(const wxString &text);
	void OnThreadEvent(wxCommandEvent&);
	void PublishWeatherReport(int model);
	void RequestForecast(float latitude, float longitude);
	const wxString &getSelectedModelName();
	void setSelectedModelName(wxString modelName);

	DECLARE_EVENT_TABLE()
};

#endif //__MVREPORTFRAME_H__
