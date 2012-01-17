#pragma once

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

	virtual void PostRender() override;
	
	bool	m_bSetup;

	DECLARE_EVENT_TABLE()
};