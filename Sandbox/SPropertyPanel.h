#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/treectrl.h"


class SCanvas;


enum PROPERTY_WIDGET_ID
{
	ID_PROPERTY_TREECTRL,
	ID_PROPERTY_GRID
};

class SPropertyGrid;

class SPropertyTreeCtrl : public wxTreeCtrl
{
public:
	SPropertyTreeCtrl(){}
	SPropertyTreeCtrl(wxWindow *parent, SPropertyGrid* pGrid, const wxWindowID id);

	void			OnSelChanged(wxTreeEvent& event);

	void			SetEntity(IEntity* pEntity);

private:
	wxTreeItemId	m_SeletedItem;
	IEntity*		m_pEntity;
	SPropertyGrid*	m_pGrid;

	DECLARE_DYNAMIC_CLASS(SPropertyTreeCtrl)
	DECLARE_EVENT_TABLE()
};





class SPropertyPanel : public wxPanel
{
	DECLARE_DYNAMIC_CLASS(SPropertyPanel)
	wxDECLARE_NO_COPY_CLASS(SPropertyPanel);

public:
	SPropertyPanel();
	SPropertyPanel(wxWindow* parent);

	void					SetObject( IEntity* pEntity );
	void					SetObject( const CResourceTexture* pResource );
	void					SetObject( const CResourceMesh* pResource );
	void					SetEmpty();

private:
	void					OrganizeInside();

	SCanvas*				m_pTextureCanvas;
	SPropertyGrid*			m_pGridMgr;
	SPropertyTreeCtrl*		m_pEntityTreeCtrl;

	DECLARE_EVENT_TABLE()
};