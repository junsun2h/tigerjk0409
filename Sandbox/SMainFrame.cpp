#include "SMainFrame.h"
#include "S3DViewPanel.h"
#include "SSceneHierarchyPanel.h"
#include "SAssetPanel.h"
#include "SMainMenuBar.h"
#include "SMainToolBar.h"
#include "SPropertyPanel.h"
#include "SMainDragAndDrop.h"


namespace GLOBAL
{
	extern SAssetPanel*	g_AssetPanel;
	extern SPropertyPanel* g_PropertyPanel;
	extern SSceneHierarchyPanel* g_SceneHierarchyPanel;
};


BEGIN_EVENT_TABLE(SMainFrame, wxFrame)
	EVT_MENU(wxID_NEW, SMainFrame::OnNew)
	EVT_MENU(wxID_SAVE, SMainFrame::OnSave)
	EVT_MENU(wxID_SAVEAS, SMainFrame::OnSaveAs)
	EVT_MENU(wxID_EXIT, SMainFrame::OnQuit)

	EVT_MENU(ID_MENU_VIEW_PROPERTY, SMainFrame::OnPanelCreate)
	EVT_MENU(ID_MENU_VIEW_SCENE_HIERARCHY, SMainFrame::OnPanelCreate)
	EVT_MENU(ID_MENU_VIEW_ASSET, SMainFrame::OnPanelCreate)
	EVT_MENU(ID_MENU_DEFAULT_LAYOUT, SMainFrame::OnDefaultLayout)
	EVT_MENU(ID_MENU_LOAD_SAVED_LAYOUT, SMainFrame::OnLoadSavedLayout)
	EVT_MENU(ID_MENU_SAVE_LAYOUT, SMainFrame::OnSaveLayout)

	EVT_AUI_PANE_CLOSE(SMainFrame::OnPanelClose)
END_EVENT_TABLE()



const wxString g_strSceneHierarchy = "SceneHierarchyPanel";
const wxString g_strAsset = "AssetPanel";
const wxString g_strProperty = "PropertyPanel";



SMainFrame::SMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, -1, title, pos, size)
{
	wxInitAllImageHandlers();

	CreateStatusBar();

	SMainToolBar* pToolBar = new SMainToolBar(this, ID_MAIN_TOOLBAR);
	SetToolBar(pToolBar);

	InitLayout();

	SMainMenuBar* pMainMenu = new SMainMenuBar;

	SetMenuBar( pMainMenu );
	pMainMenu->UpdateMenuBarCheckStatus(this);
	
	SetDropTarget( new SMainDragAndDrop );
	SetStatusText( _("") );
}

SMainFrame::~SMainFrame()
{
	wxAuiManager* mgr = wxAuiManager::GetManager(this);
	mgr->UnInit();
	delete mgr;
}

void SMainFrame::InitLayout()
{
	wxAuiManager* auiManager = new wxAuiManager;
	auiManager->SetManagedWindow(this);

	SetMinSize(wxSize(1024, 768));
	
	S3DViewPanel* pViewPanel = new S3DViewPanel(this);
	auiManager->AddPane(pViewPanel, wxAuiPaneInfo().Name("3DViewPanel").Show().CenterPane().Layer(0).Position(0));

	GLOBAL::g_SceneHierarchyPanel = new SSceneHierarchyPanel(this);
	auiManager->AddPane(GLOBAL::g_SceneHierarchyPanel, wxAuiPaneInfo().Name(g_strSceneHierarchy).Show().PinButton().Caption("Scene Hierarchy").Right().Layer(2).Position(1).FloatingSize(250, 500));

	GLOBAL::g_AssetPanel = new SAssetPanel(this);
	auiManager->AddPane(GLOBAL::g_AssetPanel, wxAuiPaneInfo().Name(g_strAsset).Show().PinButton().Caption("Asset").Right().Layer(2).Position(1).FloatingSize(250, 500));

	GLOBAL::g_PropertyPanel = new SPropertyPanel(this);
	auiManager->AddPane(GLOBAL::g_PropertyPanel, wxAuiPaneInfo().Name(g_strProperty).Show().PinButton().Caption("Properties").Right().Layer(1).Position(0).FloatingSize(250, 500));

	auiManager->Update();
	m_LayoutDefault = auiManager->SavePerspective();
}

void SMainFrame::OnNew(wxCommandEvent& event)
{

}

void SMainFrame::OnSave(wxCommandEvent& event)
{

}

void SMainFrame::OnSaveAs(wxCommandEvent& event)
{

}

void SMainFrame::OnQuit(wxCommandEvent& event)
{
	Close();
}

void SMainFrame::OnPanelClose(wxAuiManagerEvent& event)
{
	const wxAuiPaneInfo* info = event.GetPane();
	_ASSERT(info);

	wxString name = info->name;

	wxMenuBar* menuBar = GetMenuBar();
	_ASSERT(menuBar);

	if(name == g_strSceneHierarchy) menuBar->Check(ID_MENU_VIEW_SCENE_HIERARCHY, false);
	else if(name == g_strAsset) menuBar->Check(ID_MENU_VIEW_ASSET, false);
	else if(name == g_strProperty) menuBar->Check(ID_MENU_VIEW_PROPERTY, false);
};

void SMainFrame::OnPanelCreate(wxCommandEvent& event)
{
	wxAuiManager* mgr = wxAuiManager::GetManager(this);

	switch( event.GetId() )
	{
		case ID_MENU_VIEW_PROPERTY:			mgr->GetPane(g_strProperty).Show( event.IsChecked() );	break;
		case ID_MENU_VIEW_SCENE_HIERARCHY:	mgr->GetPane(g_strSceneHierarchy).Show( event.IsChecked() );	break;
		case ID_MENU_VIEW_ASSET: 			mgr->GetPane(g_strAsset).Show( event.IsChecked() );	break;
	}
	mgr->Update();
}

void SMainFrame::OnDefaultLayout(wxCommandEvent& event)
{
	wxAuiManager::GetManager(this)->LoadPerspective(m_LayoutDefault);
}

void SMainFrame::OnLoadSavedLayout(wxCommandEvent& event)
{
	wxString savedLayout = wxAuiManager::GetManager(this)->SavePerspective();
	char buff[1024] = {0,};
	GetCurrentDirectoryA( _MAX_DIR, buff );
	wxString strDir(buff);
	wxString strFullPath = strDir + "\\config.ini"; 
	GetPrivateProfileStringA("SANDBOX", "LAYOUT", "", buff, sizeof(buff), strFullPath.char_str() );
	
	wxAuiManager::GetManager(this)->LoadPerspective(buff);

	SMainMenuBar* pMainMenuBar = wxDynamicCast( GetMenuBar(), SMainMenuBar );
	pMainMenuBar->UpdateMenuBarCheckStatus(this);
}

void SMainFrame::OnSaveLayout(wxCommandEvent& event)
{
	wxString savedLayout = wxAuiManager::GetManager(this)->SavePerspective();
	wchar_t buff[_MAX_DIR] = {0,};
	GetCurrentDirectory( _MAX_DIR, buff );
	wxString strDir(buff);
	wxString strFullPath = strDir + "\\config.ini"; 
	WritePrivateProfileStringA("SANDBOX", "LAYOUT", savedLayout.char_str(), strFullPath.char_str() );
}

