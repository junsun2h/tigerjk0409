#include "wx/wx.h"

#include "CResource.h"

#include "IMotionInstance.h"
#include "IEntityProxy.h"
#include "IEntity.h"

#include "SGlobal.h"

#include "SAnimationToolBar.h"
#include "SSelectionMgr.h"


SAnimationToolBar::SAnimationToolBar(wxWindow *parent, wxWindowID id)
	: wxToolBar(parent, id,  wxDefaultPosition, wxSize(250, 35))
{	
	AddRadioTool( ID_ANIMATION_STOP, "stop", wxImage("res/stop.png") );
	AddRadioTool( ID_ANIMATION_PALY, "play", wxImage("res/play.png") );
	AddRadioTool( ID_ANIMATION_FREEZE, "freeze", wxImage("res/freeze.png") );

	AddControl(new wxSlider(this, ID_ANIMATION_PLAYTIME, 0, 0, 100, wxDefaultPosition, wxSize(90, -1), wxSL_HORIZONTAL ) );
	m_pLoop = new wxCheckBox( this, ID_ANIMATION_LOOP, wxT("Loop"), wxDefaultPosition, wxDefaultSize, 0 );
	AddControl( m_pLoop );

	this->Connect( ID_ANIMATION_PALY, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SAnimationToolBar::OnPlay ) );
	this->Connect( ID_ANIMATION_FREEZE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SAnimationToolBar::OnFreeze ) );
	this->Connect( ID_ANIMATION_STOP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SAnimationToolBar::OnStop ) );
	
	this->Connect( ID_ANIMATION_PLAYTIME, wxEVT_SCROLL_CHANGED, wxScrollEventHandler( SAnimationToolBar::OnPlayTimeChanged ) );

	Realize();
}

void SAnimationToolBar::OnPlay( wxCommandEvent& event )
{
	IEntity* pEntity = GLOBAL::SelectionMgr()->First();

	if( pEntity == NULL )
		return;

	IEntityProxyActor* pActorProxy = (IEntityProxyActor*)pEntity->GetProxy(ENTITY_PROXY_ACTOR);
	if( pActorProxy == NULL)
		return;

	wxString strMotion = GLOBAL::SelectionMgr()->GetSelectedMotion();
	const CResourceActor* pActor = pActorProxy->GetResource();

	for( UINT i=0; i < pActor->motionList.size(); ++i )
	{
		if( strMotion == pActor->motionList[i]->name )
		{
			CMotionDesc desc;
			desc.pResource = pActor->motionList[i];

			if( m_pLoop->IsChecked())
				desc.bLoop = true;

			pActorProxy->Play( &desc );
		}
	}
}

void SAnimationToolBar::OnFreeze( wxCommandEvent& event )
{
}

void SAnimationToolBar::OnStop( wxCommandEvent& event )
{
	IEntityProxyActor* pActor = GetActor();
	if( pActor == NULL )
		return;

	if( pActor->IsPlaying() )
		pActor->Freeze();
}

void SAnimationToolBar::OnPlayTimeChanged( wxScrollEvent& event )
{
}

IEntityProxyActor*	SAnimationToolBar::GetActor()
{
	IEntity* pEntity = GLOBAL::SelectionMgr()->First();

	if( pEntity == NULL )
		return NULL;

	return (IEntityProxyActor*)pEntity->GetProxy(ENTITY_PROXY_ACTOR);
}
