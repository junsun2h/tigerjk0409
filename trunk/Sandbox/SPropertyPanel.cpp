#include "SPropertyPanel.h"
#include "SPropertyGrid.h"
#include "STexturePopupWindow.h"
#include "STextureConvertor.h"
#include "SEntityTreeCtrl.h"



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
	m_pEntityTreeCtrl = new SEntityTreeCtrl(this, m_pGridMgr, ID_PROPERTY_ENTITY_TREECTRL);
	m_pSceneTreeCtrl = new wxTreeCtrl(this);

	pRootSizer->Add(m_pTextureCanvas, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pEntityTreeCtrl, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pSceneTreeCtrl, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pGridMgr, wxSizerFlags(1).Center().Expand());

	SetSizerAndFit(pRootSizer);

	Empty();
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

void SPropertyPanel::SetObject( const CResourceActor* pResource )
{
	m_pGridMgr->Set( pResource );

	{
		m_pSceneTreeCtrl->DeleteAllItems();

		wxTreeItemId rootItem = m_pSceneTreeCtrl->AddRoot( pResource->name );
		{

		}
	}

	m_pTextureCanvas->Hide();
	m_pEntityTreeCtrl->Hide();
	m_pSceneTreeCtrl->Show();
	m_pGridMgr->Show();

	OrganizeInside();
}

void SPropertyPanel::Empty()
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
	GetSizer()->FitInside( this );
	GetSizer()->RecalcSizes();
}
