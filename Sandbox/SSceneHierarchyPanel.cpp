#include "SSceneHierarchyPanel.h"
#include "wx/treectrl.h"
#include "SWxWidgetID.h"

BEGIN_EVENT_TABLE(SSceneHierarchyPanel, wxPanel)
	EVT_TEXT(ID_SCENE_FILTER_TEXTCTRL, SSceneHierarchyPanel::OnFilterChanged)
END_EVENT_TABLE()


SSceneHierarchyPanel::SSceneHierarchyPanel(wxWindow* parent)
	:  wxPanel(parent, ID_PANEL_SCENE_HIERARCHY)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* pTextBoxCtrl = new wxTextCtrl(this, ID_SCENE_FILTER_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
	pRootSizer->Add(pTextBoxCtrl, wxSizerFlags(0).Top().Border());

	wxTreeCtrl* pTreeCtrl = new wxTreeCtrl(this, ID_SCENE_TREECTRL, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
	pRootSizer->Add(pTreeCtrl, wxSizerFlags(1).Center().Border().Expand());
/*	{
		wxIcon icons[ICONID_NUM];
		icons[ICONID_ACTOR] = wxIcon("res/actor.png", wxBITMAP_TYPE_PNG);
		icons[ICONID_ACTORNODE] = wxIcon("res/actornode.png", wxBITMAP_TYPE_PNG);

		int width = icons[0].GetWidth();
		int height = icons[0].GetHeight();
		wxImageList* pImageList = new wxImageList(width, height, true);
		for( size_t i = 0; i < _countof(icons); ++i )
		{
			_ASSERT(icons[i].IsOk());
			pImageList->Add(icons[i]);
		}
		pTreeCtrl->AssignImageList(pImageList);
	}*/

	SetSizerAndFit(pRootSizer);
}


void SSceneHierarchyPanel::OnFilterChanged(wxCommandEvent& event)
{

}