#include "S3DViewPanel.h"
#include "SSelectionMgr.h"




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
		m_bSetup = true;
	}

	GLOBAL::Engine()->UpdateAndRender( GLOBAL::ObserverCamera(), this);

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

void S3DViewPanel::PostRender()
{
	IRenderHelper* pRenderHelper	= GLOBAL::Engine()->RenderHelper();
	eTRANSFORM_MODE mode			= GLOBAL::SelectionMgr()->GetTransformMode();

	int FPS = GLOBAL::Engine()->GlobalTimer()->GetFPS();

	RENDER_TEXT_BUFFER textFPS;
	_itow_s(FPS, textFPS.strMsg, 5);
	textFPS.rc.left = 0;
	textFPS.rc.top = 0;
	textFPS.rc.right = 100;
	textFPS.rc.bottom = 100;
	textFPS.clr = CColor( 1.0f, 1.0f, 1.0f, 1.0f );

	pRenderHelper->RenderText(textFPS);
	pRenderHelper->RenderWorldGrid( XMMatrixIdentity(), 5000, 100 );

	TYPE_SELECTED_ENTITIES*	selcetedEntities =	GLOBAL::SelectionMgr()->List();

	for( UINT i=0; i< selcetedEntities->size() ; ++i)
	{
		IEntity* pEntity = (*selcetedEntities)[i];

		if( mode == TRANSFORM_MOVE) pRenderHelper->RenderMover( pEntity->GetWorldTM());
		else if( mode == TRANSFORM_ROTATE) pRenderHelper->RenderRotator( pEntity->GetWorldTM());
		else if( mode == TRANSFORM_SCALE) pRenderHelper->RenderScaler( pEntity->GetWorldTM());
		else pRenderHelper->RenderAxis( pEntity->GetWorldTM());

		const IAABB* pWorldAABB = pEntity->GetWorldAABB();

		if( pWorldAABB->IsValid() )
		{
			pRenderHelper->RenderBox( XMMatrixIdentity(), pWorldAABB->GetMin(), pWorldAABB->GetMax() ,COLOR_GRAY );
			
			const IAABB* pLocalEntityAABB = pEntity->GetLocalEntityAABB();
			if( pLocalEntityAABB->IsValid() )
				pRenderHelper->RenderBox( pEntity->GetWorldTM(), pLocalEntityAABB->GetMin(), pLocalEntityAABB->GetMax() ,COLOR_WHITE );
		}
	}
}

void S3DViewPanel::OnMouseEvent(wxMouseEvent& event)
{
	if( GLOBAL::ObserverCamera() == NULL )
		return;

	if( UpdateDrag(event) )
		return;

	if( event.LeftIsDown() || event.RightIsDown() || event.MiddleIsDown() )
	{
		SetFocus();

		static CVector2 point;

		long x = event.GetX();
		long y = event.GetY();
		CVector2 dPoint = CVector2( point.x - x, point.y - y);
		point.x = x;
		point.y = y;

		if( CVector2::Length(dPoint) > 20 )
		{
			dPoint.x = 0;
			dPoint.y = 0;
		}

		if(  GLOBAL::SelectionMgr()->List()->size() == 0 )
			UpdateObserverCameraWithoutEntity(dPoint, event);
		else
			UpdateObserverCamera(dPoint, event);
	}
}

void S3DViewPanel::UpdateObserverCamera(CVector2& dPoint, wxMouseEvent& e)
{
	IEntity* pCamera = GLOBAL::ObserverCamera()->GetEntity();

	if( e.LeftIsDown() )
	{		
		if( fabs(dPoint.x) > fabs(dPoint.y) )
		{
			XMMATRIX rotTM = XMMatrixRotationAxis( CVector3(0,0,1).ToXMVEECTOR(), dPoint.x * 0.005f );
			rotTM = XMMatrixMultiply( pCamera->GetWorldTM(), rotTM );
			pCamera->SetWorldTM( rotTM );
		}
		else
		{
			XMMATRIX rot = XMMatrixRotationAxis( pCamera->GetWorldTM().r[0] ,dPoint.y * 0.005f);
			rot = XMMatrixMultiply( pCamera->GetWorldTM(), rot);
			pCamera->SetWorldTM(rot);
		}
	}
	else if( e.RightIsDown() )
	{
		pCamera->MoveOnLocalAxis( 0 , 0,  dPoint.y * m_CameraSpeed );
	}
	else if( e.MiddleIsDown() )
	{
		pCamera->MoveOnLocalAxis( -dPoint.x * m_CameraSpeed , dPoint.y * m_CameraSpeed, 0 );
	}
}

void S3DViewPanel::UpdateObserverCameraWithoutEntity(CVector2& dPoint, wxMouseEvent& e)
{
	IEntity* pCamera = GLOBAL::ObserverCamera()->GetEntity();

	if( e.LeftIsDown() )
	{		
		CVector3 origin;
		CVector3 direction;
		GLOBAL::ObserverCamera()->GetPickRayFromScreen( e.GetX(), e.GetY(), origin, direction);
		CVector3 to = origin + direction * GLOBAL::ObserverCamera()->GetDesc().farClip;

		TYPE_ENTITY_LIST list;
		CCollisionLine desc;
		desc.from = origin;
		desc.to = to;
		GLOBAL::SceneRoot()->Pick( desc, list);

		if( list.size() > 0 )
		{
			GLOBAL::SelectionMgr()->Select( *list.begin());
			return;
		}

		if( fabs(dPoint.x) > fabs(dPoint.y) )
			pCamera->RotateLocalAxis( CVector3(0, 0, 1), dPoint.x * 0.005f);
		else
			pCamera->RotateLocalAxis( pCamera->GetWorldTM().r[0], dPoint.y * 0.005f );
	}
	else if( e.RightIsDown() )
	{
		pCamera->MoveOnLocalAxis( 0 , 0,  dPoint.y * m_CameraSpeed );
	}
	else if( e.MiddleIsDown() )
	{
		pCamera->MoveOnLocalAxis( -dPoint.x * m_CameraSpeed , dPoint.y * m_CameraSpeed, 0 );
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
		}
	}
	else if ( e.LeftUp() && s_dragMode != DRAG_NONE)
	{
		s_dragMode = DRAG_NONE;
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
			int dx = abs(e.GetPosition().x - s_dragStartPos.x);
			int dy = abs(e.GetPosition().y - s_dragStartPos.y);

			s_dragStartPos.x = e.GetPosition().x;
			s_dragStartPos.y = e.GetPosition().y;

			GLOBAL::SelectionMgr()->GrabUpdate( dx, dy );
		}
	}
	else
		return false;

	return true;
}