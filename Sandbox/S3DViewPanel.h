#pragma once

#include "wx/wx.h"
#include "IRenderCallback.h"


struct IEntityProxyCamera;

class S3DViewPanel : public wxPanel, IRenderingCallback
{
public:
	S3DViewPanel(wxWindow* parent);
	virtual ~S3DViewPanel();

	bool	InitDevice();

private:
	void	OnIdle(wxIdleEvent& event);
	void	OnSize(wxSizeEvent& event);

	virtual void PostRender() override;

	void	Setup();

	DECLARE_EVENT_TABLE()

private:
	IEntityProxyCamera* m_pCamera;
};