#include "SSceneHierarchyPanel.h"
#include "wx/treectrl.h"
#include "SPropertyPanel.h"


IMPLEMENT_DYNAMIC_CLASS(SSceneHierarchyTreeCtrl, wxTreeCtrl)
	BEGIN_EVENT_TABLE(SSceneHierarchyTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_SCENE_TREECTRL, SSceneHierarchyTreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_MENU(ID_SCENE_TREECTRL, SSceneHierarchyTreeCtrl::OnItemMenu)
	EVT_MENU(ID_SCENE_DELETE, SSceneHierarchyTreeCtrl::OnDelete)
END_EVENT_TABLE()


SSceneHierarchyTreeCtrl::SSceneHierarchyTreeCtrl(wxWindow *parent, const wxWindowID id)
	: wxTreeCtrl(parent, id)
{
}

void SSceneHierarchyTreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	m_SeletedItem = event.GetItem();
	
	wxString strItem = GetItemText(m_SeletedItem);
	if( strItem == "Root" )
		return;

	wxMenu menu( GetItemText(m_SeletedItem) );
	menu.Append(ID_SCENE_DELETE, wxT("&Delete"));

	PopupMenu(&menu, event.GetPoint());
}


void SSceneHierarchyTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	GLOBAL::PropertyPanel()->SetEmpty();

	m_SeletedItem = event.GetItem();
	wxString strItem = GetItemText(m_SeletedItem);

	if( strItem == "Root" )
		return;

	IEntity* pEntity = GLOBAL::Engine()->EntityMgr()->GetEntity( strItem.c_str().AsChar() );
	GLOBAL::PropertyPanel()->SetObject(pEntity );
}

void SSceneHierarchyTreeCtrl::OnDelete(wxCommandEvent& event)
{
	wxString strItem = GetItemText(m_SeletedItem);

	GLOBAL::Engine()->EntityMgr()->RemoveEntity( strItem );

	Reload();
}

void SSceneHierarchyTreeCtrl::AddEntity(wxTreeItemId parent, IEntity* pEntity)
{
	wxTreeItemId item = AppendItem(parent, pEntity->GetName() );

	int childNum = pEntity->GetChildCount();
	for(int i=0; i< childNum; ++i)
	{
		IEntity* pChild = pEntity->GetChild(i);
		AddEntity(item, pChild);
	}
}

void SSceneHierarchyTreeCtrl::Reload()
{
	IEntity* pEntity = GLOBAL::SceneRoot();

	DeleteAllItems();
	wxTreeItemId rootItem = AddRoot( pEntity->GetName() );
	
	int childNum =  pEntity->GetChildCount();
	for(int i=0; i< childNum; ++i)
	{
		IEntity* pChild = pEntity->GetChild(i);
		AddEntity(rootItem, pChild);
	}
}


//-------------------------------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SSceneHierarchyPanel, wxPanel)
	EVT_TEXT(ID_SCENE_FILTER_TEXTCTRL, SSceneHierarchyPanel::OnFilterChanged)
END_EVENT_TABLE()


SSceneHierarchyPanel::SSceneHierarchyPanel(wxWindow* parent)
	:  wxPanel(parent, ID_PANEL_SCENE_HIERARCHY)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* pTextBoxCtrl = new wxTextCtrl(this, ID_SCENE_FILTER_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
	pRootSizer->Add(pTextBoxCtrl, wxSizerFlags(0).Top().Border());

	m_pTreeCtrl = new SSceneHierarchyTreeCtrl(this, ID_SCENE_TREECTRL);
	pRootSizer->Add(m_pTreeCtrl, wxSizerFlags(1).Center().Border().Expand());
/*	{
		wxIcon icons[ICONID_NUM];
		icons[ICONID_ACTOR] = wxIcon("res/actor.png", wxBITMAP_TYPE_PNG);
		icons[ICONID_ACTORNODE] = wxIcon("res/actornode.png", wxBITMAP_TYPE_PNG);

		int width = icons[0].GetWidth();
		int height = icons[0].GetHeight();
		wxImageList* pImageList = new wxImageList(width, height, true);
		for( size_t i = 0; i < _countof(icons); ++i )
		{
			_ASSERT(icons[i].IsOk());
			pImageList->Add(icons[i]);
		}
		pTreeCtrl->AssignImageList(pImageList);
	}*/

	SetSizerAndFit(pRootSizer);
}


void SSceneHierarchyPanel::OnFilterChanged(wxCommandEvent& event)
{

}


void SSceneHierarchyPanel::Reload()
{
	m_pTreeCtrl->Reload();
}