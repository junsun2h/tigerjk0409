#pragma once

#include "SGlobal.h"
#include "wx/treectrl.h"


enum SASSET_WIDGET_ID
{
	ID_ASSET_FILTER_TEXTCTRL,
	ID_ASSET_TREECTRL,
	ID_ASSET_DELETE,
};



class SAssetTreeCtrl : public wxTreeCtrl
{
public:
	SAssetTreeCtrl(){}
	SAssetTreeCtrl(wxWindow *parent, const wxWindowID id);

	void	OnSelChanged(wxTreeEvent& event);
	void	OnItemMenu(wxTreeEvent& event);
	void	OnDelete(wxCommandEvent& event);

	void	Reload();

private:
	wxTreeItemId	m_SeletedItem;

	DECLARE_DYNAMIC_CLASS(SAssetTreeCtrl)
	DECLARE_EVENT_TABLE()
};


class SAssetPanel: public wxPanel
{
public:
	SAssetPanel(wxWindow* parent);

	void	Reload();

private:
	void	OnFilterChanged(wxCommandEvent& event);

	SAssetTreeCtrl*			m_pTreeCtrl;

	DECLARE_EVENT_TABLE()
};
