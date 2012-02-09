#include "SPropertyGrid.h"
#include "STexturePopupWindow.h"
#include "STextureConvertor.h"
#include "SEntityTreeCtrl.h"
#include "SSceneTreeCtrl.h"
#include "SAnimationToolBar.h"
#include "SMotionTreeCtrl.h"

#include "SPropertyPanel.h"


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
	m_pSceneTreeCtrl = new SSceneTreeCtrl(this, m_pGridMgr, ID_PROPERTY_SCENE_TREECTRL);
	m_pAnimationTooBar = new SAnimationToolBar(this, ID_PROPERTY_ANIMATION_TOOLBAR);
	m_pMotionTreeCtrl = new SMotionTreeCtrl(this, m_pGridMgr, ID_PROPERTY_MOTION_TREECTRL);
	
	pRootSizer->Add(m_pAnimationTooBar, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pTextureCanvas, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pEntityTreeCtrl, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pMotionTreeCtrl, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pSceneTreeCtrl, 0, wxALL|wxEXPAND, 5);
	pRootSizer->Add(m_pGridMgr, wxSizerFlags(1).Center().Expand());
	
	SetSizerAndFit(pRootSizer);

	Empty();
}

void SPropertyPanel::SetObject( IEntity* pEntity )
{
	m_pEntityTreeCtrl->SetEntity(pEntity);
	m_pGridMgr->Clear();

	IEntityProxyActor* pActor = (IEntityProxyActor*)pEntity->GetProxy(ENTITY_PROXY_ACTOR );
	if( pActor )
	{
		OrganizeInside( ENTITY_TREE_CTRL | MOTION_TREE_CTRL | PROPERTY_GRID | ANIMATION_TOOLBAR);
		m_pMotionTreeCtrl->SetActor( pActor->GetResource() );
	}
	else
		OrganizeInside( ENTITY_TREE_CTRL | PROPERTY_GRID );
}

void SPropertyPanel::SetObject( const CResourceBase* pResource )
{
	if( pResource->Type() == RESOURCE_ACTOR )			SetObject( (const CResourceActor*)pResource);
	else if( pResource->Type() == RESOURCE_TEXTURE )	SetObject( (const CResourceTexture*)pResource);
	else if( pResource->Type() == RESOURCE_MESH )		SetObject( (const CResourceMesh*)pResource);
	else if( pResource->Type() == RESOURCE_MOTION )		SetObject( (const CResourceMotion*)pResource);
	else if( pResource->Type() == RESOURCE_MATERIAL)	SetObject( (const CResourceMtrl*)pResource);
}

void SPropertyPanel::SetObject( const CResourceTexture* pResource )
{
	m_pGridMgr->Set( pResource );
	SaveTextureToPNGFile( pResource, COLOR_FORMAT_R8G8B8A8_TYPELESS);
	m_pTextureCanvas->SetImage( &wxImage("temp.png") );
	OrganizeInside( PROPERTY_GRID | TEXTURE_CANVAS );
}

void SPropertyPanel::SetObject( const CResourceMesh* pResource )
{
	m_pGridMgr->Set( pResource );
	OrganizeInside( PROPERTY_GRID );
}

void SPropertyPanel::SetObject( const CResourceActor* pResource )
{
	m_pSceneTreeCtrl->SetScene( &pResource->jointList );
	m_pGridMgr->Set( pResource );
	m_pMotionTreeCtrl->SetActor( pResource );
	OrganizeInside( PROPERTY_GRID | MOTION_TREE_CTRL | SCENE_TREE_CTRL );
}

void SPropertyPanel::SetObject( const CResourceMotion* pResource )
{
	m_pGridMgr->Set( pResource );
	OrganizeInside( PROPERTY_GRID );
}

void SPropertyPanel::Empty()
{
	m_pGridMgr->Clear();
	m_pEntityTreeCtrl->SetEntity(NULL);

	OrganizeInside( 0 );
}

void SPropertyPanel::OrganizeInside(DWORD flag)
{
	if( ( flag & TEXTURE_CANVAS ) == TEXTURE_CANVAS )
		m_pTextureCanvas->Show();
	else
		m_pTextureCanvas->Hide();

	if( ( flag & SCENE_TREE_CTRL ) == SCENE_TREE_CTRL )
		m_pSceneTreeCtrl->Show();
	else
		m_pSceneTreeCtrl->Hide();

	if( ( flag & ENTITY_TREE_CTRL ) == ENTITY_TREE_CTRL )
		m_pEntityTreeCtrl->Show();
	else
		m_pEntityTreeCtrl->Hide();

	if( ( flag & PROPERTY_GRID ) == PROPERTY_GRID )
		m_pGridMgr->Show();
	else
		m_pGridMgr->Hide();

	if( ( flag & ANIMATION_TOOLBAR ) == ANIMATION_TOOLBAR )
		m_pAnimationTooBar->Show();
	else
		m_pAnimationTooBar->Hide();

	if( ( flag & MOTION_TREE_CTRL ) == MOTION_TREE_CTRL )
		m_pMotionTreeCtrl->Show();
	else
		m_pMotionTreeCtrl->Hide();

	
	GetSizer()->FitInside( this );
	GetSizer()->RecalcSizes();
}