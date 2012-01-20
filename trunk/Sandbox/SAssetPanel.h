#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/treectrl.h"
#include "STexturePopupWindow.h"



enum SASSET_WIDGET_ID
{
	ID_ASSET_FILTER_TEXTCTRL,
	ID_ASSET_TREECTRL,
	ID_ASSET_DELETE,
	ID_ASSET_RELOAD
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
	eRESOURCE_FILE_TYPE		GetAssetType();

	wxTreeItemId			m_SeletedItem;
	STexturePopupWindow*	m_pTexturePopupWindow;

	wchar_t					m_Path[MAX_PATH];

	DECLARE_DYNAMIC_CLASS(SAssetTreeCtrl)
	DECLARE_EVENT_TABLE()
};


class SAssetPanel: public wxPanel
{
public:
	SAssetPanel(wxWindow* parent);
	
	void			OnReload(wxCommandEvent& event);

private:
	void			OnFilterChanged(wxCommandEvent& event);

	SAssetTreeCtrl*			m_pTreeCtrl;
	
	DECLARE_EVENT_TABLE()
};
