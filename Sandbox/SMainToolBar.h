#pragma once

#include "SGlobal.h"


class SMainToolBar: public wxToolBar
{
public:
	SMainToolBar(wxWindow *parent, wxWindowID id);

	virtual void OnSelectMode( wxCommandEvent& event );
	virtual void OnPositionMode( wxCommandEvent& event );
	virtual void OnRotateMode( wxCommandEvent& event );
	virtual void OnScaleMode( wxCommandEvent& event );
};
