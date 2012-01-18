#include "SPropertyGrid.h"
#include "SPropertyGridReflection.h"
#include "SVectorProperty.h"
#include "STextureFormatString.h"



const wxString strProLocalPos = "Local Position";
const wxString strProLocalRot = "Local Rotation";
const wxString strProLocalScale = "Local Scale";
const wxString strProWorldPos = "World Position";
const wxString strProWorldRot = "World Rotation";

const wxString strTextureUsage = "Usage";
const wxString strTextureFormat = "Pixel Format";
const wxString strTextureWidth = "Width";
const wxString strTextureHeight = "Height";
const wxString strTextureMipLevels = "MipLevel";


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

void SPRopertyGridReflection::SetTextureReflection(SPropertyGrid* pPropertyGrid, const CResourceTexture* pTexture)
{
	pPropertyGrid->AddPage();

	if( pTexture->usage == TEXTURE_NORMAL )
		pPropertyGrid->Append( new wxStringProperty( "Usage", wxPG_LABEL, "texture resource") );
	else
		pPropertyGrid->Append( new wxStringProperty( "Usage", wxPG_LABEL, "Render target") );

	pPropertyGrid->Append( new wxStringProperty( "Pixel Format", wxPG_LABEL, PixelFormatToString(pTexture->Format)) );
	pPropertyGrid->Append( new wxIntProperty( "Width", wxPG_LABEL, pTexture->Width) );
	pPropertyGrid->Append( new wxIntProperty( "Height", wxPG_LABEL, pTexture->height) );
	pPropertyGrid->Append( new wxIntProperty( "MipLevel", wxPG_LABEL, pTexture->MipLevels) );

	pPropertyGrid->Disable();
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
