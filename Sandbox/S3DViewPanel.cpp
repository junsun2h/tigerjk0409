#include "S3DViewPanel.h"
#include "IEngine.h"

IEngine* gEng = NULL;


BEGIN_EVENT_TABLE(S3DViewPanel, wxPanel)
	EVT_IDLE(S3DViewPanel::OnIdle)
	EVT_SIZE(S3DViewPanel::OnSize)
END_EVENT_TABLE()



S3DViewPanel::S3DViewPanel(wxWindow* parent)
	:  wxPanel(parent)
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

	return true;
}

void S3DViewPanel::OnIdle(wxIdleEvent& event)
{
	gEng->RDevice()->Render(0);
}

void S3DViewPanel::OnSize(wxSizeEvent& event)
{
	wxSize size = event.GetSize();

	gEng->RDevice()->Resize( size.x, size.y );
}