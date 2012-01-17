#include "SPropertyGrid.h"
#include "SPropertyPanel.h"
#include "SVectorProperty.h"


IMPLEMENT_DYNAMIC_CLASS(SPropertyGrid, wxPropertyGridManager)

BEGIN_EVENT_TABLE(SPropertyGrid, wxPropertyGridManager)
	EVT_PG_CHANGED(ID_PROPERTY_GRID, SPropertyGrid::OnPropertyChanged)
END_EVENT_TABLE()



SPropertyGrid::SPropertyGrid()
	: wxPropertyGridManager()
{
}

SPropertyGrid::SPropertyGrid(wxWindow* parent)
	: wxPropertyGridManager(parent, ID_PROPERTY_GRID, wxDefaultPosition, wxSize(250, wxDefaultSize.y), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DESCRIPTION)
{
}

void SPropertyGrid::SetProperty( IEntity* pEntity )
{
	ClearProperties();

	m_CurrentPropertyType = ENTITY_PROPERTY;
	m_PropertyReflection.SetEntityReflection( this, pEntity );
}

void SPropertyGrid::SetProperty( IEntityProxy* pEntityProxy )
{
}

void SPropertyGrid::OnPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();

	wxPGProperty* pg = GetProperty( propertyName );

	switch( m_CurrentPropertyType )
	{
	case ENTITY_PROPERTY:
		m_PropertyReflection.OnEntityPropertyChanged( propertyName, (wxVectorProperty*)pg );
	}
}

void SPropertyGrid::RefreshAllProperties()
{

}

void SPropertyGrid::ClearProperties()
{
	Clear();
	m_PropertyReflection.Clear();
}
