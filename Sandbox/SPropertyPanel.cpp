#include "SPropertyPanel.h"
#include "SPropertyGrid.h"
#include "STexturePopupWindow.h"
#include "STextureConvertor.h"


IMPLEMENT_DYNAMIC_CLASS(SPropertyTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(SPropertyTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_PROPERTY_TREECTRL, SPropertyTreeCtrl::OnSelChanged)
END_EVENT_TABLE()


SPropertyTreeCtrl::SPropertyTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id)
	: wxTreeCtrl(parent, id,  wxDefaultPosition, wxSize(250, 100), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DESCRIPTION)
	, m_pGrid(pGrid)
{
}

inline const char* PROXY_TYPE_STRING(eENTITY_PROXY_TYPE type)
{
	if( type == ENTITY_PROXY_ACTOR )		return ENUMSTR(ENTITY_PROXY_ACTOR);
	else if( type == ENTITY_PROXY_RENDER)	return ENUMSTR(ENTITY_PROXY_RENDER);
	else if( type == ENTITY_PROXY_CAMERA)	return ENUMSTR(ENTITY_PROXY_CAMERA);
	else
		assert(0);

	return "";
}


void SPropertyTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	m_SeletedItem = event.GetItem();
	m_pGrid->Clear();
	wxString name = GetItemText( m_SeletedItem );

	if( m_SeletedItem == GetRootItem() )
	{
		m_pGrid->Set(m_pEntity);
	}
	else
	{
		wxString strItem = GetItemText(m_SeletedItem);
		
		for( int i=0 ; i < NUM_ENTITY_PROXY_TYPE; ++i )
		{
			if( strItem == PROXY_TYPE_STRING( eENTITY_PROXY_TYPE(i)) )
				m_pGrid->Set( m_pEntity->GetProxy( eENTITY_PROXY_TYPE(i) ) );
		}
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
		IEntityProxy* pEntityProxy = m_pEntity->GetProxy( eENTITY_PROXY_TYPE(i) );
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

	m_pTextureCanvas = new SCanvas(this);
	m_pGridMgr = new SPropertyGrid(this);	
	m_pEntityTreeCtrl = new SPropertyTreeCtrl(this, m_pGridMgr, ID_PROPERTY_TREECTRL);

	pRootSizer->Add(m_pTextureCanvas, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pEntityTreeCtrl, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pGridMgr, wxSizerFlags(1).Center().Expand());

	SetSizerAndFit(pRootSizer);

	SetEmpty();
}

void SPropertyPanel::SetObject( IEntity* pEntity )
{
	m_pEntityTreeCtrl->SetEntity(pEntity);
	m_pGridMgr->Clear();

	m_pTextureCanvas->Hide();
	m_pEntityTreeCtrl->Show();
	m_pGridMgr->Show();

	OrganizeInside();
}

void SPropertyPanel::SetObject( const CResourceTexture* pResource )
{
	m_pGridMgr->Set( pResource );

	SaveTextureToPNGFile( pResource, COLOR_FORMAT_R8G8B8A8_TYPELESS);

	m_pTextureCanvas->SetImage( &wxImage("temp.png") );

	m_pTextureCanvas->Show();
	m_pGridMgr->Show();
	m_pEntityTreeCtrl->Hide();

	OrganizeInside();
}

void SPropertyPanel::SetObject( const CResourceMesh* pResource )
{
	m_pGridMgr->Set( pResource );

	m_pTextureCanvas->Hide();
	m_pEntityTreeCtrl->Hide();
	m_pGridMgr->Show();

	OrganizeInside();
}

void SPropertyPanel::SetEmpty()
{
	m_pGridMgr->Clear();
	m_pEntityTreeCtrl->SetEntity(NULL);

	m_pEntityTreeCtrl->Hide();
	m_pGridMgr->Hide();
	m_pTextureCanvas->Hide();

	OrganizeInside();
}

void SPropertyPanel::OrganizeInside()
{
	int nWidth = 100;
	int nHeight = 100;
	GetSize(&nWidth, &nHeight);
//DoMoveSibling( GetHWND(), nWidth, nHeight, )

	SetSize(nWidth + 1, nHeight);
	SetSize(nWidth - 1, nHeight);
}
