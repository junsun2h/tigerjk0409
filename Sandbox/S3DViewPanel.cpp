#include "S3DViewPanel.h"
#include "SEntitySelection.h"



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
	IEngine* pEngine = GLOBAL::Engine();

	int FPS = pEngine->GlobalTimer()->GetFPS();

	RENDER_TEXT_BUFFER textFPS;
	_itow_s(FPS, textFPS.strMsg, 5);
	textFPS.rc.left = 0;
	textFPS.rc.top = 0;
	textFPS.rc.right = 100;
	textFPS.rc.bottom = 100;
	textFPS.clr = CColor( 1.0f, 1.0f, 1.0f, 1.0f );

	pEngine->RenderHelper()->RenderText(textFPS);
	pEngine->RenderHelper()->RenderGrid( XMMatrixIdentity(), 5000, 100 );

	TYPE_SELECTED_ENTITIES*	selcetedEntities =	GLOBAL::EntitySelection()->List();

	for( UINT i=0; i< selcetedEntities->size() ; ++i)
	{
		IEntity* pEntity = (*selcetedEntities)[i];
		const IAABB* pWorldAABB = pEntity->GetWorldAABB();

		if( pWorldAABB->IsValid() )
		{
			pEngine->RenderHelper()->RenderBox( XMMatrixIdentity(), pWorldAABB->GetMin(), pWorldAABB->GetMax() ,COLOR_GRAY );
			
			const IAABB* pLocalEntityAABB = pEntity->GetLocalEntityAABB();
			if( pLocalEntityAABB->IsValid() )
				pEngine->RenderHelper()->RenderBox( pEntity->GetWorldTM(), pLocalEntityAABB->GetMin(), pLocalEntityAABB->GetMax() ,COLOR_WHITE );
		}
	}
}

void S3DViewPanel::OnMouseEvent(wxMouseEvent& event)
{
	if( GLOBAL::ObserverCamera() == NULL )
		return;

	static CVector2 point;

	long x = 0;
	long y = 0;
	event.GetPosition( &x, &y );

	CVector2 dPoint = CVector2( point.x - x, point.y - y);
	point.x = x;
	point.y = y;

	IEntity* pCamera = GLOBAL::ObserverCamera()->GetEntity();
	if( event.LeftIsDown() )
	{
		SetFocus();
		if( fabs(dPoint.x) > fabs(dPoint.y) )
			pCamera->RotateLocalAxis( CVector3(0,0,1), dPoint.x * 0.005f );
		else
			pCamera->RotateLocalAxis( pCamera->GetWorldTM().r[0], dPoint.y * 0.005f );
	}
	else if( event.RightIsDown() )
	{
		SetFocus();
		pCamera->MoveLocalAxis( 0 , 0,  dPoint.y * m_CameraSpeed );
	}
	else if( event.MiddleIsDown() )
	{
		SetFocus();
		pCamera->MoveLocalAxis( -dPoint.x * m_CameraSpeed , dPoint.y * m_CameraSpeed, 0 );
	}

	int delta = event.GetWheelRotation();
}

void S3DViewPanel::OnKeyDown(wxKeyEvent& event)
{
	IEntity* pCamera = GLOBAL::ObserverCamera()->GetEntity();
	
	switch( event.GetKeyCode() )
	{
	case 'W':	pCamera->MoveLocalAxis( 0, 0, 100 * m_CameraSpeed);		break;
	case 'S':	pCamera->MoveLocalAxis( 0, 0, -100 * m_CameraSpeed);		break;
	case 'A':	pCamera->MoveLocalAxis( -100 * m_CameraSpeed, 0, 0);		break;
	case 'D':	pCamera->MoveLocalAxis( 100 * m_CameraSpeed, 0, 0);		break;
	}
}