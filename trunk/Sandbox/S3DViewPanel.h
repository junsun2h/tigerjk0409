#pragma once

#include "wx/wx.h"
#include "SGlobal.h"
#include "IRenderCallback.h"


struct IEntityProxyCamera;

class S3DViewPanel : public wxPanel, IRenderingCallback
{
public:
	S3DViewPanel(wxWindow* parent);
	virtual ~S3DViewPanel();

	float	m_CameraSpeed;

private:
	void	OnIdle(wxIdleEvent& event);
	void	OnSize(wxSizeEvent& event);
	void	OnMouseEvent(wxMouseEvent& event);
	void	OnKeyDown(wxKeyEvent& event);

	void	UpdateEntityCamera(CVector2& dPoint, long x, long y, wxMouseEvent& event);
	void	UpdateFPSCamera(CVector2& dPoint, long x, long y, wxMouseEvent& event);

	virtual void PostRender() override;
	
	bool	m_bSetup;

	DECLARE_EVENT_TABLE()
};