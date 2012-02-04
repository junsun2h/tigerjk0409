#include "SPropertyGrid.h"
#include "SPropertyPanel.h"
#include "SEntityTreeCtrl.h"




IMPLEMENT_DYNAMIC_CLASS(SEntityTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(SEntityTreeCtrl, wxTreeCtrl)
EVT_TREE_SEL_CHANGED(ID_PROPERTY_ENTITY_TREECTRL, SEntityTreeCtrl::OnSelChanged)
END_EVENT_TABLE()



SEntityTreeCtrl::SEntityTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id)
	: wxTreeCtrl(parent, id,  wxDefaultPosition, wxSize(250, 100), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DESCRIPTION)
	, m_pGrid(pGrid)	
{
}

const char* SEntityTreeCtrl::ProxyTypeString(eENTITY_PROXY_TYPE type)
{
	if( type == ENTITY_PROXY_ACTOR )		return ENUMSTR(ENTITY_PROXY_ACTOR);
	else if( type == ENTITY_PROXY_RENDER)	return ENUMSTR(ENTITY_PROXY_RENDER);
	else if( type == ENTITY_PROXY_CAMERA)	return ENUMSTR(ENTITY_PROXY_CAMERA);
	else
		assert(0);

	return "";
}

void SEntityTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	m_pGrid->Clear();
	if(  event.GetItem() == GetRootItem() )
	{
		m_pGrid->Set(m_pEntity);
	}
	else
	{
		wxString strItem = GetItemText( event.GetItem() );
		for( int i=0 ; i < NUM_ENTITY_PROXY_TYPE; ++i )
		{
			if( strItem == ProxyTypeString( eENTITY_PROXY_TYPE(i)) )
				m_pGrid->Set( m_pEntity->GetProxy( eENTITY_PROXY_TYPE(i) ) );
		}
	}
}

void SEntityTreeCtrl::SetEntity(IEntity* pEntity)
{
	DeleteAllItems();

	m_pEntity = pEntity;

	if( m_pEntity == NULL)
		return;

	wxString strName = "Entity (" + pEntity->GetName() + ")";

	wxTreeItemId rootItem = AddRoot( strName );

	for( int i=0 ; i < NUM_ENTITY_PROXY_TYPE; ++i )
	{
		IEntityProxy* pEntityProxy = m_pEntity->GetProxy( eENTITY_PROXY_TYPE(i) );
		if( pEntityProxy != NULL )
			AppendItem(rootItem,pEntityProxy->GetTypeString() );
	}

	Expand(rootItem);
}
