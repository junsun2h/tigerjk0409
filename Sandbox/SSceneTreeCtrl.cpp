#include "SPropertyGrid.h"
#include "SPropertyPanel.h"
#include "SSceneTreeCtrl.h"




IMPLEMENT_DYNAMIC_CLASS(SSceneTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(SSceneTreeCtrl, wxTreeCtrl)
EVT_TREE_SEL_CHANGED(ID_PROPERTY_SCENE_TREECTRL, SSceneTreeCtrl::OnSelChanged)
END_EVENT_TABLE()



SSceneTreeCtrl::SSceneTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id)
	: wxTreeCtrl(parent, id,  wxDefaultPosition, wxSize(250, 100))
	, m_pGrid(pGrid)
{
}

void SSceneTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	for(UINT i = 0; i < m_pJointList->size(); ++i )
	{
		if( (*m_pJointList)[i].name == GetItemText( event.GetItem() ) )
		{
			m_pGrid->Set( &(*m_pJointList)[i] );
			break;
		}
	}
}

void SSceneTreeCtrl::SetScene(const JOINT_LIST* pJointList)
{
	DeleteAllItems();

	m_pJointList = pJointList;

	wxTreeItemId rootItem = AddRoot( (*pJointList)[0].name );
	std::vector<wxTreeItemId>	itemList;
	itemList.push_back(rootItem);

	for( UINT i=1 ; i < pJointList->size(); ++i )
	{
		for( UINT iItem = 0; iItem < itemList.size(); ++iItem)
		{
			if( GetItemText( itemList[iItem] ) == (*pJointList)[i].parentName )
			{
				itemList.push_back( AppendItem(itemList[iItem], (*pJointList)[i].name) );
				break;
			}
		}
	}
}
