#include "EEntityProxyCamera.h"
#include "EEngine.h"


void EEntityProxyCamera::Init(IEntity* pEntity)
{
	m_pEntity = pEntity;
	m_FrameTransformChanged = false;

	// default setting
	SetProjParam(XM_PIDIV4, 800, 600, 1.0f, 1000.0f );
	SetViewDescFromWorldMatrix();
}

void EEntityProxyCamera::ProcessEvent( EntityEvent &event )
{
	if( event.type == E_EVENT_TRANSFORM_CHANGED )
	{
		m_FrameTransformChanged = g_Engine.GetCurrentFrame();
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
}

void EEntityProxyCamera::SetViewParam(CVector3& eyePos, CVector3& targetPos, CVector3& upVector)
{
	m_Desc.ViewTM = XMMatrixLookAtLH(eyePos.ToXMVEECTOR(), targetPos.ToXMVEECTOR(), upVector.ToXMVEECTOR() );

	if( m_pEntity != NULL)
		m_pEntity->SetLocalTM( XMMATRIX_UTIL::Inverse( NULL,  m_Desc.ViewTM ) );
	
	SetViewDescFromWorldMatrix();
}

void EEntityProxyCamera::SetViewDescFromWorldMatrix()
{
	XMMATRIX worldTM = m_pEntity->GetWorldTM();

	m_Desc.ViewTM =  XMMATRIX_UTIL::Inverse( NULL, worldTM );
	m_Desc.eyePos = CVector3( worldTM.r[3] );
	m_Desc.eyeDirection = CVector3( worldTM.r[2] );
	m_Desc.upVector = CVector3( worldTM.r[1] );
}