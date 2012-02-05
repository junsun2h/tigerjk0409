#include "SVectorProperty.h"


// See propgridsample.h for wxVector3f class

WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(CVector3);

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector3Property,wxPGProperty,
CVector3,const CVector3&,TextCtrl)


wxVector3Property::wxVector3Property( const wxString& label,
	const wxString& name,
	const CVector3& value )
	: wxPGProperty(label,name)
{
	vector3 = value;

	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("Z"),wxPG_LABEL,value.z) );
}

wxVector3Property::~wxVector3Property() { }

void wxVector3Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const CVector3& vector = CVector3RefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
	Item(2)->SetValue( vector.z );
	vector3 = vector;
}

wxVariant wxVector3Property::ChildChanged( wxVariant& thisValue,
	int childIndex,
	wxVariant& childValue ) const
{
	CVector3 vector;
	vector << thisValue;
	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	case 2: vector.z = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}

//-------------------------------------------------------------------------------------------------


WX_PG_IMPLEMENT_VARIANT_DATA_DUMMY_EQ(CVector4);

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxVector4Property,wxPGProperty,
CVector4,const CVector4&,TextCtrl)


wxVector4Property::wxVector4Property( const wxString& label,
const wxString& name,
const CVector4& value )
	: wxPGProperty(label,name)
{
	vector4 = value;

	SetValue( WXVARIANT(value) );
	AddPrivateChild( new wxFloatProperty(wxT("X"),wxPG_LABEL,value.x) );
	AddPrivateChild( new wxFloatProperty(wxT("Y"),wxPG_LABEL,value.y) );
	AddPrivateChild( new wxFloatProperty(wxT("Z"),wxPG_LABEL,value.z) );
	AddPrivateChild( new wxFloatProperty(wxT("W"),wxPG_LABEL,value.w) );
}

wxVector4Property::~wxVector4Property() { }

void wxVector4Property::RefreshChildren()
{
	if ( !GetChildCount() ) return;
	const CVector4& vector = CVector4RefFromVariant(m_value);
	Item(0)->SetValue( vector.x );
	Item(1)->SetValue( vector.y );
	Item(2)->SetValue( vector.z );
	Item(3)->SetValue( vector.w );
	vector4 = vector;
}

wxVariant wxVector4Property::ChildChanged( wxVariant& thisValue,
	int childIndex,
	wxVariant& childValue ) const
{
	CVector4 vector;
	vector << thisValue;
	switch ( childIndex )
	{
	case 0: vector.x = childValue.GetDouble(); break;
	case 1: vector.y = childValue.GetDouble(); break;
	case 2: vector.z = childValue.GetDouble(); break;
	case 3: vector.w = childValue.GetDouble(); break;
	}
	wxVariant newVariant;
	newVariant << vector;
	return newVariant;
}