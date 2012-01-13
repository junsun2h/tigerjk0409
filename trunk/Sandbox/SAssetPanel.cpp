#include "SAssetPanel.h"
#include "wx/treectrl.h"
#include "SWxWidgetID.h"

IMPLEMENT_DYNAMIC_CLASS(SAssetTreeCtrl, wxTreeCtrl)
BEGIN_EVENT_TABLE(SAssetTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_ASSET_TREECTRL, SAssetTreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_MENU(ID_ASSET_TREECTRL, SAssetTreeCtrl::OnItemMenu)
	EVT_MENU(ID_ASSET_DELETE, SAssetTreeCtrl::OnDelete)
END_EVENT_TABLE()


wxString strAssetType[] = 
{
	wxString("VERTEX"),
	wxString("INDEX"),
	wxString("GEOMETRY"),
	wxString("TEXTURE"),
	wxString("MESH"),
	wxString("LOD_MESH"),
	wxString("ACTOR"),
	wxString("MOTION"),
	wxString("MATERIAL")
};


SAssetTreeCtrl::SAssetTreeCtrl(wxWindow *parent, const wxWindowID id)
	: wxTreeCtrl(parent, id)
{
}

void SAssetTreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	m_SeletedItem = event.GetItem();

	wxMenu menu( GetItemText(m_SeletedItem) );
	menu.Append(ID_ASSET_DELETE, wxT("&Delete"));

	PopupMenu(&menu, event.GetPoint());
}


void SAssetTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	int ad = 0;
}

void SAssetTreeCtrl::OnDelete(wxCommandEvent& event)
{
	IAssetMgr* pAssetMgr =SGLOBAL::Engine()->AssetMgr();

	wxString parentText = GetItemText( GetItemParent( m_SeletedItem ) );
	std::string strSeletedItem =  GetItemText(m_SeletedItem).c_str();

	for( int i = RESOURCE_TEXTURE; i< RESOURCE_SHADER; ++i	)
	{
		if( strAssetType[i] == parentText )
		{
			pAssetMgr->Remove( RESOURCE_TYPE(i), strSeletedItem );
			break;
		}
	}
	GetItemText( m_SeletedItem );

}

void SAssetTreeCtrl::Reload()
{
	IAssetMgr* pAssetMgr =SGLOBAL::Engine()->AssetMgr();

	DeleteAllItems();
	wxTreeItemId rootItem = AddRoot( "Asset" );

	for( int i=RESOURCE_TEXTURE; i< RESOURCE_SHADER; ++i	)
	{
		const TYPE_RESOURCE_MAP* pResourceMap = pAssetMgr->GetResources( RESOURCE_TYPE(i) );

		int itemCount = pResourceMap->GetCount();

		if( itemCount > 0)
		{
			wxTreeItemId category = AppendItem( rootItem, strAssetType[i] );

			POSITION pos = pResourceMap->GetStartPosition();
			const TYPE_RESOURCE_MAP::CPair* itr = NULL;

			while (pos)
			{
				itr = pResourceMap->GetNext(pos);		
				AppendItem( category, itr->m_value->name );
			}
		}
	}
}


//---------------------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SAssetPanel, wxPanel)
	EVT_TEXT(ID_ASSET_FILTER_TEXTCTRL, SAssetPanel::OnFilterChanged)
END_EVENT_TABLE()


SAssetPanel::SAssetPanel(wxWindow* parent)
	:  wxPanel(parent, ID_PANEL_ASSET)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);

	wxTextCtrl* pTextBoxCtrl = new wxTextCtrl(this, ID_ASSET_FILTER_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
	pRootSizer->Add(pTextBoxCtrl, wxSizerFlags(0).Top().Border());

	m_pTreeCtrl = new SAssetTreeCtrl(this, ID_ASSET_TREECTRL);
	pRootSizer->Add(m_pTreeCtrl, wxSizerFlags(1).Center().Border().Expand());

	SetSizerAndFit(pRootSizer);
}


void SAssetPanel::OnFilterChanged(wxCommandEvent& event)
{

}

void SAssetPanel::Reload()
{
	m_pTreeCtrl->Reload();
}