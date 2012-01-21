#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/propgrid/manager.h"
#include "wx/propgrid/advprops.h"

class wxVectorProperty;

enum ePROPERTY_GRID_TYPE
{
	ENTITY_PROPERTY
};


class SPropertyGrid : public wxPropertyGridManager
{
	DECLARE_DYNAMIC_CLASS(SPropertyGrid)
	wxDECLARE_NO_COPY_CLASS(SPropertyGrid);

public:
	SPropertyGrid();
	SPropertyGrid(wxWindow* parent);
	~SPropertyGrid(){}

	void			Set( IEntity* pEntity );
	void			Set( IEntityProxy* pEntityProxy );
	void			Set( const CResourceTexture* pTexture );
	void			Set( const CResourceMesh* pMesh );

	void			ClearProperties();

private:
	void			OnPropertyChanged(wxPropertyGridEvent& event);

	void			OnEntityPropertyChanged(wxString& propertyName, wxVectorProperty* pPg);
	void			RefreshAllProperties();

	ePROPERTY_GRID_TYPE			m_CurrentPropertyType;

	IEntity*					m_pEntity;
	IEntityProxy*				m_pEntityProxy;

	DECLARE_EVENT_TABLE()
};

