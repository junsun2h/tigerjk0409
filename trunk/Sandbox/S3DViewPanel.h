#pragma once

#include "wx/wx.h"


class S3DViewPanel : public wxPanel
{
public:
	S3DViewPanel(wxWindow* parent);
	virtual ~S3DViewPanel();

	bool	InitDevice();

private:
	void	OnIdle(wxIdleEvent& event);
	void	OnSize(wxSizeEvent& event);

	DECLARE_EVENT_TABLE()
};