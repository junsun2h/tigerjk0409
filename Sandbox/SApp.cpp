#include "SMainFrame.h"


class MyApp: public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
	SMainFrame *frame = new SMainFrame( _("SandBox"), wxPoint(50, 50), wxSize(800, 600) );
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}
