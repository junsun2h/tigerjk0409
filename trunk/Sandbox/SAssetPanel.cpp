#include "SAssetPanel.h"

#include "CFileUtility.h"

#include "wx/treectrl.h"
#include "wx/filename.h"
#include "wx/dnd.h"
#include "SPropertyPanel.h"
#include "SSelectionMgr.h"
#include "SMeshImportor.h"
#include "SDragAndDropState.h"

class SFileDragAndDrop : public wxFileDropTarget
{
public:
	bool SFileDragAndDrop::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
	{
		wchar_t path[MAX_PATH];
		GetCurrentDirectory( MAX_PATH, path);

		size_t nFiles = filenames.GetCount();

		bool bImported = false;
		for( UINT i=0; i < filenames.Count(); ++i)
		{
			wxFileName fn = filenames[i];
			if( fn.GetExt() == "mesh" )
			{
				SMESH_LOADER::SRAW_MESH rawMesh;
				SMESH_LOADER::LoadRawMesh(  fn.GetFullPath().char_str() , rawMesh);

				rawMesh.ChangeCoordsys( SMESH_LOADER::COODSYS_DIRECTX );

				wxString saveFile = wxString(path) + wxString("\\Data\\mesh\\") + fn.GetName() + wxString(".tmesh");
				SMESH_LOADER::SaveRawMeshToFile( &rawMesh , saveFile );

				bImported = true;
			}
			else if( fn.GetExt() == "actor" )
			{
				SMESH_LOADER::SRAW_ACTOR rawActor;
				SMESH_LOADER::LoadRawActor(  fn.GetFullPath().char_str() , rawActor);

				rawActor.ChangeCoordsys( SMESH_LOADER::COODSYS_DIRECTX );

				wxString saveFile = wxString(path) + wxString("\\Data\\actor\\") + fn.GetName() + wxString(".tac");
				SMESH_LOADER::SaveRawActorToFile( &rawActor , saveFile );

				bImported = true;
			}
		}

		if( bImported )
		{
			wxCommandEvent e;
			GLOBAL::AssetPanel()->OnReload(e);
		}

		return true;
	}
};


//-------------------------------------------------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(SAssetTreeCtrl, wxTreeCtrl)
BEGIN_EVENT_TABLE(SAssetTreeCtrl, wxTreeCtrl)
	EVT_TREE_SEL_CHANGED(ID_ASSET_TREECTRL, SAssetTreeCtrl::OnSelChanged)
	EVT_TREE_ITEM_MENU(ID_ASSET_TREECTRL, SAssetTreeCtrl::OnItemMenu)
	EVT_TREE_BEGIN_DRAG(wxID_ANY, SAssetTreeCtrl::OnBeginDrag)
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
{
	GetCurrentDirectory( MAX_PATH, m_Path);
}

void SAssetTreeCtrl::OnItemMenu(wxTreeEvent& event)
{
	wxTreeItemId seletedItem = event.GetItem();
	
	wxString strItem = GetItemText(seletedItem);

	if( strItem == "Root" ) 
		return;

	for( int i=0; i< NUM_RESOURCE_FILE_TYPE; ++i)
		if( strItem == strAssetType[i])
			return;

	wxMenu menu( GetItemText(seletedItem) );
	menu.Append(ID_ASSET_DELETE, wxT("&Delete"));

	PopupMenu(&menu, event.GetPoint());
}

void SAssetTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	SPropertyPanel*	pPropertyPanel	= GLOBAL::PropertyPanel();
	IAssetMgr*		pAssetMgr		= GLOBAL::Engine()->AssetMgr();
	ILoader*		pLoader			= GLOBAL::Engine()->Loader();

	wxTreeItemId seletedItem		= event.GetItem();
	wxString strItem	= GetItemText(seletedItem);

	eRESOURCE_FILE_TYPE fileType = GetAssetType(seletedItem);

	pPropertyPanel->Empty();
	GLOBAL::SelectionMgr()->Clear();

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
	else if( fileType == RESOURCE_FILE_MESH )
	{
		const CResourceBase* pMesh = pAssetMgr->GetResource(RESOURCE_MESH, strItem.c_str().AsChar() );

		if( pMesh == NULL )
		{
			wxString fullPath = wxString(m_Path) + wxString("\\Data\\mesh\\") + strItem + wxString(".tmesh");
			pMesh = pLoader->LoadForward( fullPath.char_str(), strItem.char_str(), RESOURCE_FILE_MESH );
		}

		pPropertyPanel->SetObject( (CResourceMesh*)pMesh );
	}
	else if( fileType == RESOURCE_FILE_ACTOR )
	{
		const CResourceBase* pActor = pAssetMgr->GetResource(RESOURCE_ACTOR, strItem.c_str().AsChar() );

		if( pActor == NULL )
		{
			wxString fullPath = wxString(m_Path) + wxString("\\Data\\actor\\") + strItem + wxString(".tac");
			pActor = pLoader->LoadForward( fullPath.char_str(), strItem.char_str(), RESOURCE_FILE_ACTOR );
		}

		pPropertyPanel->SetObject( (CResourceActor*)pActor );
	}
}


void SAssetTreeCtrl::OnDelete(wxCommandEvent& event)
{
	wxTreeItemId pSelectedID = GetSelection();

	if( !pSelectedID.IsOk() )
		return;

	IAssetMgr*			pAssetMgr	= GLOBAL::Engine()->AssetMgr();
	wxString			strItem		= GetItemText(pSelectedID);
	eRESOURCE_FILE_TYPE	fileType	= GetAssetType(pSelectedID);

	if( fileType == RESOURCE_FILE_TEXTURE )
	{
		wxString fullPath = wxString(m_Path) + wxString("\\Data\\texture\\") + strItem + wxString(".dds");
		if ( !DeleteFile( fullPath ) )
			assert(0);

		pAssetMgr->Remove( RESOURCE_TEXTURE, (char*)strItem.char_str() );
		Delete(pSelectedID);
	}
	else if( fileType == RESOURCE_FILE_MESH )
	{
		wxString fullPath = wxString(m_Path) + wxString("\\Data\\mesh\\") + strItem + wxString(".tmesh");
		if ( !DeleteFile( fullPath ) )
			assert(0);

		pAssetMgr->Remove( RESOURCE_MESH, (char*)strItem.char_str() );
		Delete(pSelectedID);
	}
	else if( fileType == RESOURCE_FILE_ACTOR )
	{
		wxString fullPath = wxString(m_Path) + wxString("\\Data\\actor\\") + strItem + wxString(".tac");
		if ( !DeleteFile( fullPath ) )
			assert(0);

		pAssetMgr->Remove( RESOURCE_ACTOR, (char*)strItem.char_str() );
		Delete(pSelectedID);
	}

	Reload();
}

void SAssetTreeCtrl::OnBeginDrag(wxTreeEvent& WXUNUSED(event))
{
	wxTreeItemId pSelectedID = GetSelection();

	if( pSelectedID.IsOk() )
	{
		eRESOURCE_FILE_TYPE	fileType	= GetAssetType(pSelectedID);
		
		if( fileType == RESOURCE_FILE_INVALID )
			return;

		SDragAndDragDesc desc;
		
		if( fileType == RESOURCE_FILE_ACTOR )		{	desc.type = DND_ACTOR;	}
		else if( fileType == RESOURCE_FILE_MESH )	{	desc.type = DND_MESH;	}
		else
			return;

		desc.strParam = GetItemText(pSelectedID);
		SDragAndDropState::Do(desc, this);
	}
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
		CFILE_UTILITY::CollectFileList( pathBuf, &fileList);
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
		CFILE_UTILITY::CollectFileList( pathBuf, &fileList);
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
		CFILE_UTILITY::CollectFileList( pathBuf, &fileList);
		for(UINT i=0; i< fileList.size(); ++i)
		{
			wxFileName fn = fileList[i];
			AppendItem( meshItem, fn.GetName() );
		}
	}
}

eRESOURCE_FILE_TYPE SAssetTreeCtrl::GetAssetType(wxTreeItemId seletedItem)
{
	wxString strItem = GetItemText(seletedItem);

	if( strItem == "Asset" )
		return RESOURCE_FILE_INVALID;

	wxString strParent = GetItemText( GetItemParent(seletedItem) );

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
	SetDropTarget( new SFileDragAndDrop );
}


void SAssetPanel::OnFilterChanged(wxCommandEvent& event)
{

}

void SAssetPanel::OnReload(wxCommandEvent& event)
{
	m_pTreeCtrl->Reload();
}
