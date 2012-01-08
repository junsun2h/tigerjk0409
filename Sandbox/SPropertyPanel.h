#pragma once

#include "wx/wx.h"
#include "wx/propgrid/manager.h"
#include "wx/propgrid/advprops.h"


class SPropertyPanel : public wxPanel
{
	DECLARE_DYNAMIC_CLASS(SPropertyPanel)
	wxDECLARE_NO_COPY_CLASS(SPropertyPanel);

public:
	SPropertyPanel();
	SPropertyPanel(wxWindow* parent);

private:
	wxPropertyGridManager*	m_pGridMgr;


	DECLARE_EVENT_TABLE()
};