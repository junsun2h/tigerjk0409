#include "CMathType.h"
#include "CCamera.h"
#include "CUnitPool.h"

#include "IEntity.h"
#include "IRDevice.h"
#include "IEntityProxy.h"

#include "EGlobal.h"
#include "EEntityProxyCamera.h"



void EEntityProxyCamera::Init(IEntity* pEntity)
{
	m_pEntity = pEntity;
	m_FrustrumChanged = false;

	// default setting
	SetProjParam(XM_PIDIV4, 800, 600, 1.0f, 1000.0f );
	SetViewDescFromWorldMatrix();
}

void EEntityProxyCamera::ProcessEvent( EntityEvent &event )
{
	if( event.type == E_EVENT_TRANSFORM_CHANGED )
	{
		m_FrustrumChanged = GLOBAL::Engine()->GetCurrentFrame();
		SetViewDescFromWorldMatrix();
	}
}

void EEntityProxyCamera::SetProjParam(float fovy, int width, int height, float nearPlane, float farPlane)
{
	m_Desc.farClip = farPlane;
	m_Desc.nearClip = nearPlane;
	m_Desc.aspect = (float)width / (float)height;
	m_Desc.Fovy = fovy;
	
	m_Desc.ProjTM = XMMatrixPerspectiveFovLH( m_Desc.Fovy, m_Desc.aspect, nearPlane, farPlane);

	m_FrustrumChanged = GLOBAL::Engine()->GetCurrentFrame();
}

void EEntityProxyCamera::SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector)
{
	m_Desc.ViewTM = XMMatrixLookAtLH(eyePos.ToXMVEECTOR(), targetPos.ToXMVEECTOR(), upVector.ToXMVEECTOR() );

	if( m_pEntity != NULL)
		m_pEntity->SetLocalTM( XMMATRIX_UTIL::Inverse( NULL,  m_Desc.ViewTM ) );
	
	XMMATRIX worldTM = m_pEntity->GetWorldTM();

	m_Desc.eyePos = worldTM.r[3];
	m_Desc.eyeDirection = worldTM.r[2];
	m_Desc.upVector = worldTM.r[1];

	m_FrustrumChanged = GLOBAL::Engine()->GetCurrentFrame();
}

void EEntityProxyCamera::SetViewDescFromWorldMatrix()
{
	XMMATRIX worldTM = m_pEntity->GetWorldTM();

	m_Desc.ViewTM =  XMMATRIX_UTIL::Inverse( NULL, worldTM );
	m_Desc.eyePos = worldTM.r[3];
	m_Desc.eyeDirection = worldTM.r[2];
	m_Desc.upVector = worldTM.r[1];
}

void EEntityProxyCamera::GetPickRayFromScreen(UINT screenX, UINT screenY, CVector3& origin, CVector3& direction)
{
	RDeviceDesc deviceDesc = GLOBAL::Engine()->RDevice()->GetDeviceSetting();

	// Compute the vector of the pick ray in screen space
	CVector3 v;
	v.x = ( ( ( 2.0f * screenX ) / deviceDesc.width ) - 1 ) / m_Desc.ProjTM._11;
	v.y = -( ( ( 2.0f * screenY ) / deviceDesc.height ) - 1 ) / m_Desc.ProjTM._22;
	v.z = m_Desc.nearClip;

	// Get the inverse view matrix
	XMMATRIX InvView = XMMATRIX_UTIL::Inverse( NULL, m_Desc.ViewTM);

	// Transform the screen space pick ray into 3D space
	direction.x = v.x * InvView._11 + v.y * InvView._21 + v.z * InvView._31;
	direction.y = v.x * InvView._12 + v.y * InvView._22 + v.z * InvView._32;
	direction.z = v.x * InvView._13 + v.y * InvView._23 + v.z * InvView._33;

	direction = CVector3::Normalize( direction);

	origin.x = InvView._41;
	origin.y = InvView._42;
	origin.z = InvView._43;
}
