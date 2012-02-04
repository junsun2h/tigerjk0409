#include "SSceneHierarchyPanel.h"
#include "wx/treectrl.h"
#include "wx/dnd.h"
#include "SPropertyPanel.h"
#include "SSelectionMgr.h"
#include "SDragAndDropState.h"


IMPLEMENT_DYNAMIC_CLASS(SSceneHierarchyTreeCtrl, wxTreeCtrl)
	BEGIN_EVENT_TABLE(SSceneHierarchyTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_SCENE_TREECTRL, SSceneHierarchyTreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_MENU(ID_SCENE_TREECTRL, SSceneHierarchyTreeCtrl::OnItemMenu)
	EVT_MENU(ID_SCENE_DELETE, SSceneHierarchyTreeCtrl::OnDelete)
    EVT_SET_FOCUS(SSceneHierarchyTreeCtrl::OnFocusGot)
	EVT_TREE_BEGIN_DRAG(wxID_ANY, SSceneHierarchyTreeCtrl::OnBeginDrag)
END_EVENT_TABLE()


class SAssetDragAndDrop : public wxTextDropTarget
{
public:
	SAssetDragAndDrop(SSceneHierarchyTreeCtrl* pParent)
		: m_SceneHierarchyTree(pParent){}
	bool SAssetDragAndDrop::OnDropText(wxCoord x, wxCoord y, const wxString& text)
	{
		m_SceneHierarchyTree->OnDrop( wxPoint(x,y), text);
		return true;
	}
private:
	SSceneHierarchyTreeCtrl*	m_SceneHierarchyTree;
};

//---------------------------------------------------------------------------------------------------------------------

SSceneHierarchyTreeCtrl::SSceneHierarchyTreeCtrl(wxWindow *parent, const wxWindowID id)
	: wxTreeCtrl(parent, id)
{
	SetDropTarget( new SAssetDragAndDrop(this) );
}

void SSceneHierarchyTreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId seletedItem = event.GetItem();
	
	wxString strItem = GetItemText(seletedItem);
	if( strItem == "Root" )
		return;

	wxMenu menu( GetItemText(seletedItem) );
	menu.Append(ID_SCENE_DELETE, wxT("&Delete"));
	menu.Append(ID_SCENE_ADD_COMPONENT, wxT("&ADD Component"));

	PopupMenu(&menu, event.GetPoint());
}

void SSceneHierarchyTreeCtrl::OnFocusGot(wxFocusEvent& event)
{
	wxTreeItemId pSelectedID = GetSelection();

	if( pSelectedID.IsOk() )
	{
		wxTreeEvent e;
		e.SetItem(pSelectedID);
		OnSelChanged(e);
	}
}

void SSceneHierarchyTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	GLOBAL::PropertyPanel()->Empty();
	
	IEntity* pEntity = GetSelectedEntity();
	if( pEntity != NULL)
		GLOBAL::SelectionMgr()->Select(pEntity);
}

void SSceneHierarchyTreeCtrl::OnDelete(wxCommandEvent& event)
{
	wxTreeItemId selectedID = GetSelection();

	if( !selectedID.IsOk() )
		return;

	wxString strItem = GetItemText(selectedID);

	GLOBAL::Engine()->EntityMgr()->RemoveEntity( strItem );

	Reload();
}

IEntity* SSceneHierarchyTreeCtrl::GetSelectedEntity()
{
	wxTreeItemId selectedID = GetSelection();

	if( !selectedID.IsOk() )
		return NULL;

	wxString strItem = GetItemText(selectedID);

	if( strItem == "Root" )
		return NULL;

	return GLOBAL::Engine()->EntityMgr()->GetEntity( strItem.c_str().AsChar() );
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

void SSceneHierarchyTreeCtrl::OnDrop(wxPoint point, const wxString& text)
{
	SDragAndDragDesc desc;
	if( SDragAndDropState::Get(desc) == false )
		return;

	int flags =0;
	wxTreeItemId seletedItem = HitTest( point, flags);
	if (flags & (wxTREE_HITTEST_ABOVE | wxTREE_HITTEST_BELOW | wxTREE_HITTEST_NOWHERE | wxTREE_HITTEST_TOLEFT | wxTREE_HITTEST_TORIGHT))
		return;

	IAssetMgr* pAssetMgr = GLOBAL::Engine()->AssetMgr();
	IEntityMgr* pEntityMgr = GLOBAL::Engine()->EntityMgr();

	IEntity* pEntity = pEntityMgr->GetEntity( GetItemText(seletedItem).c_str().AsChar() );
	if( pEntity == NULL)
		return;

	if( desc.type == DND_ENTITY )
	{		
		IEntity* pTargetEntity = pEntityMgr->GetEntity( desc.strParam.c_str().AsChar() );
		
		pEntity->AttachChild(pTargetEntity);

		Reload();
	}
	else if( desc.type == DND_ACTOR )
	{
		pAssetMgr->GetResource(RESOURCE_ACTOR, desc.strParam.char_str() );
		pEntity->CreateProxy(ENTITY_PROXY_ACTOR);
	}
	else if( desc.type == DND_MESH )
	{
		IEntityProxyRender* pProxy = (IEntityProxyRender*)pEntity->GetProxy(ENTITY_PROXY_RENDER, true);
		pProxy->Insert( pAssetMgr->GetResource(RESOURCE_MESH, desc.strParam.char_str())->RID );
	}

}

void SSceneHierarchyTreeCtrl::OnBeginDrag(wxTreeEvent& WXUNUSED(event))
{
	wxTreeItemId pSelectedID = GetSelection();

	if( pSelectedID.IsOk() )
	{
		if( GetSelectedEntity() == NULL )
			return;

		SDragAndDragDesc desc;
		desc.type = DND_ENTITY;
		desc.strParam = GetItemText(pSelectedID);
		SDragAndDropState::Do(desc, this);
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

	SetSizerAndFit(pRootSizer);
}


void SSceneHierarchyPanel::OnFilterChanged(wxCommandEvent& event)
{

}


void SSceneHierarchyPanel::Reload()
{
	m_pTreeCtrl->Reload();
}

void SSceneHierarchyPanel::SelectEntity(IEntity* pEntity)
{
	//TODO
}
