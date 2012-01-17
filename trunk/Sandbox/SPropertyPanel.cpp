#include "SPropertyPanel.h"
#include "SPropertyGrid.h"


IMPLEMENT_DYNAMIC_CLASS(SPropertyTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(SPropertyTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_PROPERTY_TREECTRL, SPropertyTreeCtrl::OnSelChanged)
END_EVENT_TABLE()


SPropertyTreeCtrl::SPropertyTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id)
	: wxTreeCtrl(parent, id,  wxDefaultPosition, wxSize(250, 100), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DESCRIPTION)
	, m_pGrid(pGrid)
{
}

void SPropertyTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	m_SeletedItem = event.GetItem();
	m_pGrid->Clear();
	wxString name = GetItemText( m_SeletedItem );

	if( m_SeletedItem == GetRootItem() )
	{
		m_pGrid->SetProperty(m_pEntity);
	}
}

void SPropertyTreeCtrl::SetEntity(IEntity* pEntity)
{
	DeleteAllItems();

	m_pEntity = pEntity;

	if( m_pEntity == NULL)
		return;

	wxString strName = "Entity (" + pEntity->GetName() + ")";

	wxTreeItemId rootItem = AddRoot( strName );

	for( int i=0 ; i < NUM_ENTITY_PROXY_TYPE; ++i )
	{
		IEntityProxy* pEntityProxy = m_pEntity->GetProxy( ENTITY_PROXY_TYPE(i) );
		if( pEntityProxy != NULL )
			AppendItem(rootItem,pEntityProxy->GetTypeString() );
	}

	Expand(rootItem);
}



//-------------------------------------------------------------------------------------------------------------------
IMPLEMENT_DYNAMIC_CLASS(SPropertyPanel, wxPanel)
BEGIN_EVENT_TABLE(SPropertyPanel, wxPanel)
END_EVENT_TABLE()


SPropertyPanel::SPropertyPanel()
{

}

SPropertyPanel::SPropertyPanel(wxWindow* parent) 
	: wxPanel(parent, ID_PANEL_PROPERTY)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);

	m_pGridMgr = new SPropertyGrid(this);	
	m_pTreeCtrl = new SPropertyTreeCtrl(this, m_pGridMgr, ID_PROPERTY_TREECTRL);

	pRootSizer->Add(m_pTreeCtrl, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pGridMgr, wxSizerFlags(1).Center().Expand());

	SetSizerAndFit(pRootSizer);
}

void SPropertyPanel::SetObject( IEntity* pEntity )
{
	m_pTreeCtrl->SetEntity(pEntity);
	m_pGridMgr->Clear();
}