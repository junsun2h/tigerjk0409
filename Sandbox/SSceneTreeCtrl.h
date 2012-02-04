#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/treectrl.h"

class SPropertyGrid;

class SSceneTreeCtrl : public wxTreeCtrl
{
public:
	SSceneTreeCtrl(){}
	SSceneTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id);

	void				OnSelChanged(wxTreeEvent& event);
	void				SetJoint(CJoint* pJoint);

private:
	SPropertyGrid*		m_pGrid;
	CJoint*				m_pJoint;

	DECLARE_DYNAMIC_CLASS(SSceneTreeCtrl)
	DECLARE_EVENT_TABLE()
};
