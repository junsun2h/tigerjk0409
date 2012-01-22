#include "EAABB.h"
#include <float.h>

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


void EAABB::AddAABB(IAABB* pAABB)
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
