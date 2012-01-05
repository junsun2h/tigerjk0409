#pragma once

#include "wx/wx.h"

struct IEntityProxyCamera;

class S3DViewPanel : public wxPanel
{
public:
	S3DViewPanel(wxWindow* parent);
	virtual ~S3DViewPanel();

	bool	InitDevice();

private:
	void	OnIdle(wxIdleEvent& event);
	void	OnSize(wxSizeEvent& event);

	void	DrawHelper();
	void	Setup();

	DECLARE_EVENT_TABLE()

private:
	IEntityProxyCamera* m_pCamera;
};