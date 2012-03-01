#pragma once

enum eANIMATION_CONTROL_IDS
{
	ID_ANIMATION_PALY,
	ID_ANIMATION_STOP,
	ID_ANIMATION_FREEZE,
	ID_ANIMATION_PLAYTIME,
	ID_ANIMATION_LOOP,
};


class SAnimationToolBar: public wxToolBar
{
public:
	SAnimationToolBar(wxWindow *parent, wxWindowID id);

	virtual void OnPlay( wxCommandEvent& event );
	virtual void OnFreeze( wxCommandEvent& event );
	virtual void OnStop( wxCommandEvent& event );
	virtual void OnPlayTimeChanged( wxScrollEvent& event );

private:
	IEntityProxyActor*	GetActor();

	wxCheckBox*		m_pLoop;
};
