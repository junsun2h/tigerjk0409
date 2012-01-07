#include "S3DViewPanel.h"
#include "IEngine.h"

IEngine* gEng = NULL;



BEGIN_EVENT_TABLE(S3DViewPanel, wxPanel)
	EVT_IDLE(S3DViewPanel::OnIdle)
	EVT_SIZE(S3DViewPanel::OnSize)
END_EVENT_TABLE()



S3DViewPanel::S3DViewPanel(wxWindow* parent)
	: wxPanel(parent)
	, m_pCamera(NULL)
{
}

S3DViewPanel::~S3DViewPanel()
{
	gEng->ShutDown();
}

bool S3DViewPanel::InitDevice()
{
	int nWidth = 100;
	int nHeight = 100;
	GetSize(&nWidth, &nHeight);

	CENGINE_INIT_PARAM engineParam;
	engineParam.hWnd = GetHWND();
	engineParam.width = nWidth;
	engineParam.height = nHeight;

	HMODULE	engineDll = ::LoadLibrary( L"Engine.dll");
	if( engineDll == NULL )
		return false;

	typedef IEngine *(*CREATE_ENGINE)();
	CREATE_ENGINE FuncCreateEngine = NULL;

	FuncCreateEngine = (CREATE_ENGINE)::GetProcAddress( engineDll, "CreateEngine" );

	if( FuncCreateEngine == NULL )
		return false;

	gEng = FuncCreateEngine();
	if( gEng->StartUp( engineParam ) == false )
		return false;

	Setup();

	return true;
}

void S3DViewPanel::Setup()
{
	int nWidth = 100;
	int nHeight = 100;
	GetSize(&nWidth, &nHeight);

	IEntityMgr* entityMgr = gEng->EntityMgr();

	IEntity* pEntity = entityMgr->SpawnEntity( "Camera" );
	m_pCamera = (IEntityProxyCamera*)entityMgr->SpawnEntityProxy("Main Camera" , ENTITY_PROXY_CAMERA);
	
	m_pCamera->SetProjParam( XM_PIDIV4,  nWidth, nHeight, 1, 10000);
	m_pCamera->SetViewParam( CVector3(1000, -1000, 1000), CVector3(0, 0, 0), CVector3(0.0f, 0.0f, 1.0f) );

	pEntity->SetProxy( m_pCamera );
}

void S3DViewPanel::OnIdle(wxIdleEvent& event)
{
	gEng->UpdateAndRender( m_pCamera, this);
}

void S3DViewPanel::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();

	gEng->RDevice()->Resize( size.x, size.y );
}

void S3DViewPanel::PostRender()
{
	IRenderHelper* pRenderHelper = gEng->RenderHelper();

	XMMATRIX tm = XMMatrixIdentity();
	pRenderHelper->RenderGrid( tm, 5000, 100 );
}