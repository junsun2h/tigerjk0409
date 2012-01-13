#include "SMainFrame.h"
#include "SWxWidgetID.h"


namespace SGLOBAL
{
	SMainFrame*				g_MainFrame = NULL;
	IEngine*				g_Eng = NULL;

	IEngine*				Engine()		{ return g_Eng; }
	SAssetPanel*			AssetPanel()	{ return wxDynamicCast( g_MainFrame->FindWindow(ID_PANEL_ASSET), SAssetPanel); }
	SPropertyPanel*			PropertyPanel()	{ return wxDynamicCast( g_MainFrame->FindWindow(ID_PANEL_PROPERTY), SPropertyPanel); }
	SSceneHierarchyPanel*	SceneHierarchyPanel() { return wxDynamicCast( g_MainFrame->FindWindow(ID_PANEL_SCENE_HIERARCHY), SSceneHierarchyPanel); }
	SMainFrame*				MainFrame()		{ return g_MainFrame; }
}


class MyApp: public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
	SGLOBAL::g_MainFrame = new SMainFrame( _("SandBox"), wxPoint(50, 50), wxSize(1024, 768) );
	SGLOBAL::g_MainFrame->Show(true);
	SetTopWindow(SGLOBAL::g_MainFrame);
	return true;
}
