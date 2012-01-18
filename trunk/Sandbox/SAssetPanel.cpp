#include "SAssetPanel.h"
#include "wx/treectrl.h"
#include "wx/filename.h"
#include "SPropertyPanel.h"




IMPLEMENT_DYNAMIC_CLASS(SAssetTreeCtrl, wxTreeCtrl)
BEGIN_EVENT_TABLE(SAssetTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_ASSET_TREECTRL, SAssetTreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_MENU(ID_ASSET_TREECTRL, SAssetTreeCtrl::OnItemMenu)
	EVT_MENU(ID_ASSET_DELETE, SAssetTreeCtrl::OnDelete)
END_EVENT_TABLE()


wxString strAssetType[] = 
{
	wxString("TEXTURE"),
	wxString("ACTOR"),
	wxString("MESH"),
	wxString("MOTION"),
	wxString("MATERIAL")
};


SAssetTreeCtrl::SAssetTreeCtrl(wxWindow *parent, const wxWindowID id)
	: wxTreeCtrl(parent, id)
	, m_pTexturePopupWindow(NULL)
{
	GetCurrentDirectory( MAX_PATH, m_Path);
}

void SAssetTreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	m_SeletedItem = event.GetItem();
	
	wxString strItem = GetItemText(m_SeletedItem);

	if( strItem == "Root" ) 
		return;

	for( int i=0; i< NUM_RESOURCE_FILE_TYPE; ++i)
		if( strItem == strAssetType[i])
			return;

	wxMenu menu( GetItemText(m_SeletedItem) );
	menu.Append(ID_ASSET_DELETE, wxT("&Delete"));

	PopupMenu(&menu, event.GetPoint());
}


void SAssetTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	SPropertyPanel*	pPropertyPanel	= GLOBAL::PropertyPanel();
	IAssetMgr*		pAssetMgr		= GLOBAL::Engine()->AssetMgr();
	ILoader*		pLoader			= GLOBAL::Engine()->Loader();

	m_SeletedItem		= event.GetItem();
	wxString strItem	= GetItemText(m_SeletedItem);

	RESOURCE_FILE_TYPE fileType = GetAssetType();

	pPropertyPanel->SetEmpty();

	if( fileType == RESOURCE_FILE_TEXTURE )
	{
		const CResourceBase* pTexture = pAssetMgr->GetResource(RESOURCE_TEXTURE, strItem.c_str().AsChar() );

		if( pTexture == NULL )
		{
			wxString fullPath = wxString(m_Path) + wxString("\\Data\\texture\\") + strItem + wxString(".dds");
			pTexture = pLoader->LoadForward( fullPath.char_str(), strItem.char_str(), RESOURCE_FILE_TEXTURE );
		}

		pPropertyPanel->SetObject( (CResourceTexture*)pTexture );
	}
}


void SAssetTreeCtrl::OnDelete(wxCommandEvent& event)
{
	IAssetMgr*			pAssetMgr	= GLOBAL::Engine()->AssetMgr();
	wxString			strItem		= GetItemText(m_SeletedItem);
	RESOURCE_FILE_TYPE	fileType	= GetAssetType();

	if( fileType == RESOURCE_FILE_TEXTURE )
	{
		wxString fullPath = wxString(m_Path) + wxString("\\Data\\texture\\") + strItem + wxString(".dds");
		DeleteFile( fullPath );

		pAssetMgr->Remove( RESOURCE_TEXTURE, (char*)strItem.char_str() );
		Delete(m_SeletedItem);
	}

	Reload();
}


void SAssetTreeCtrl::Reload()
{
	DeleteAllItems();

	IAssetMgr*		pAssetMgr	= GLOBAL::Engine()->AssetMgr();
	wxTreeItemId	rootItem	= AddRoot( "Asset" );
	wchar_t			pathBuf[MAX_PATH];
	
	{ // texture
		wxTreeItemId textureItem = AppendItem( rootItem, strAssetType[RESOURCE_FILE_TEXTURE] );
		swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\Texture");

		CSTRING_LIST fileList;
		GLOBAL::Engine()->FileUtility()->CollectFileList( pathBuf, &fileList);
		for(UINT i=0; i< fileList.size(); ++i)
		{
			wxFileName fn = fileList[i];
			AppendItem( textureItem, fn.GetName() );
		}
	}
	{ // actor
		wxTreeItemId actorItem = AppendItem( rootItem, strAssetType[RESOURCE_FILE_ACTOR] );
		swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\actor");

		CSTRING_LIST fileList;
		GLOBAL::Engine()->FileUtility()->CollectFileList( pathBuf, &fileList);
		for(UINT i=0; i< fileList.size(); ++i)
		{
			wxFileName fn = fileList[i];
			AppendItem( actorItem, fn.GetName() );
		}
	}
	{ // mesh
		wxTreeItemId meshItem = AppendItem( rootItem, strAssetType[RESOURCE_FILE_MESH] );
		swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\mesh");

		CSTRING_LIST fileList;
		GLOBAL::Engine()->FileUtility()->CollectFileList( pathBuf, &fileList);
		for(UINT i=0; i< fileList.size(); ++i)
		{
			wxFileName fn = fileList[i];
			AppendItem( meshItem, fn.GetName() );
		}
	}
}

RESOURCE_FILE_TYPE SAssetTreeCtrl::GetAssetType()
{
	wxString strItem = GetItemText(m_SeletedItem);

	if( strItem == "Asset" )
		return RESOURCE_FILE_INVALID;

	wxString strParent = GetItemText( GetItemParent(m_SeletedItem) );

	if( strParent == strAssetType[RESOURCE_FILE_TEXTURE] )		return RESOURCE_FILE_TEXTURE;
	else if( strParent == strAssetType[RESOURCE_FILE_ACTOR] )	return RESOURCE_FILE_ACTOR;
	else if( strParent == strAssetType[RESOURCE_FILE_MESH] )	return RESOURCE_FILE_MESH;
	else if( strParent == strAssetType[RESOURCE_FILE_MOTION] )	return RESOURCE_FILE_MOTION;
	else if( strParent == strAssetType[RESOURCE_FILE_MATERIAL] )return RESOURCE_FILE_MATERIAL;
	else
		return RESOURCE_FILE_INVALID;
}




//---------------------------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SAssetPanel, wxPanel)
	EVT_TEXT(ID_ASSET_FILTER_TEXTCTRL, SAssetPanel::OnFilterChanged)
	EVT_BUTTON(ID_ASSET_RELOAD, SAssetPanel::OnReload)
END_EVENT_TABLE()


SAssetPanel::SAssetPanel(wxWindow* parent)
	: wxPanel(parent, ID_PANEL_ASSET)
{
	wxBoxSizer* pRootSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxButton* pbtnReload = new wxButton( this, ID_ASSET_RELOAD, wxT("Reload"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
		pRootSizer->Add( pbtnReload, 0, wxALL, 5 );

		wxTextCtrl* pTextBoxCtrl = new wxTextCtrl(this, ID_ASSET_FILTER_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(200, wxDefaultSize.y));
		pRootSizer->Add(pTextBoxCtrl, wxSizerFlags(0).Top().Border());

		m_pTreeCtrl = new SAssetTreeCtrl(this, ID_ASSET_TREECTRL);
		pRootSizer->Add(m_pTreeCtrl, wxSizerFlags(1).Center().Border().Expand());
	}
	SetSizerAndFit(pRootSizer);
}


void SAssetPanel::OnFilterChanged(wxCommandEvent& event)
{

}

void SAssetPanel::OnReload(wxCommandEvent& event)
{
	m_pTreeCtrl->Reload();
}
