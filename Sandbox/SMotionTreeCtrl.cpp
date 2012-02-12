#include "wx/wx.h"
#include "wx/treectrl.h"
#include "wx/dnd.h"

#include "SPropertyGrid.h"
#include "SPropertyPanel.h"
#include "SDragAndDropState.h"
#include "SSelectionMgr.h"

#include "SMotionTreeCtrl.h"


class SMotionDragAndDrop : public wxTextDropTarget
{
public:
	SMotionDragAndDrop(SMotionTreeCtrl* pMotionTreeCtrl)
		: m_pMotionTreeCtrl(pMotionTreeCtrl){}

	bool OnDropText(wxCoord x, wxCoord y, const wxString& text)
	{
		m_pMotionTreeCtrl->OnDrop( wxPoint(x,y), text );
		return true;
	}

private:
	SMotionTreeCtrl*	m_pMotionTreeCtrl;
};


IMPLEMENT_DYNAMIC_CLASS(SMotionTreeCtrl, wxTreeCtrl)

BEGIN_EVENT_TABLE(SMotionTreeCtrl, wxTreeCtrl)
EVT_TREE_SEL_CHANGED(ID_PROPERTY_MOTION_TREECTRL, SMotionTreeCtrl::OnSelChanged)
END_EVENT_TABLE()



SMotionTreeCtrl::SMotionTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id)
	: wxTreeCtrl(parent, id,  wxDefaultPosition, wxSize(250, 100))
	, m_pGrid(pGrid)
	, m_pEntity(NULL)
{
	SetDropTarget( new SMotionDragAndDrop(this) );
}

void SMotionTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
	m_pGrid->Clear();
	if(  event.GetItem() == GetRootItem() )
	{
		m_pGrid->ClearProperties();
	}
	else
	{
		wxString strItem = GetItemText( event.GetItem() );
		for( UINT i=0 ; i <	m_pActor->motionList.size(); ++i )
		{
			if( strItem == m_pActor->motionList[i]->name)
			{
				m_pGrid->Set( m_pActor->motionList[i] );
				GLOBAL::SelectionMgr()->SelectMotion(strItem);
				break;
			}
		}
	}
}

void SMotionTreeCtrl::SetActor(const CResourceActor* pActor, IEntity* pEntity)
{
	DeleteAllItems();

	m_pActor = pActor;
	m_pEntity = pEntity;

	if( m_pActor == NULL)
		return;

	wxString name = wxString("Motions (") + m_pActor->name + ")";
	wxTreeItemId rootItem = AddRoot( name );

	for( UINT i=0 ; i < 	m_pActor->motionList.size(); ++i )
		AppendItem( rootItem, m_pActor->motionList[i]->name );
}

void SMotionTreeCtrl::OnDrop(wxPoint point, const wxString& text)
{
	SDragAndDragDesc desc;
	if( SDragAndDropState::Get(desc) == false )
		return;
	
	if( desc.type != DND_MOTION )
		return;

	for(UINT i=0; i < m_pActor->motionList.size(); ++i)
	{
		if( desc.strParam == m_pActor->motionList[i]->name )
			return;
	}

	IAssetMgr* pAssetMgr = GLOBAL::Engine()->AssetMgr();

	const CResourceMotion* pMotion = (const CResourceMotion*)pAssetMgr->GetResource( RESOURCE_MOTION, desc.strParam.char_str() );
	if( pMotion == NULL )
		assert(0);

	CResourceActor*	pUnSafeActor = (CResourceActor*)pAssetMgr->UnSafeGetResource(RESOURCE_ACTOR, m_pActor->RID);
	pUnSafeActor->motionList.push_back( pMotion );

	GLOBAL::PropertyPanel()->SetObject( m_pActor );
}
