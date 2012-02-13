#pragma once

class SPropertyGrid;

class SSceneTreeCtrl : public wxTreeCtrl
{
public:
	SSceneTreeCtrl(){}
	SSceneTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id);

	void				OnSelChanged(wxTreeEvent& event);
	void				SetScene(const JOINT_LIST* pJointList);

private:
	SPropertyGrid*		m_pGrid;
	const JOINT_LIST*	m_pJointList;

	DECLARE_DYNAMIC_CLASS(SSceneTreeCtrl)
	DECLARE_EVENT_TABLE()
};
