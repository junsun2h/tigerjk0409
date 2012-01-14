#pragma once

#include "SGlobal.h"
#include "wx/treectrl.h"

struct IEntity;

class SSceneHierarchyreeCtrl : public wxTreeCtrl
{
public:
	SSceneHierarchyreeCtrl(){}
	SSceneHierarchyreeCtrl(wxWindow *parent, const wxWindowID id);

	void	OnSelChanged(wxTreeEvent& event);
	void	OnItemMenu(wxTreeEvent& event);
	void	OnDelete(wxCommandEvent& event);

	void	Reload();

private:
	void	AddEntity(wxTreeItemId parent, IEntity* pEntity);

	wxTreeItemId	m_SeletedItem;

	DECLARE_DYNAMIC_CLASS(SSceneHierarchyreeCtrl)
	DECLARE_EVENT_TABLE()
};



class SSceneHierarchyPanel: public wxPanel
{
public:
	SSceneHierarchyPanel(wxWindow* parent);
		
	void	Reload();

private:
	void OnFilterChanged(wxCommandEvent& event);

	SSceneHierarchyreeCtrl*	m_pTreeCtrl;

	DECLARE_EVENT_TABLE()
};
