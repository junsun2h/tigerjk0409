#include "SPropertyGrid.h"
#include "SPropertyPanel.h"
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



IMPLEMENT_DYNAMIC_CLASS(SPropertyGrid, wxPropertyGridManager)

BEGIN_EVENT_TABLE(SPropertyGrid, wxPropertyGridManager)
	EVT_PG_CHANGED(ID_PROPERTY_GRID, SPropertyGrid::OnPropertyChanged)
END_EVENT_TABLE()



SPropertyGrid::SPropertyGrid()
	: wxPropertyGridManager()
	, m_pEntity(NULL)
	, m_pEntityProxy(NULL)
{
}

SPropertyGrid::SPropertyGrid(wxWindow* parent)
	: wxPropertyGridManager(parent, ID_PROPERTY_GRID, wxDefaultPosition, wxSize(250, wxDefaultSize.y), wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxPG_DESCRIPTION)
{
}

void SPropertyGrid::Set( IEntity* pEntity )
{
	ClearProperties();

	m_CurrentPropertyType = ENTITY_PROPERTY;
	m_pEntity = pEntity;

	CVector3 localRot;
	XMMATRIX_UTIL::ToRollPitchYaw( localRot.x, localRot.y, localRot.z, pEntity->GetLocalTM());

	CVector3 worldRot;
	XMMATRIX_UTIL::ToRollPitchYaw( worldRot.x, worldRot.y, worldRot.z, pEntity->GetWorldTM());

	AddPage();
	Append( new wxVectorProperty( strProLocalPos, wxPG_LABEL, pEntity->GetLocalPos()) );
	Append( new wxVectorProperty( strProLocalRot, wxPG_LABEL, localRot) );
	Append( new wxVectorProperty( strProLocalScale, wxPG_LABEL, pEntity->GetLocalScale()) );
	Append( new wxVectorProperty( strProWorldPos, wxPG_LABEL, pEntity->GetWorldPos()) );
	Append( new wxVectorProperty( strProWorldRot, wxPG_LABEL, worldRot) );
}

void SPropertyGrid::Set( IEntityProxy* pEntityProxy )
{
}

void SPropertyGrid::Set(const CResourceTexture* pTexture)
{
	AddPage();

	if( pTexture->usage == TEXTURE_NORMAL )
		Append( new wxStringProperty( "Usage", wxPG_LABEL, "texture resource") );
	else
		Append( new wxStringProperty( "Usage", wxPG_LABEL, "Render target") );

	Append( new wxStringProperty( "Pixel Format", wxPG_LABEL, PixelFormatToString(pTexture->Format)) );
	Append( new wxIntProperty( "Width", wxPG_LABEL, pTexture->Width) );
	Append( new wxIntProperty( "Height", wxPG_LABEL, pTexture->height) );
	Append( new wxIntProperty( "MipLevel", wxPG_LABEL, pTexture->MipLevels) );

	Disable();
}

void SPropertyGrid::Set(const CResourceMesh* pMesh)
{
	AddPage();

	Append( new wxVectorProperty( "BoundingBox Min", wxPG_LABEL, pMesh->min) );
	Append( new wxVectorProperty( "BoundingBox Max", wxPG_LABEL, pMesh->max) );
	Append( new wxIntProperty( "Geometry Count", wxPG_LABEL, pMesh->geometryNum) );

	char buf[32];
	for(int i=0; i< pMesh->geometryNum; ++i)
	{
		const CResourceGeometry* pGeometry = (const CResourceGeometry*)GLOBAL::Engine()->AssetMgr()->GetResource( RESOURCE_GEOMETRY, pMesh->goemetries[i] );

		_itoa(i, buf, 32);
		Append(new wxPropertyCategory( wxString("Geometry") + wxString(buf) ));
		Append(new wxStringProperty(  "Vertex Type", wxPG_LABEL, wxString( VERTEX_TYPE_STRING(pGeometry->eVertexType))) );
		Append(new wxIntProperty(  "Vertex Count", wxPG_LABEL, pGeometry->vertexCount) );

		if( pGeometry->eIndexType == INDEX_16BIT_TYPE )
			Append(new wxStringProperty(  "Index Type", wxPG_LABEL, wxString("16Bit")) );
		else
			Append(new wxStringProperty(  "Index Type", wxPG_LABEL, wxString("16Bit")) );

		Append(new wxIntProperty(  "Polygon Count", wxPG_LABEL, pGeometry->primitiveCount) );
		Append(new wxFileProperty(  "Default Material", wxString(pGeometry->mtrlName)) );
	}

	Disable();
}

void SPropertyGrid::OnPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();

	wxPGProperty* pg = GetProperty( propertyName );

	switch( m_CurrentPropertyType )
	{
	case ENTITY_PROPERTY:
		OnEntityPropertyChanged( propertyName, (wxVectorProperty*)pg );
	}
}

void SPropertyGrid::RefreshAllProperties()
{

}

void SPropertyGrid::ClearProperties()
{
	Clear();
	m_pEntity = NULL;
	m_pEntityProxy = NULL;
}

void SPropertyGrid::OnEntityPropertyChanged(wxString& propertyName, wxVectorProperty* pPg)
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
