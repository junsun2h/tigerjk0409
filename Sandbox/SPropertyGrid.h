#pragma once

#include "SGlobal.h"
#include "wx/propgrid/manager.h"
#include "wx/propgrid/advprops.h"
#include "SPropertyGridReflection.h"


class SPropertyGrid : public wxPropertyGridManager
{
	DECLARE_DYNAMIC_CLASS(SPropertyGrid)
	wxDECLARE_NO_COPY_CLASS(SPropertyGrid);

public:
	SPropertyGrid();
	SPropertyGrid(wxWindow* parent);
	~SPropertyGrid(){}

	void			SetProperty( IEntity* pEntity );
	void			SetProperty( IEntityProxy* pEntityProxy );
	void			ClearProperties();

private:
	void			OnPropertyChanged(wxPropertyGridEvent& event);
	void			RefreshAllProperties();

	PROPERTY_GRID_TYPE			m_CurrentPropertyType;
	SPRopertyGridReflection		m_PropertyReflection;

	DECLARE_EVENT_TABLE()
};

