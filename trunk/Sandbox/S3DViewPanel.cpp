#include "wx/wx.h"

#include "CResource.h"
#include "CEngineParam.h"
#include "CQuad.h"
#include "CTimer.h"
#include "CAABB.h"
#include "CCamera.h"
#include "CLight.h"

#include "IEntityProxy.h"
#include "IEntity.h"
#include "IRenderer.h"
#include "SGlobal.h"
#include "SSelectionMgr.h"
#include "SPropertyPanel.h"
#include "ILightMgr.h"

#include "S3DViewPanel.h"

CLightDesc* g_pLight = NULL;
CLightDesc* g_pLight2 = NULL;

void TestLogicUpdate()
{
	static CTimer timer;
	static float angle = 0;

	XMMATRIX rot = XMMatrixRotationZ( angle );
	g_pLight->pos = CVector3::Transform( g_pLight->pos, rot);
	g_pLight2->pos = CVector3::Transform( g_pLight2->pos, rot);

	static double formerTime = timer.GetAbsoluteTime();

	angle += timer.GetElapsedTime() * 0.0000001f;
}

class PostRenderer : public IRenderingCallback
{
public:
	virtual void PostRender() override
	{
		IRenderer* pRenderer	= GLOBAL::Engine()->Renderer();
		eTRANSFORM_MODE mode	= GLOBAL::SelectionMgr()->GetTransformMode();

		int FPS = GLOBAL::Engine()->GlobalTimer()->GetFPS();

		RENDER_TEXT_QUAD textFPS;
		_itow_s(FPS, textFPS.strMsg, 5, 10);
		textFPS.rc.left = 0;
		textFPS.rc.top = 0;
		textFPS.rc.right = 100;
		textFPS.rc.bottom = 100;
		textFPS.clr = CColor( 1.0f, 1.0f, 1.0f, 1.0f );

		pRenderer->RenderText(&textFPS);
		pRenderer->RenderWorldGrid( XMMatrixIdentity(), 5000, 100 );

		TYPE_SELECTED_ENTITIES*	selcetedEntities =	GLOBAL::SelectionMgr()->List();

		for( UINT i=0; i< selcetedEntities->size() ; ++i)
		{
			IEntity* pEntity = (*selcetedEntities)[i];

			// Draw transform helper
			CQuat rot;
			XMMATRIX tm  = XMMatrixIdentity();
			tm.r[3] = pEntity->GetWorldTM().r[3];

			if( mode == TRANSFORM_MOVE) 
			{
				pRenderer->RenderMover( tm );
			}
			else if( mode == TRANSFORM_ROTATE) 
			{
				pRenderer->RenderRotator( tm);
			}
			else if( mode == TRANSFORM_SCALE)
			{
				pRenderer->RenderScaler( tm );
			}

			pRenderer->RenderAxis( pEntity->GetWorldTM() );

			const CAABB* pWorldAABB = pEntity->GetWorldAABB();

			// Draw Bounding Box
			if( pWorldAABB->IsValid() )
			{
				pRenderer->RenderBox( XMMatrixIdentity(), pWorldAABB->GetMin(), pWorldAABB->GetMax() ,COLOR_GRAY );

				const CAABB* pLocalEntityAABB = pEntity->GetLocalEntityAABB();
				if( pLocalEntityAABB->IsValid() )
					pRenderer->RenderBox( pEntity->GetWorldTM(), pLocalEntityAABB->GetMin(), pLocalEntityAABB->GetMax() ,COLOR_WHITE );
			}

			// Draw skeleton if it has a actor
			IEntityProxyActor* pActor = (IEntityProxyActor*)pEntity->GetProxy(ENTITY_PROXY_ACTOR);
			if( pActor != NULL)
			{
				JOINT_ENTITY_LIST* pJointEntitesList = pActor->GetJointEntities();
				CVertexPC buff[512];

				int vertIndex = 0;
				for( UINT i=1; i < pJointEntitesList->size(); ++i)
				{
					buff[vertIndex].vPos = (*pJointEntitesList)[i]->GetWorldPos();
					buff[vertIndex].color = COLOR_RED;
					vertIndex++;
					buff[vertIndex].vPos = (*pJointEntitesList)[i]->GetParent()->GetWorldPos();
					buff[vertIndex].color = COLOR_RED;
					vertIndex++;
				}

				pRenderer->RenderLine( buff, vertIndex );
			}
		}
	}
}g_PostRenderer;



//---------------------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(S3DViewPanel, wxPanel)
	EVT_IDLE(S3DViewPanel::OnIdle)
	EVT_SIZE(S3DViewPanel::OnSize)
	EVT_MOUSE_EVENTS(S3DViewPanel::OnMouseEvent)
	EVT_KEY_DOWN(S3DViewPanel::OnKeyDown)
END_EVENT_TABLE()



S3DViewPanel::S3DViewPanel(wxWindow* parent)
	: wxPanel(parent, ID_PANEL_3DVIEW)
	, m_bSetup(false)
	, m_CameraSpeed(1.0f)
{
}

S3DViewPanel::~S3DViewPanel()
{
	GLOBAL::Engine()->ShutDown();
}

void S3DViewPanel::OnIdle(wxIdleEvent& event)
{
	if( m_bSetup == false )
	{
		int nWidth = 100;
		int nHeight = 100;
		GetSize(&nWidth, &nHeight);

		CENGINE_INIT_PARAM engineParam;
		engineParam.hWnd = GetHWND();
		engineParam.width = nWidth;
		engineParam.height = nHeight;
		engineParam.resourceFolder = L"\\Data";

		GLOBAL::InitDevice( engineParam );

		g_pLight = GLOBAL::Engine()->LightMgr()->Create();

		g_pLight->pos = CVector3(-1000, 1000, 1000);
		g_pLight->range = 2000;
		g_pLight->color = CVector3(1,0,0);
		g_pLight->intensity = 1;

		g_pLight2 = GLOBAL::Engine()->LightMgr()->Create();

		g_pLight2->pos = CVector3(1000, 1000, 1000);
		g_pLight2->range = 2000;
		g_pLight2->color = CVector3(0,1,0);
		g_pLight2->intensity = 1;


		m_bSetup = true;
	}

	TestLogicUpdate();

	if( GLOBAL::ObserverCamera() != NULL)
	{
		CCAMERA_DESC cameraDesc;
		GLOBAL::ObserverCamera()->CopyDesc(&cameraDesc);
		GLOBAL::Engine()->UpdateAndRender( &cameraDesc, &g_PostRenderer);
	}
	
	event.RequestMore();
}

void S3DViewPanel::OnSize(wxSizeEvent& event)
{
	if( m_bSetup )
	{
		wxSize size = event.GetSize();

		GLOBAL::ObserverCamera()->SetProjParam( XM_PIDIV4, size.x, size.y, 1, 10000);
		GLOBAL::Engine()->Resize( size.x, size.y );
	}
}

void S3DViewPanel::OnMouseEvent(wxMouseEvent& event)
{
	if( GLOBAL::ObserverCamera() == NULL )
		return;

	if( UpdateDrag(event) )
		return;

	if( event.LeftDown() )
	{
		SetFocus();

		SSelectionMgr* pSelectMgr = GLOBAL::SelectionMgr();
		if( pSelectMgr->GetTransformMode() == TRANSFORM_SELECT )
		{
			TYPE_ENTITY_LIST list;
			PickFromScreen( list, event.GetX(), event.GetY() );

			if( list.size() > 0 )
			{
				IEntity* pSelectedEntity = *list.begin();

				if( pSelectedEntity != pSelectMgr->First() )
				{
					pSelectMgr->SelectEntity( pSelectedEntity );
					GLOBAL::PropertyPanel()->SetObject( pSelectedEntity );
				}	
			}
		}
	}

	UpdateCamera(event);
}

void S3DViewPanel::UpdateCamera(wxMouseEvent& event)
{
	static wxPoint point;

	if( event.LeftDown() || event.RightDown() || event.MiddleDown() )
		point = event.GetPosition();

	if( event.LeftIsDown() || event.RightIsDown() || event.MiddleIsDown() )
	{
		long x = event.GetX();
		long y = event.GetY();

		CVector2 dPoint = CVector2( point.x - x, point.y - y);
		point.x = x;
		point.y = y;

		IEntity* pCamera = GLOBAL::ObserverCamera()->GetEntity();

		if( event.LeftIsDown() )
		{	
			if( GLOBAL::SelectionMgr()->List()->size() == 0 )
			{
				// FPS camera moving
				if( fabs(dPoint.x) > fabs(dPoint.y) )
					pCamera->RotateLocalAxis( CVector3(0, 0, 1), dPoint.x * 0.005f);
				else
					pCamera->RotateLocalAxis( pCamera->GetWorldTM().r[0], dPoint.y * 0.005f );
			}
			else
			{
				// rotate in entity coordinate system
				IEntity* pEntity = GLOBAL::SelectionMgr()->First();
				XMMATRIX entityTM = XMMatrixIdentity();
				entityTM.r[3] = pEntity->GetWorldPos().ToXMVEECTOR();
				XMMATRIX invEntityTM = XMMATRIX_UTIL::Inverse(NULL, entityTM );

				XMMATRIX tm = XMMatrixMultiply( pCamera->GetWorldTM() , invEntityTM );

				if( fabs(dPoint.x) > fabs(dPoint.y) )
				{
					XMMATRIX rotTM = XMMatrixRotationAxis( CVector3(0,0,1).ToXMVEECTOR(), dPoint.x * 0.005f );
					tm = XMMatrixMultiply( tm, rotTM );

				}
				else
				{
					XMMATRIX rotTM = XMMatrixRotationAxis( pCamera->GetWorldTM().r[0] ,dPoint.y * 0.005f);
					tm = XMMatrixMultiply( tm, rotTM );
				}

				tm = XMMatrixMultiply( tm, entityTM );
				pCamera->SetWorldTM( tm );
			}
		}
		else if( event.RightIsDown() )
		{
			pCamera->MoveOnLocalAxis( 0 , 0,  dPoint.y * m_CameraSpeed );
		}
		else if( event.MiddleIsDown() )
		{
			pCamera->MoveOnLocalAxis( -dPoint.x * m_CameraSpeed , dPoint.y * m_CameraSpeed, 0 );
		}
	}
}


void S3DViewPanel::OnKeyDown(wxKeyEvent& event)
{
	/*
	switch( event.GetKeyCode() )
	{
	}*/
}


bool S3DViewPanel::UpdateDrag(wxMouseEvent& e)
{
	enum eDRAG_MODE
	{
		DRAG_NONE,
		DRAG_START,
		DRAG_DRAGGING
	};

	static eDRAG_MODE	s_dragMode = DRAG_NONE;
	static wxPoint		s_dragStartPos;

	if ( e.LeftDown())
	{
		if( GLOBAL::SelectionMgr()->GrabHelper( e.GetX(), e.GetY() ) )
		{
			s_dragMode = DRAG_START;
			s_dragStartPos = e.GetPosition();
			return true;
		}
	}
	else if ( e.LeftUp() && s_dragMode != DRAG_NONE)
	{
		s_dragMode = DRAG_NONE;
		return true;
	}
	else if ( e.Dragging() && s_dragMode != DRAG_NONE)
	{
		if (s_dragMode == DRAG_START)
		{
			// We will start dragging if we've moved beyond a couple of pixels
			int tolerance = 2;
			int dx = abs(e.GetPosition().x - s_dragStartPos.x);
			int dy = abs(e.GetPosition().y - s_dragStartPos.y);
			if (dx <= tolerance && dy <= tolerance)
				return true;

			s_dragMode = DRAG_DRAGGING;
		}
		else if (s_dragMode == DRAG_DRAGGING)
		{
			GLOBAL::SelectionMgr()->GrabUpdate( e.GetX(), e.GetY() );
		}
		return true;
	}
	
	return false;
}

void S3DViewPanel::PickFromScreen(TYPE_ENTITY_LIST& list, long x, long y)
{
	CVector3 origin;
	CVector3 direction;
	GLOBAL::ObserverCamera()->GetPickRayFromScreen( x, y, origin, direction);
	CVector3 to = origin + direction * GLOBAL::ObserverCamera()->GetDesc()->farClip;

	GLOBAL::SceneRoot()->Pick( origin, to, list);
}


