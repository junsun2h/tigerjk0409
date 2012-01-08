#include "SAssetPanel.h"
#include "wx/treectrl.h"
#include "SWxWidgetID.h"

BEGIN_EVENT_TABLE(SAssetPanel, wxPanel)
	EVT_TEXT(ID_ASSET_FILTER_TEXTCTRL, SAssetPanel::OnFilterChanged)
END_EVENT_TABLE()


SAssetPanel::SAssetPanel(wxWindow* parent)
	:  wxPanel(parent, ID_PANEL_ASSET)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* pTextBoxCtrl = new wxTextCtrl(this, ID_ASSET_FILTER_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
	pRootSizer->Add(pTextBoxCtrl, wxSizerFlags(0).Top().Border());

	wxTreeCtrl* pTreeCtrl = new wxTreeCtrl(this, ID_ASSET_TREECTRL, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
	pRootSizer->Add(pTreeCtrl, wxSizerFlags(1).Center().Border().Expand());
	
	SetSizerAndFit(pRootSizer);
}


void SAssetPanel::OnFilterChanged(wxCommandEvent& event)
{

}