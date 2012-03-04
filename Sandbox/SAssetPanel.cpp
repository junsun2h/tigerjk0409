#include "wx/wx.h"
#include "wx/treectrl.h"
#include "wx/filename.h"
#include "wx/dnd.h"
#include "wx/popupwin.h"

#include "CResource.h"
#include "CFileUtility.h"
#include "CGeometryConstructor.h"

#include "IAssetMgr.h"
#include "ILoader.h"

#include "SGlobal.h"
#include "STexturePopupWindow.h"
#include "SPropertyPanel.h"
#include "SSelectionMgr.h"
#include "SRawResourceLoader.h"
#include "SDragAndDropState.h"

#include "SAssetPanel.h"


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
				SRAW_FILE_LOADER::SRAW_MESH rawMesh;
				SRAW_FILE_LOADER::LoadRawMesh(  fn.GetFullPath().char_str() , rawMesh);

				rawMesh.ChangeCoordsys( SRAW_FILE_LOADER::COODSYS_DIRECTX );

				wxString saveFile = wxString(path) + wxString("\\Data\\mesh\\") + fn.GetName() + wxString(".tmesh");
				SRAW_FILE_LOADER::SaveRawMeshToFile( &rawMesh , saveFile );

				bImported = true;
			}
			else if( fn.GetExt() == "actor" )
			{
				SRAW_FILE_LOADER::SRAW_ACTOR rawActor;
				SRAW_FILE_LOADER::LoadRawActor(  fn.GetFullPath().char_str() , rawActor);

				rawActor.ChangeCoordsys( SRAW_FILE_LOADER::COODSYS_DIRECTX );

				wxString saveFile = wxString(path) + wxString("\\Data\\actor\\") + fn.GetName() + wxString(".tac");
				SRAW_FILE_LOADER::SaveRawActorToFile( &rawActor , saveFile );

				bImported = true;
			}
			else if( fn.GetExt() == "motion" )
			{
				SRAW_FILE_LOADER::SRAW_MOTION rawMotion;
				SRAW_FILE_LOADER::LoadRawMotion(  fn.GetFullPath().char_str() , rawMotion);

				rawMotion.ChangeCoordsys( SRAW_FILE_LOADER::COODSYS_DIRECTX );

				wxString saveFile = wxString(path) + wxString("\\Data\\motion\\") + fn.GetName() + wxString(".tmo");
				SRAW_FILE_LOADER::SaveRawMotionToFile( &rawMotion , saveFile );

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
	SPropertyPanel*	pPropertyPanel	=  GLOBAL::PropertyPanel();

	pPropertyPanel->Empty();
	GLOBAL::SelectionMgr()->Clear();

	const CResourceBase* pResource = LoadResource( event.GetItem() );

	if( pResource != NULL )
		pPropertyPanel->SetObject( pResource );
}

const CResourceBase* SAssetTreeCtrl::LoadResource(wxTreeItemId seletedItem)
{
	IAssetMgr* pAssetMgr = GLOBAL::Engine()->AssetMgr();
	eRESOURCE_FILE_TYPE fileType = GetAssetType(seletedItem);

	if( fileType == RESOURCE_FILE_INVALID)
		return NULL;

	wxString strItem = GetItemText(seletedItem);
	return GLOBAL::Engine()->Loader()->Load( strItem.char_str(), fileType, true );
}

void SAssetTreeCtrl::OnDelete(wxCommandEvent& event)
{
	wxTreeItemId pSelectedID = GetSelection();

	if( !pSelectedID.IsOk() )
		return;

	IAssetMgr*			pAssetMgr	= GLOBAL::Engine()->AssetMgr();
	wxString			strItem		= GetItemText(pSelectedID);
	eRESOURCE_FILE_TYPE	fileType	= GetAssetType(pSelectedID);
	wxString fullPath;

	if( fileType == RESOURCE_FILE_TEXTURE )
	{
		fullPath = wxString(m_Path) + wxString("\\Data\\texture\\") + strItem + wxString(".dds");
		pAssetMgr->Remove( RESOURCE_TEXTURE, (char*)strItem.char_str() );
	}
	else if( fileType == RESOURCE_FILE_MESH )
	{
		fullPath = wxString(m_Path) + wxString("\\Data\\mesh\\") + strItem + wxString(".tmesh");
		pAssetMgr->Remove( RESOURCE_MESH, (char*)strItem.char_str() );
	}
	else if( fileType == RESOURCE_FILE_ACTOR )
	{
		fullPath = wxString(m_Path) + wxString("\\Data\\actor\\") + strItem + wxString(".tac");
		pAssetMgr->Remove( RESOURCE_ACTOR, (char*)strItem.char_str() );
	}
	else if( fileType == RESOURCE_FILE_MOTION )
	{
		fullPath = wxString(m_Path) + wxString("\\Data\\motion\\") + strItem + wxString(".tmo");
		pAssetMgr->Remove( RESOURCE_MOTION, (char*)strItem.char_str() );
	}
	else if( fileType == RESOURCE_FILE_MATERIAL )
	{
		fullPath = wxString(m_Path) + wxString("\\Data\\material\\") + strItem + wxString(".xml");
		pAssetMgr->Remove( RESOURCE_MATERIAL, (char*)strItem.char_str() );
	}

	if ( !DeleteFile( fullPath ) )
		assert(0);

	Delete(pSelectedID);
	Reload();
}

void SAssetTreeCtrl::OnBeginDrag(wxTreeEvent& event)
{
	int flags =0;
	wxTreeItemId seletedItem = HitTest( event.GetPoint(), flags);
	if (flags & (wxTREE_HITTEST_ABOVE | wxTREE_HITTEST_NOWHERE | wxTREE_HITTEST_BELOW | wxTREE_HITTEST_TOLEFT | wxTREE_HITTEST_TORIGHT))
		return;
	
	if( seletedItem.IsOk() )
	{
		eRESOURCE_FILE_TYPE	fileType	= GetAssetType(seletedItem);
		
		if( fileType == RESOURCE_FILE_INVALID )
			return;

		LoadResource(seletedItem);

		SDragAndDragDesc desc;
		
		if( fileType == RESOURCE_FILE_ACTOR )		{	desc.type = DND_ACTOR;	}
		else if( fileType == RESOURCE_FILE_MESH )	{	desc.type = DND_MESH;	}
		else if( fileType == RESOURCE_FILE_MOTION )	{	desc.type = DND_MOTION;	}
		else
			return;

		desc.strParam = GetItemText(seletedItem);
		SDragAndDropState::Do(desc, this);
	}
}

void SAssetTreeCtrl::Reload()
{
	DeleteAllItems();

	IAssetMgr*		pAssetMgr	= GLOBAL::Engine()->AssetMgr();
	wxTreeItemId	rootItem	= AddRoot( "Asset" );
	wchar_t			pathBuf[MAX_PATH];

	for( UINT i =0 ; i < NUM_RESOURCE_FILE_TYPE; ++i)
	{
		wxTreeItemId parent = AppendItem( rootItem, strAssetType[i] );

		if( i == RESOURCE_FILE_TEXTURE )		swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\Texture");
		else if( i == RESOURCE_FILE_ACTOR )		swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\actor");
		else if( i == RESOURCE_FILE_MESH )		swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\mesh");
		else if( i == RESOURCE_FILE_MOTION )	swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\motion");
		else if( i == RESOURCE_FILE_MATERIAL )	swprintf_s( pathBuf, MAX_PATH, _T("%s%s"), m_Path, L"\\Data\\material");
		else
			assert(0);

		CSTRING_LIST fileList;
		CFILE_UTILITY::CollectFileList( pathBuf, &fileList);
		for(UINT iFile=0; iFile< fileList.size(); ++iFile)
		{
			wxFileName fn = fileList[iFile];
			AppendItem( parent, fn.GetName() );
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
