/***************************************************************************
 * MeteoVachePlugin.cpp 1.0 2019/10/07
 *
 * Project:  OpenCPN
 * Purpose:  MeteoVache OpenCpn Plugin
 * Author:   Ronan Demoment
 *
 ***************************************************************************
 */

#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif
#ifndef __OCPN__ANDROID__
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include "qopengl.h"
#include "GL/gl_private.h"
#endif
#include <wx/mstream.h>
#include <wx/menuitem.h>

#include <MeteoVachePlugin.h>

#include <icons.h>

void MVLogMessage1(wxString s) {
	wxLogMessage("METEOVACHE: " + s);
}

extern "C" void MVLogMessage(const char *s) {
	MVLogMessage1(wxString::FromAscii(s));
}

extern "C" DECL_EXP opencpn_plugin* create_pi(void *pluginManager) {
	return new MeteoVachePlugin(pluginManager);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin *pluginInstance) {
	delete pluginInstance;
}

//---------------------------------------------------------------------------------------------------------
//
//    MeteoVache PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

MeteoVachePlugin::MeteoVachePlugin(void *pluginManager) :
		opencpn_plugin_18(pluginManager) {

	toolBarIconId = 0;
	contextMenuId = 0;
	weatherReportFrame = nullptr;
	ocpnParentWindow = nullptr;
	contextMenu = nullptr;
	cursorLat = 0.0f;
	cursorLon = 0.0f;
	ocpnConfig = nullptr;
	windowWidth = 500;
	windowHeight = 500;
	windowXPos = 500;
	windowYPos = 500;

	// Create bitmap of the plugin icon
	initialize_images();
	mvPluginIcon = _img_meteovache_pi;
	mvToolbarIcon = _img_meteovache_tb;
}

MeteoVachePlugin::~MeteoVachePlugin() {
	delete _img_meteovache_tb;
	delete _img_meteovache_pi;
}

int MeteoVachePlugin::Init(void) {
	AddLocaleCatalog( PLUGIN_CATALOG_NAME );

	// TODO : Limit to the minimum
	int plugInFlags = (WANTS_OVERLAY_CALLBACK |
	WANTS_CURSOR_LATLON |
	WANTS_TOOLBAR_CALLBACK |
	WANTS_CONFIG);

	// Load config from config file
	ocpnConfig = GetOCPNConfigObject();
	LoadConfig();

	// Plugin icon in plugin manager
	toolBarIconId = InsertPlugInTool(_("MeteoVache"), mvPluginIcon, mvPluginIcon, wxITEM_NORMAL, _("MeteoVache"), _("MeteoVache plugin"), NULL, -1, 0, this);

	// Toolbar icon
	SetToolbarToolBitmaps(toolBarIconId, mvToolbarIcon, mvToolbarIcon);
	plugInFlags |= INSTALLS_TOOLBAR_TOOL;

	contextMenu = new wxMenuItem(NULL, wxID_ANY, wxString(_("Weather forecast")),
			wxString(_("Request weather forecast from MeteoVache server at the location of the cursor")), wxITEM_NORMAL, NULL);
	contextMenuId = AddCanvasContextMenuItem(contextMenu, this);
	plugInFlags |= INSTALLS_CONTEXTMENU_ITEMS;

	ocpnParentWindow = GetOCPNCanvasWindow();
	weatherReportFrame = new MVReportFrame(ocpnParentWindow, wxID_ANY, wxString(_("MeteoVache")), wxPoint(windowXPos, windowYPos),
			wxSize(windowWidth, windowHeight),
			wxDEFAULT_FRAME_STYLE);
	weatherReportFrame->SetPosition(wxPoint(windowXPos, windowYPos));
	weatherReportFrame->SetReportText(wxString(""));
	weatherReportFrame->setSelectedModelName(selectedModelName);

	return plugInFlags;
}

bool MeteoVachePlugin::DeInit(void) {
	SaveConfig();

	delete weatherReportFrame;
	delete contextMenu;

	return true;
}

int MeteoVachePlugin::GetAPIVersionMajor() {
	return PI_API_VERSION_MAJOR;
}

int MeteoVachePlugin::GetAPIVersionMinor() {
	return PI_API_VERSION_MINOR;
}

int MeteoVachePlugin::GetPlugInVersionMajor() {
	return PLUGIN_VERSION_MAJOR;
}

int MeteoVachePlugin::GetPlugInVersionMinor() {
	return PLUGIN_VERSION_MINOR;
}

wxBitmap* MeteoVachePlugin::GetPlugInBitmap() {
	return mvPluginIcon;
}

wxString MeteoVachePlugin::GetCommonName() {
	return _("MeteoVache");
}

wxString MeteoVachePlugin::GetShortDescription() {
	return _("MeteoVache weather plugin for OpenCPN");
}

wxString MeteoVachePlugin::GetLongDescription() {
	return _(
			"MeteoVache is a weather plugin for OpenCPN.\n\
It provides weather forecasts everywhere in the world with only\n\
a low bandwidth internet connection.");
}

void MeteoVachePlugin::OnToolbarToolCallback(int id) {
	(void) id;

	weatherReportFrame->Show(!weatherReportFrame->IsShown());
	weatherReportFrame->Layout();
}

int MeteoVachePlugin::GetToolbarToolCount(void) {
	return 1;
}

void MeteoVachePlugin::OnContextMenuItemCallback(int id) {
	(void) id;
	wxString text;

	if (id == contextMenuId) {
		weatherReportFrame->RequestForecast(cursorLat, cursorLon);
	}
}

void MeteoVachePlugin::SetCursorLatLon(double lat, double lon) {
	cursorLat = (float) lat;
	cursorLon = (float) lon;
}

bool MeteoVachePlugin::LoadConfig(void) {
	wxFileConfig *pConf = (wxFileConfig*) ocpnConfig;

	if (pConf) {
		pConf->SetPath("/Settings/MeteoVache");
		pConf->Read("WindowWidth", &windowWidth, 500);
		pConf->Read("WindowHeight", &windowHeight, 500);
		pConf->Read("WindowXPos", &windowXPos, 250);
		pConf->Read("WindowYPos", &windowYPos, 250);
		pConf->Read("ModelName", &selectedModelName, "");

		return true;
	}
	else
		return false;
}

bool MeteoVachePlugin::SaveConfig(void) {
	wxFileConfig *pConf = (wxFileConfig*) ocpnConfig;

	if (pConf) {
		pConf->SetPath("/Settings/MeteoVache");
		pConf->Write("WindowWidth", weatherReportFrame->GetSize().x);
		pConf->Write("WindowHeight", weatherReportFrame->GetSize().y);
		pConf->Write("WindowXPos", weatherReportFrame->GetPosition().x);
		pConf->Write("WindowYPos", weatherReportFrame->GetPosition().y);
		pConf->Write("ModelName", weatherReportFrame->getSelectedModelName());
		return true;
	}
	else
		return false;
}
