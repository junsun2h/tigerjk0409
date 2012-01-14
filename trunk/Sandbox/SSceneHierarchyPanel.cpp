#include "SSceneHierarchyPanel.h"
#include "wx/treectrl.h"
#include "SPropertyPanel.h"


IMPLEMENT_DYNAMIC_CLASS(SSceneHierarchyreeCtrl, wxTreeCtrl)
	BEGIN_EVENT_TABLE(SSceneHierarchyreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_SCENE_TREECTRL, SSceneHierarchyreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_MENU(ID_SCENE_TREECTRL, SSceneHierarchyreeCtrl::OnItemMenu)
	EVT_MENU(ID_SCENE_DELETE, SSceneHierarchyreeCtrl::OnDelete)
END_EVENT_TABLE()


SSceneHierarchyreeCtrl::SSceneHierarchyreeCtrl(wxWindow *parent, const wxWindowID id)
	: wxTreeCtrl(parent, id)
{
}

void SSceneHierarchyreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	m_SeletedItem = event.GetItem();

	wxMenu menu( GetItemText(m_SeletedItem) );
	menu.Append(ID_SCENE_DELETE, wxT("&ADD Component"));
	menu.Append(ID_SCENE_DELETE, wxT("&Delete"));

	PopupMenu(&menu, event.GetPoint());
}


void SSceneHierarchyreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	m_SeletedItem = event.GetItem();
	
	std::string entityName  = GetItemText(m_SeletedItem);

	IEntity* pEntity = GLOBAL::Engine()->EntityMgr()->GetEntity( entityName );

	GLOBAL::PropertyPanel()->SetProperty(pEntity );
}

void SSceneHierarchyreeCtrl::OnDelete(wxCommandEvent& event)
{
	IAssetMgr* pAssetMgr =GLOBAL::Engine()->AssetMgr();

	wxString parentText = GetItemText( GetItemParent( m_SeletedItem ) );
	std::string strSeletedItem =  GetItemText(m_SeletedItem).c_str();

	for( int i = RESOURCE_TEXTURE; i< RESOURCE_SHADER; ++i	)
	{
		/*
		if( strAssetType[i] == parentText )
		{
			pAssetMgr->Remove( RESOURCE_TYPE(i), strSeletedItem );
			break;
		}*/
	}

	Reload();
}

void SSceneHierarchyreeCtrl::AddEntity(wxTreeItemId parent, IEntity* pEntity)
{
	wxTreeItemId item = AppendItem(parent, pEntity->GetName() );

	int childNum = pEntity->GetChildCount();
	for(int i=0; i< childNum; ++i)
	{
		IEntity* pChild = pEntity->GetChild(i);
		AddEntity(item, pChild);
	}
}

void SSceneHierarchyreeCtrl::Reload()
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



BEGIN_EVENT_TABLE(SSceneHierarchyPanel, wxPanel)
	EVT_TEXT(ID_SCENE_FILTER_TEXTCTRL, SSceneHierarchyPanel::OnFilterChanged)
END_EVENT_TABLE()


SSceneHierarchyPanel::SSceneHierarchyPanel(wxWindow* parent)
	:  wxPanel(parent, ID_PANEL_SCENE_HIERARCHY)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* pTextBoxCtrl = new wxTextCtrl(this, ID_SCENE_FILTER_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
	pRootSizer->Add(pTextBoxCtrl, wxSizerFlags(0).Top().Border());

	m_pTreeCtrl = new SSceneHierarchyreeCtrl(this, ID_SCENE_TREECTRL);
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