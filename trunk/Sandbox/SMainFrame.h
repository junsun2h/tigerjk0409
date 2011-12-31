#pragma once

#include "wx/wx.h"

enum
{
	ID_Quit = 1,
	ID_About,
};


class SMainFrame: public wxFrame
{
public:
	SMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~SMainFrame();

	void	OnQuit(wxCommandEvent& event);
	void	OnAbout(wxCommandEvent& event);

private:
	DECLARE_EVENT_TABLE()
};