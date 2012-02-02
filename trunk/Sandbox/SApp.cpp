#include "SMainFrame.h"


class MyApp: public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
#ifdef _DEBUG
	AllocConsole();
	//	freopen_s(NULL, "CONOUT$", "w", stdout);
#endif
	SMainFrame* pMainFrame = new SMainFrame( _("SandBox"), wxPoint(50, 50), wxSize(1024, 768) );
	pMainFrame->Show(true);
	SetTopWindow(pMainFrame);
	return true;
}
