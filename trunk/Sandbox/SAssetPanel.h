#pragma once

#include "wx/wx.h"

class SAssetPanel: public wxPanel
{
public:
	SAssetPanel(wxWindow* parent);

private:
	void OnFilterChanged(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()
};
