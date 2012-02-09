#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/treectrl.h"

class SPropertyGrid;

class SMotionTreeCtrl : public wxTreeCtrl
{
public:
	SMotionTreeCtrl(){}
	SMotionTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id);

	void			OnSelChanged(wxTreeEvent& event);
	void			SetActor(const CResourceActor* pActor);
	void			OnDrop(wxPoint point, const wxString& text);

private:
	const CResourceActor*	m_pActor;
	SPropertyGrid*			m_pGrid;

	DECLARE_DYNAMIC_CLASS(SEntityTreeCtrl)
	DECLARE_EVENT_TABLE()
};
