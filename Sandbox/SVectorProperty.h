#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/propgrid/manager.h"
#include "wx/propgrid/advprops.h"


WX_PG_DECLARE_VARIANT_DATA(CVector3)

class wxVector3Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxVector3Property)
public:

	wxVector3Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CVector3& value = CVector3() );
	virtual ~wxVector3Property();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

	CVector3	vector3;
};	

//-------------------------------------------------------------------------------------------------
WX_PG_DECLARE_VARIANT_DATA(CVector4)
class wxVector4Property : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxVector4Property)
public:

	wxVector4Property( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CVector4& value = CVector4() );
	virtual ~wxVector4Property();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

	CVector4	vector4;
};	