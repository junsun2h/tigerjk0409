#pragma once

#include "SPropertyGrid.h"

class SPropertyGrid;
class wxVectorProperty;

enum PROPERTY_GRID_TYPE
{
	ENTITY_PROPERTY,
};

class SPRopertyGridReflection
{
public:
	SPRopertyGridReflection();

	void	SetEntityReflection(SPropertyGrid* pPropertyGrid, IEntity* pEntity);
	void	OnEntityPropertyChanged(wxString& propertyName, wxVectorProperty* pPg);
	void	Clear();

private:
	IEntity*		m_pEntity;
	IEntityProxy*	m_pEntityProxy;
};