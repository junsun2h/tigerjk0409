#pragma once

#include "wx/wx.h"
#include "SGlobal.h"
#include "wx/aui/aui.h"


enum MAIN_FRAME_WIDGET_ID
{
	ID_MAIN_TOOLBAR = wxID_HIGHEST+10000,
	ID_TOOLBAR_SELECT,
	ID_TOOLBAR_POSITION,
	ID_TOOLBAR_ROTATION,
	ID_TOOLBAR_SCALE,
	ID_TOOLBAR_CAMERA_SPEED,

	// menu
	ID_MENU_DEFAULT_LAYOUT,
	ID_MENU_LOAD_SAVED_LAYOUT,
	ID_MENU_SAVE_LAYOUT,

	ID_MENU_VIEW_PROPERTY,
	ID_MENU_VIEW_SCENE_HIERARCHY,
	ID_MENU_VIEW_ASSET,

	ID_MENU_ENTITY_NEW,
};


class SMainFrame: public wxFrame
{
public:
	SMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~SMainFrame();

	void	OnNew(wxCommandEvent& event);
	void	OnSave(wxCommandEvent& event);
	void	OnSaveAs(wxCommandEvent& event);
	void	OnQuit(wxCommandEvent& event);

	void	OnPanelClose(wxAuiManagerEvent& event);
	void	OnPanelCreate(wxCommandEvent& event);
	void	OnDefaultLayout(wxCommandEvent& event);
	void	OnLoadSavedLayout(wxCommandEvent& event);
	void	OnSaveLayout(wxCommandEvent& event);

	void	OnCreateEntity(wxCommandEvent& event);

private:
	void	InitLayout();
	void	InitToolBar();

	wxString	m_LayoutDefault;
	wxString	m_LayoutSaved;

	DECLARE_EVENT_TABLE()
};