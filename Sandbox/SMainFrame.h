#pragma once

#include "wx/wx.h"
#include "wx/aui/aui.h"

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

private:
	void	InitLayout();

	wxString	m_LayoutDefault;
	wxString	m_LayoutSaved;

	DECLARE_EVENT_TABLE()
};