#include "wx/wx.h"
#include "wx/aui/aui.h"
#include "SMainFrame.h"


class MyApp: public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
	SMainFrame* pMainFrame = new SMainFrame( _("SandBox"), wxPoint(50, 50), wxSize(1024, 768) );
	pMainFrame->Show(true);
	SetTopWindow(pMainFrame);
	return true;
}
