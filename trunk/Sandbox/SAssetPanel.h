#pragma once


enum SASSET_WIDGET_ID
{
	ID_ASSET_FILTER_TEXTCTRL,
	ID_ASSET_TREECTRL,
	ID_ASSET_DELETE,
	ID_ASSET_RELOAD,
};



class SAssetTreeCtrl : public wxTreeCtrl
{
public:
	SAssetTreeCtrl(){}
	SAssetTreeCtrl(wxWindow *parent, const wxWindowID id);

	void	OnSelChanged(wxTreeEvent& event);
	void	OnItemMenu(wxTreeEvent& event);
	void	OnDelete(wxCommandEvent& event);
    void	OnBeginDrag(wxTreeEvent& event);

	void	Reload();

private:
	const CResourceBase*	LoadResource(wxTreeItemId seletedItem);
	eRESOURCE_FILE_TYPE		GetAssetType(wxTreeItemId seletedItem);

	wchar_t					m_Path[MAX_PATH];
		
	virtual void adw( wxTreeEvent& event ) { event.Skip(); }

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
