#pragma once

#include "wx/wx.h"
#include "SGlobal.h"


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

	void	UpdateCamera(wxMouseEvent& event);
	bool	UpdateDrag(wxMouseEvent& e);

	void	PickFromScreen(TYPE_ENTITY_LIST& list, long x, long y);

	virtual void PostRender() override;
	
	bool				m_bSetup;

	DECLARE_EVENT_TABLE()
};