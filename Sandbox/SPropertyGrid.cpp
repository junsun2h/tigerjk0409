#include "wx/wx.h"


#include "CResource.h"
#include "CAABB.h"
#include "CCamera.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IAssetMgr.h"

#include "SGlobal.h"

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
	Append( new wxVector3Property( strProLocalPos, wxPG_LABEL, pEntity->GetLocalPos()) );
	Append( new wxVector3Property( strProLocalRot, wxPG_LABEL, localRot) );
	Append( new wxVector3Property( strProLocalScale, wxPG_LABEL, pEntity->GetLocalScale()) );
	Append( new wxVector3Property( strProWorldPos, wxPG_LABEL, pEntity->GetWorldPos()) );
	Append( new wxVector3Property( strProWorldRot, wxPG_LABEL, worldRot) );

	if( pEntity->GetWorldAABB()->IsValid() )
	{
		Append( new wxVector3Property( "BoundingBox Min (Local)", wxPG_LABEL, pEntity->GetLocalEntityAABB()->GetMin()) );
		Append( new wxVector3Property( "BoundingBox Max (Local)", wxPG_LABEL, pEntity->GetLocalEntityAABB()->GetMax()) );
		Append( new wxVector3Property( "BoundingBox Min (World)", wxPG_LABEL, pEntity->GetWorldAABB()->GetMin()) );
		Append( new wxVector3Property( "BoundingBox Max (World)", wxPG_LABEL, pEntity->GetWorldAABB()->GetMax()) );
	}
	else
	{
		Append( new wxStringProperty( "BoundingBox", wxPG_LABEL, "No volume") );
	}
}

void SPropertyGrid::Set( IEntityProxy* pEntityProxy )
{
	ClearProperties();

	eENTITY_PROXY_TYPE type = pEntityProxy->GetType();

	if( type == ENTITY_PROXY_CAMERA )
	{
		IEntityProxyCamera* pCamera = (IEntityProxyCamera*)pEntityProxy;
		const CCAMERA_DESC* pDesc = pCamera->GetDesc();

		Append(new wxFloatProperty( "Near Clip", wxPG_LABEL, pDesc->nearClip) );
		Append(new wxFloatProperty( "Far Clip", wxPG_LABEL, pDesc->farClip) );
		Append(new wxFloatProperty( "Fovy", wxPG_LABEL, pDesc->Fovy) );
		Append(new wxFloatProperty( "Aspect", wxPG_LABEL, pDesc->aspect) );
	}
	else if( type == ENTITY_PROXY_RENDER )
	{
		IEntityProxyRender* pRenderer = (IEntityProxyRender*)pEntityProxy;
		//TODO;
	}
}

void SPropertyGrid::Set(const CResourceTexture* pTexture)
{
	ClearProperties();

	if( pTexture->usage == TEXTURE_USAGE_RENDER_TAGET )
		Append( new wxStringProperty( "Usage", wxPG_LABEL, "Render target") );
	else
		Append( new wxStringProperty( "Usage", wxPG_LABEL, "texture resource") );

	Append( new wxStringProperty( "Pixel Format", wxPG_LABEL, PixelFormatToString(pTexture->Format)) );
	Append( new wxIntProperty( "Width", wxPG_LABEL, pTexture->Width) );
	Append( new wxIntProperty( "Height", wxPG_LABEL, pTexture->height) );
	Append( new wxIntProperty( "MipLevel", wxPG_LABEL, pTexture->MipLevels) );
}

void SPropertyGrid::Set(const CResourceMesh* pMesh)
{
	ClearProperties();

	Append( new wxVector3Property( "BoundingBox Min", wxPG_LABEL, pMesh->min) );
	Append( new wxVector3Property( "BoundingBox Max", wxPG_LABEL, pMesh->max) );
	Append( new wxIntProperty( "Geometry Count", wxPG_LABEL, pMesh->goemetries.size() ) );

	char buf[32];
	for(UINT i=0; i< pMesh->goemetries.size(); ++i)
	{
		const CResourceGeometry* pGeometry = pMesh->goemetries[i];

		_itoa(i, buf, 10);
		Append(new wxPropertyCategory( wxString("Geometry") + wxString(buf) ));
		Append(new wxStringProperty( wxString(buf) + "_Vertex Type", wxPG_LABEL, wxString( VERTEX_TYPE_STRING(pGeometry->eVertexType))) );
		Append(new wxIntProperty( wxString(buf) + "_Vertex Count", wxPG_LABEL, pGeometry->vertexCount) );

		if( pGeometry->eIndexType == INDEX_16BIT_TYPE )
			Append(new wxStringProperty(  wxString(buf) + "_Index Type", wxPG_LABEL, wxString("16Bit")) );
		else
			Append(new wxStringProperty(  wxString(buf) + "_Index Type", wxPG_LABEL, wxString("16Bit")) );

		Append(new wxIntProperty(  wxString(buf) + "_Polygon Count", wxPG_LABEL, pGeometry->primitiveCount) );
		Append(new wxFileProperty(  wxString(buf) + "_Default Material", wxString(pGeometry->mtrlName)) );
	}

	if( pMesh->skinBoneList.size() > 0 )
	{
		Append(new wxPropertyCategory( "BoneList" ));
		for( UINT i=0; i < pMesh->skinBoneList.size(); ++i)
		{
			_itoa(i, buf, 10);
			Append(new wxStringProperty( wxString("bone_") + buf, wxPG_LABEL, pMesh->skinBoneList[i] ) );
		}
	}
}

void SPropertyGrid::Set( const CResourceActor* pActor )
{
	ClearProperties();

	Append( new wxIntProperty( "Joint Num", wxPG_LABEL, pActor->jointList.size()) );
	Append( new wxIntProperty( "Motion Num", wxPG_LABEL, pActor->motionList.size()) );
	Append( new wxPropertyCategory( "Meshs" ) );

	char buf[32];
	for( UINT i =0; i< pActor->meshList.size(); ++i)
	{
		_itoa(i, buf, 10);
		Append(new wxStringProperty(  "Mesh" + wxString(buf), wxPG_LABEL, wxString( pActor->meshList[i]->name)) );
	}
}

void SPropertyGrid::Set( const CResourceMotion* pMotion )
{
	ClearProperties();

	Append( new wxIntProperty( "Frame Rate", wxPG_LABEL, pMotion->frameRate) );
	Append( new wxIntProperty( "Total Frame", wxPG_LABEL, pMotion->totalFrame) );
	Append( new wxFloatProperty( "Total Time(s)", wxPG_LABEL, pMotion->totalFrame / float(pMotion->frameRate) ) );
	Append( new wxIntProperty( "Joint Count", wxPG_LABEL, pMotion->jointList.size()) );
	Append( new wxIntProperty( "Sample Count", wxPG_LABEL, pMotion->jointList[0].keys.size() ) );
}

void SPropertyGrid::Set( const CJoint* pJoint )
{
	ClearProperties();

	Append( new wxVector3Property( "position", wxPG_LABEL, pJoint->pos) );
	CVector4 rot = CVector4( pJoint->rot.x, pJoint->rot.y, pJoint->rot.z, pJoint->rot.w);
	Append( new wxVector4Property( "rotation", wxPG_LABEL, rot) );
}

void SPropertyGrid::OnPropertyChanged(wxPropertyGridEvent& event)
{
	wxString propertyName = event.GetPropertyName();

	wxPGProperty* pg = GetProperty( propertyName );

	switch( m_CurrentPropertyType )
	{
	case ENTITY_PROPERTY:
		OnEntityPropertyChanged( propertyName, (wxVector3Property*)pg );
	}
}

void SPropertyGrid::ClearProperties()
{
	if( GetSelectedPage() != -1 )
		GetCurrentPage()->Clear();
	else
		AddPage();

	Clear();

	m_pEntity = NULL;
	m_pEntityProxy = NULL;
}

void SPropertyGrid::OnEntityPropertyChanged(wxString& propertyName, wxVector3Property* pPg)
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
