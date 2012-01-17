#include "SPropertyGrid.h"
#include "SPropertyGridReflection.h"
#include "SVectorProperty.h"



const wxString strProLocalPos = "Local Position";
const wxString strProLocalRot = "Local Rotation";
const wxString strProLocalScale = "Local Scale";
const wxString strProWorldPos = "World Position";
const wxString strProWorldRot = "World Rotation";

SPRopertyGridReflection::SPRopertyGridReflection()
	: m_pEntity(NULL)
	, m_pEntityProxy(NULL)
{

}


void SPRopertyGridReflection::SetEntityReflection(SPropertyGrid* pPropertyGrid, IEntity* pEntity)
{
	m_pEntity = pEntity;
	
	CVector3 localRot;
	XMMATRIX_UTIL::ToRollPitchYaw( localRot.x, localRot.y, localRot.z, pEntity->GetLocalTM());

	CVector3 worldRot;
	XMMATRIX_UTIL::ToRollPitchYaw( worldRot.x, worldRot.y, worldRot.z, pEntity->GetWorldTM());
	
	pPropertyGrid->AddPage();
	pPropertyGrid->Append( new wxVectorProperty( strProLocalPos, wxPG_LABEL, pEntity->GetLocalPos()) );
	pPropertyGrid->Append( new wxVectorProperty( strProLocalRot, wxPG_LABEL, localRot) );
	pPropertyGrid->Append( new wxVectorProperty( strProLocalScale, wxPG_LABEL, pEntity->GetLocalScale()) );
	pPropertyGrid->Append( new wxVectorProperty( strProWorldPos, wxPG_LABEL, pEntity->GetWorldPos()) );
	pPropertyGrid->Append( new wxVectorProperty( strProWorldRot, wxPG_LABEL, worldRot) );
}


void SPRopertyGridReflection::OnEntityPropertyChanged(wxString& propertyName, wxVectorProperty* pPg)
{
	if( propertyName == strProLocalPos )
	{
		m_pEntity->SetLocalPos( pPg->vector3 );
	}
	else if( propertyName == strProLocalRot )
	{
		CVector3 localRot;
		XMMATRIX rotTM = XMMATRIX_UTIL::ToMatrix( pPg->vector3.x, pPg->vector3.y, pPg->vector3.z);
		CQuat rot = XMQuaternionRotationMatrix(rotTM);

		m_pEntity->SetLocalRot( rot );
	}
	else if( propertyName == strProLocalScale )
	{
		m_pEntity->SetLocalScale( pPg->vector3 );
	}
	else if( propertyName == strProWorldPos )
	{
		m_pEntity->SetWorldPos( pPg->vector3 );
	}
	else if( propertyName == strProWorldRot ) 
	{
		CVector3 localRot;
		XMMATRIX rotTM = XMMATRIX_UTIL::ToMatrix( pPg->vector3.x, pPg->vector3.y, pPg->vector3.z);
		CQuat rot = XMQuaternionRotationMatrix(rotTM);

		m_pEntity->SetWorldRot( rot );
	}
}

void SPRopertyGridReflection::Clear()
{
	m_pEntity = NULL;
	m_pEntityProxy = NULL;
}
