#include <float.h>

#include "IAABB.h"
#include "EAABB.h"

EAABB::EAABB()
	: m_Min(FLT_MAX, FLT_MAX, FLT_MAX)
	, m_Max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
	, m_IsValid(false)
{
}

void EAABB::AddAABB(XMMATRIX& worldTM, CVector3& min, CVector3& max)
{
	CVector3 vec[8] = { 
		min,
		CVector3( max.x, min.y, min.z),
		CVector3( max.x, max.y, min.z),
		CVector3( min.x, max.y, min.z),

		CVector3( min.x, min.y, max.z),
		CVector3( max.x, min.y, max.z),
		max,
		CVector3( min.x, max.y, max.z)};

	for (int i=0; i< 8; ++i)
	{
		vec[i] = CVector3::Transform( vec[i], worldTM);
		m_Min = CVector3::Min(m_Min, vec[i]);
		m_Max = CVector3::Max(m_Max, vec[i]);
	}

	m_IsValid = true;
}

void EAABB::AddAABB(CVector3& min, CVector3& max)
{
	m_Min = CVector3::Min( m_Min, min );
	m_Max = CVector3::Max( m_Max, max );

	m_IsValid = true;
}


void EAABB::AddAABB(const IAABB* pAABB)
{
	if( pAABB->IsValid() == false )
		return;

	m_Min = CVector3::Min( m_Min, pAABB->GetMin() );
	m_Max = CVector3::Max( m_Max, pAABB->GetMax() );

	m_IsValid = true;
}

void EAABB::GetAABBPoints(CVector3 vPoints[], const XMMATRIX& tm)
{
	// This function simply converts the center and extents of an AABB into 8 points
	static const CVector3 vExtentsMap[] = 
	{ 
		CVector3(1.0f, 1.0f, -1.0f), 
		CVector3(-1.0f, 1.0f, -1.0f), 
		CVector3(1.0f, -1.0f, -1.0f), 
		CVector3(-1.0f, -1.0f, -1.0f), 
		CVector3(1.0f, 1.0f, 1.0f), 
		CVector3(-1.0f, 1.0f, 1.0f), 
		CVector3(1.0f, -1.0f, 1.0f), 
		CVector3(-1.0f, -1.0f, 1.0f) 
	};

	CVector3 vSceneCenter = GetCenter();
	CVector3 vSceneExtents = GetExtent();

	for( int index = 0; index < 8; ++index ) 
	{
		vPoints[index] = vExtentsMap[index] * vSceneExtents + vSceneCenter;
		vPoints[index] = CVector3::Transform( vPoints[index], tm); 
	}
}

void EAABB::Reset()
{
	m_Min = CVector3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_Max = CVector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	m_IsValid = false;
}

bool EAABB::IsValid() const
{
	if( !m_IsValid ||  m_Min == m_Max)
		return false;

	return true;
}

XMMATRIX EAABB::CalculateAABBCoordinate() const
{
	// AABB center will be (0,0,0) in AABBCoordinate
	XMMATRIX AABBCoordinate = XMMatrixIdentity();
	CVector3 center = GetCenter();

	AABBCoordinate.r[3].x = -center.x;
	AABBCoordinate.r[3].y = -center.y;
	AABBCoordinate.r[3].z = -center.z;
	return AABBCoordinate;
}

bool EAABB::IsPointInBox(const CVector3 &InP, XMMATRIX* pWorld ) const
{
	if( !IsValid() )
		return false;

	CVector3 P = InP;
	if( pWorld != NULL)
	{
		XMMATRIX invTM = XMMATRIX_UTIL::Inverse( NULL, *pWorld);
		 P = CVector3::Transform( InP, invTM);
	}

	if ( ( P.x > m_Min.x && P.x < m_Max.x ) &&
		 ( P.y > m_Min.y && P.y < m_Max.y ) &&
		 ( P.z > m_Min.z && P.z < m_Max.z ) )
		return true;

	return false;
}

bool EAABB::IsSphereInBox( const CVector3 &InP, float fRadius, XMMATRIX* pWorld ) const
{
	if( !IsValid() )
		return false;

	float fDist;
	float fDistSq = 0;

	CVector3 P;
	if( pWorld != NULL)
	{
		XMMATRIX temp = XMMATRIX_UTIL::Inverse( NULL, *pWorld);
		temp = XMMatrixMultiply( temp, CalculateAABBCoordinate() );

		P = CVector3::Transform( InP, temp);
	}
	else
	{
		P = CVector3::Transform( InP, CalculateAABBCoordinate() );
	}

	CVector3 extent = GetExtent();

	// Add distance squared from sphere center point to box for each axis
	for ( int i = 0; i < 3; i++ )
	{
		if ( fabs(P[i]) > extent[i] )
		{
			fDist = fabs(P[i]) - extent[i];
			fDistSq += fDist*fDist;
		}
	}
	return ( fDistSq <= fRadius*fRadius );
}


bool EAABB::IsLineInBox( const CVector3& L1, const CVector3& L2 , XMMATRIX* pWorld  ) const
{	
	if( !IsValid() )
		return false;

	CVector3 LB1, LB2;
	XMMATRIX AABBCoordinate;
	if( pWorld != NULL)
	{
		AABBCoordinate = XMMATRIX_UTIL::Inverse( NULL, *pWorld);
		AABBCoordinate = XMMatrixMultiply( AABBCoordinate, CalculateAABBCoordinate() );
	}
	else
	{
		AABBCoordinate = CalculateAABBCoordinate();
	}

	// Put line in box space
	LB1 = CVector3::Transform( L1, AABBCoordinate);
	LB2 = CVector3::Transform( L2, AABBCoordinate);

	// Get line midpoint and extent
	CVector3 LMid = (LB1 + LB2) * 0.5f; 
	CVector3 L = (LB1 - LMid);
	CVector3 LExt = CVector3( fabs(L.x), fabs(L.y), fabs(L.z) );
	CVector3 extent = GetExtent();

	// Use Separating Axis Test
	// Separation vector from box center to line center is LMid, since the line is in box space
	if ( fabs( LMid.x ) > extent.x + LExt.x ) return false;
	if ( fabs( LMid.y ) > extent.y + LExt.y ) return false;
	if ( fabs( LMid.z ) > extent.z + LExt.z ) return false;
	// Crossproducts of line and each axis
	if ( fabs( LMid.y * L.z - LMid.z * L.y)  >  (extent.y * LExt.z + extent.z * LExt.y) ) return false;
	if ( fabs( LMid.x * L.z - LMid.z * L.x)  >  (extent.x * LExt.z + extent.z * LExt.x) ) return false;
	if ( fabs( LMid.x * L.y - LMid.y * L.x)  >  (extent.x * LExt.y + extent.y * LExt.x) ) return false;
	// No separating axis, the line intersects
	return true;
}
