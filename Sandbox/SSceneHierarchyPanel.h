#pragma once

#include "wx/wx.h"

class SSceneHierarchyPanel: public wxPanel
{
public:
	SSceneHierarchyPanel(wxWindow* parent);

private:
	void OnFilterChanged(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};
