#include "SPropertyGrid.h"
#include "SPropertyPanel.h"
#include "SSceneTreeCtrl.h"




IMPLEMENT_DYNAMIC_CLASS(SSceneTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(SSceneTreeCtrl, wxTreeCtrl)
EVT_TREE_SEL_CHANGED(ID_PROPERTY_SCENE_TREECTRL, SSceneTreeCtrl::OnSelChanged)
END_EVENT_TABLE()



SSceneTreeCtrl::SSceneTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id)
	: wxTreeCtrl(parent, id,  wxDefaultPosition, wxSize(250, 100), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DESCRIPTION)
	, m_pGrid(pGrid)
	, m_pJoint(NULL)
{
}

void SSceneTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	if( event.GetItem() != GetRootItem() )
	{
		m_pGrid->Clear();
		wxString strItem = GetItemText( event.GetItem() );
	}
}

void SSceneTreeCtrl::SetJoint(CJoint* pJoint)
{
	DeleteAllItems();

}
