#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/treectrl.h"

struct IEntity;

enum SCENE_WIDGET_ID
{
	ID_SCENE_FILTER_TEXTCTRL,
	ID_SCENE_TREECTRL,
	ID_SCENE_DELETE,
	ID_SCENE_ADD_COMPONENT,
};

class SSceneHierarchyTreeCtrl : public wxTreeCtrl
{
public:
	SSceneHierarchyTreeCtrl(){}
	SSceneHierarchyTreeCtrl(wxWindow *parent, const wxWindowID id);

	void	OnSelChanged(wxTreeEvent& event);
	void	OnItemMenu(wxTreeEvent& event);
	void	OnDelete(wxCommandEvent& event);

	void	Reload();
	void	FindItem(wxTreeItemId parent, wxString text);

	void	OnDrop(wxPoint point, const wxString& text);
    void	OnBeginDrag(wxTreeEvent& event);

private:
	IEntity*	GetSelectedEntity();
	void		AddEntity(wxTreeItemId parent, IEntity* pEntity);

	DECLARE_DYNAMIC_CLASS(SSceneHierarchyTreeCtrl)
	DECLARE_EVENT_TABLE()
};



class SSceneHierarchyPanel: public wxPanel
{
public:
	SSceneHierarchyPanel(wxWindow* parent);
		
	void	Reload();
	void	SelectEntity(IEntity* pEntity);

private:
	void OnFilterChanged(wxCommandEvent& event);

	SSceneHierarchyTreeCtrl*	m_pTreeCtrl;

	DECLARE_EVENT_TABLE()
};
