#include "SMainFrame.h"
#include "S3DViewPanel.h"
#include "wx/aui/aui.h"


BEGIN_EVENT_TABLE(SMainFrame, wxFrame)
	EVT_MENU(ID_Quit,  SMainFrame::OnQuit)
	EVT_MENU(ID_About, SMainFrame::OnAbout)
END_EVENT_TABLE()


SMainFrame::SMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, -1, title, pos, size)
{
	wxAuiManager* auiManager = new wxAuiManager;
	auiManager->SetManagedWindow(this);

	SetMinSize(wxSize(800,600));

	//////////////////////////////////////////////////////////////////////////
	// Panel initializing
	S3DViewPanel* pViewPanel = new S3DViewPanel(this);
	auiManager->AddPane(pViewPanel, wxAuiPaneInfo().Name("3DViewPanel").Show().CenterPane().Layer(0).Position(0));

	auiManager->Update();

	pViewPanel->InitDevice();

	wxMenu *menuFile = new wxMenu;
	
	menuFile->Append( ID_About, _("&About...") );
	menuFile->AppendSeparator();
	menuFile->Append( ID_Quit, _("E&xit") );

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, _("&File") );

	SetMenuBar( menuBar );

	CreateStatusBar();
	SetStatusText( _("") );
}

SMainFrame::~SMainFrame()
{
	wxAuiManager* mgr = wxAuiManager::GetManager(this);
	mgr->UnInit();
	delete mgr;
}

void SMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

void SMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox( _("This is a wxWidgets Hello world sample"),
		_("About Hello World"),
		wxOK | wxICON_INFORMATION, this );
}
