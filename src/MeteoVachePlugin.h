#ifndef _METEOVACHEPLUGIN_H_
#define _METEOVACHEPLUGIN_H_

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/fileconf.h>

#include "version.h"
#include "wxWTranslateCatalog.h"
#include "ocpn_plugin.h"

#include <MVReportFrame.h>

#define PI_API_VERSION_MAJOR 1
#define PI_API_VERSION_MINOR 8

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------
class MeteoVachePlugin: public opencpn_plugin_18 {
public:
	MeteoVachePlugin(void *ppimgr);
	~MeteoVachePlugin();

	// The required PlugIn Methods
	int Init(void);
	bool DeInit(void);

	int GetAPIVersionMajor();
	int GetAPIVersionMinor();
	int GetPlugInVersionMajor();
	int GetPlugInVersionMinor();
	wxBitmap* GetPlugInBitmap();
	wxString GetCommonName();
	wxString GetShortDescription();
	wxString GetLongDescription();

	void OnToolbarToolCallback(int id);
	int GetToolbarToolCount(void);
	void OnContextMenuItemCallback(int id);
	void SetCursorLatLon(double lat, double lon);

	wxString m_shareLocn;
	MVReportFrame *weatherReportFrame;

private:
	wxBitmap *mvPluginIcon;
	wxBitmap *mvToolbarIcon;
	int toolBarIconId;
	int contextMenuId;
	wxWindow *ocpnParentWindow;
	wxMenuItem *contextMenu;
	float cursorLat, cursorLon;
    wxFileConfig *ocpnConfig;
    int windowWidth, windowHeight;
    int windowXPos, windowYPos;
    wxString selectedModelName;

    bool LoadConfig(void);
    bool SaveConfig(void);
};

#endif
