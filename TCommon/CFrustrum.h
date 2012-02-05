#pragma once

#include "CDefine.h"
#include "CPlane.h"
#include "CCamera.h"

#include "IEntityProxy.h"

struct FRUSTRUM_DESC
{
	CVector3 Origin;            // Origin of the frustum (and projection).
	CVector4 Orientation;       // Unit quaternion representing rotation.

	float RightSlope;           // Positive X slope (X/Z).
	float LeftSlope;            // Negative X slope.
	float TopSlope;             // Positive Y slope (Y/Z).
	float BottomSlope;          // Negative Y slope.
	float Near, Far;            // Z of the near plane and far plane.
};

inline void ComputeFrustumFromProjection( FRUSTRUM_DESC* pOut, XMMATRIX& projection )
{
	// Corners of the projection frustum in homogeneous space.
	static CVector4 HomogenousPoints[6] =
	{
		CVector4( 1.0f,  0.0f, 1.0f, 1.0f ),   // right (at far plane)
		CVector4( -1.0f,  0.0f, 1.0f, 1.0f ),   // left

		CVector4(  0.0f,  1.0f, 1.0f, 1.0f ),   // top
		CVector4(  0.0f, -1.0f, 1.0f, 1.0f ),   // bottom

		CVector4( 0.0f, 0.0f, 0.0f, 1.0f ),     // near
		CVector4( 0.0f, 0.0f, 1.0f, 1.0f )      // far
	};

	XMMATRIX projInv = XMMATRIX_UTIL::Inverse( NULL, projection );

	// Compute the frustum corners in world space.
	CVector4 Points[6];

	for( int i = 0; i < 6; i++ )
	{
		// Transform point.
		Points[i] = CVector4::Transform(  HomogenousPoints[i] , projInv );
	}

	pOut->Origin = CVector3( 0.0f, 0.0f, 0.0f );
	pOut->Orientation = CVector4( 0.0f, 0.0f, 0.0f, 1.0f );

	pOut->RightSlope = Points[0].x / Points[0].z;
	pOut->LeftSlope = Points[1].x / Points[1].z;
	pOut->TopSlope = Points[2].y / Points[2].z;
	pOut->BottomSlope = Points[3].y / Points[3].z;

	pOut->Near = Points[4].z/Points[4].w;
	pOut->Far = Points[5].z/Points[5].w;

	return;
}



class CFrustumCull
{
public:
	void ConstructFrustum(IEntityProxyCamera* pCamera);

	bool CheckPoint(const CVector3& point);
	bool CheckCube(const CVector3& center, float radius);
	bool CheckSphere(const CVector3& center, float radius);
	bool CheckRectangle(const CVector3& center, const CVector3& size);

private:
	CPlane		m_planes[6];
};



inline void CFrustumCull::ConstructFrustum(IEntityProxyCamera* pCamera)
{
	const CCAMERA_DESC desc= pCamera->GetDesc();

	float screenDepth = desc.farClip;
	XMMATRIX projTM = desc.ProjTM;
	XMMATRIX viewTM = desc.ViewTM;

	// Calculate the minimum Z distance in the frustum.
	float zMinimum = -projTM._43 / projTM._33;
	float r = screenDepth / (screenDepth - zMinimum);
	projTM._33 = r;
	projTM._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	XMMATRIX matrix = projTM * viewTM;

	// Calculate near plane of frustum.
	m_planes[0].a = matrix._14 + matrix._13;
	m_planes[0].b = matrix._24 + matrix._23;
	m_planes[0].c = matrix._34 + matrix._33;
	m_planes[0].d = matrix._44 + matrix._43;
	m_planes[0].Normalize();

	// Calculate far plane of frustum.
	m_planes[1].a = matrix._14 - matrix._13; 
	m_planes[1].b = matrix._24 - matrix._23;
	m_planes[1].c = matrix._34 - matrix._33;
	m_planes[1].d = matrix._44 - matrix._43;
	m_planes[1].Normalize();

	// Calculate left plane of frustum.
	m_planes[2].a = matrix._14 + matrix._11; 
	m_planes[2].b = matrix._24 + matrix._21;
	m_planes[2].c = matrix._34 + matrix._31;
	m_planes[2].d = matrix._44 + matrix._41;
	m_planes[2].Normalize();

	// Calculate right plane of frustum.
	m_planes[3].a = matrix._14 - matrix._11; 
	m_planes[3].b = matrix._24 - matrix._21;
	m_planes[3].c = matrix._34 - matrix._31;
	m_planes[3].d = matrix._44 - matrix._41;
	m_planes[3].Normalize();

	// Calculate top plane of frustum.
	m_planes[4].a = matrix._14 - matrix._12; 
	m_planes[4].b = matrix._24 - matrix._22;
	m_planes[4].c = matrix._34 - matrix._32;
	m_planes[4].d = matrix._44 - matrix._42;
	m_planes[4].Normalize();

	// Calculate bottom plane of frustum.
	m_planes[5].a = matrix._14 + matrix._12;
	m_planes[5].b = matrix._24 + matrix._22;
	m_planes[5].c = matrix._34 + matrix._32;
	m_planes[5].d = matrix._44 + matrix._42;
	m_planes[5].Normalize();
}

inline bool CFrustumCull::CheckPoint(const CVector3& point)
{
	for(int i=0; i<6; i++) 
	{
		if( m_planes[i].Dot(point) < 0.0f)
			return false;
	}

	return true;
}

inline bool CFrustumCull::CheckCube(const CVector3& center, float radius)
{
	for(int i=0; i<6; i++) 
	{
		if( m_planes[i].Dot( center - radius) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x + radius, center.y - radius, center.z - radius) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x - radius, center.y + radius, center.z - radius) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x + radius, center.y + radius, center.z - radius) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x - radius, center.y - radius, center.z + radius) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x + radius, center.y - radius, center.z + radius) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x - radius, center.y + radius, center.z + radius) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( center + radius ) >= 0.0f)
			continue;

		return false;
	}

	return true;
}

inline bool CFrustumCull::CheckSphere(const CVector3& center, float radius)
{
	for(int i=0; i<6; i++) 
	{
		if( m_planes[i].Dot( center ) < -radius)
			return false;
	}

	return true;
}

inline bool CFrustumCull::CheckRectangle(const CVector3& center, const CVector3& size)
{
	for(int i=0; i<6; i++)
	{
		if( m_planes[i].Dot( center - size) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x + size.x, center.y - size.y, center.z - size.z) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x - size.x, center.y + size.y, center.z - size.z) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x - size.x, center.y - size.y, center.z + size.z) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x + size.x, center.y + size.y, center.z - size.z) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x + size.x, center.y - size.y, center.z + size.z) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( CVector3( center.x - size.x, center.y + size.y, center.z + size.z) ) >= 0.0f)
			continue;

		if( m_planes[i].Dot( center + size) >= 0.0f)
			continue;

		return false;
	}

	return true;
}