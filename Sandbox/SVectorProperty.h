#pragma once

#include "SGlobal.h"
#include "wx/wx.h"
#include "wx/propgrid/manager.h"
#include "wx/propgrid/advprops.h"


WX_PG_DECLARE_VARIANT_DATA(CVector3)

class wxVectorProperty : public wxPGProperty
{
	WX_PG_DECLARE_PROPERTY_CLASS(wxVectorProperty)
public:

	wxVectorProperty( const wxString& label = wxPG_LABEL,
		const wxString& name = wxPG_LABEL,
		const CVector3& value = CVector3() );
	virtual ~wxVectorProperty();

	virtual wxVariant ChildChanged( wxVariant& thisValue,
		int childIndex,
		wxVariant& childValue ) const;
	virtual void RefreshChildren();

	CVector3	vector3;
};	