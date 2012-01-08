#include "SPropertyPanel.h"
#include "SWxWidgetID.h"


IMPLEMENT_DYNAMIC_CLASS(SPropertyPanel, wxPanel)
BEGIN_EVENT_TABLE(SPropertyPanel, wxPanel)
END_EVENT_TABLE()


SPropertyPanel::SPropertyPanel()
{

}

SPropertyPanel::SPropertyPanel(wxWindow* parent) 
	: wxPanel(parent, ID_PANEL_PROPERTY)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);

	m_pGridMgr = new wxPropertyGridManager(this);
	
	pRootSizer->Add(m_pGridMgr, wxSizerFlags(1).Center().Expand());

	SetSizerAndFit(pRootSizer);
}