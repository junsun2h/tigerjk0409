#pragma once

#include "CDefine.h"
#include "CResource.h"
#include "CMathType.h"
#include "CGeometryConstructor.h"

enum ePICK_LEVEL
{
	PICK_BOUNDINGBOX,
	PICK_POLYGON,
};

enum eCOLLISION_CHECK_OPTION
{
	LINE_COLLISION
};

struct CCollisionBase
{
	virtual	eCOLLISION_CHECK_OPTION Type() = 0; 

	ePICK_LEVEL	pickLevel;

	CCollisionBase()
		: pickLevel(PICK_POLYGON)
	{
	}
};

struct CCollisionLine : CCollisionBase
{
	eCOLLISION_CHECK_OPTION Type() override { return LINE_COLLISION; };

	CVector3	from;
	CVector3	to;
};


namespace COLLISION_UTIL
{
	//--------------------------------------------------------------------------------------
	// Given a ray origin (orig) and direction (dir), and three vertices of a triangle, this
	// function returns TRUE and the interpolated texture coordinates if the ray intersects 
	// the triangle
	// brought from d3dx example
	//--------------------------------------------------------------------------------------
	inline bool	IntersectTriangle( const CVector3& orig, const CVector3& dir,
							CVector3& v0, CVector3& v1, CVector3& v2,
							FLOAT* pt = NULL, FLOAT* pu = NULL, FLOAT* pv = NULL )
	{
		float t,u,v;

		// Find vectors for two edges sharing vert0
		CVector3 edge1 = v1 - v0;
		CVector3 edge2 = v2 - v0;

		// Begin calculating determinant - also used to calculate U parameter
		CVector3 pvec = CVector3::Cross(dir, edge2);

		// If determinant is near zero, ray lies in plane of triangle
		FLOAT det = CVector3::Dot( edge1, pvec );

		CVector3 tvec;
		if( det > 0 )
		{
			tvec = orig - v0;
		}
		else
		{
			tvec = v0 - orig;
			det = -det;
		}

		if( det < 0.0001f )
			return FALSE;

		// Calculate U parameter and test bounds
		u = CVector3::Dot( tvec, pvec );
		if( u < 0.0f || u > det )
			return FALSE;

		// Prepare to test V parameter
		CVector3 qvec =	CVector3::Cross( tvec, edge1 );

		// Calculate V parameter and test bounds
		v = CVector3::Dot( dir, qvec );
		if( v < 0.0f || u + v > det )
			return FALSE;

		// Calculate t, scale parameters, ray intersects triangle
		t = CVector3::Dot( edge2, qvec );
		FLOAT fInvDet = 1.0f / det;
		t *= fInvDet;
		u *= fInvDet;
		v *= fInvDet;

		if( pt ) *pt = t;
		if( pu ) *pu = u;
		if( pv ) *pv = v;

		return TRUE;
	}

	inline bool	GrabPosMover(XMMATRIX& worldTM, CVector3& from, CVector3& to, eDIRECTION& grabDirection )
	{
		XMMATRIX invWorldTM = XMMATRIX_UTIL::Inverse( NULL, worldTM );

		CVector3 lineStart = CVector3::Transform( from, invWorldTM);
		CVector3 lineEnd = CVector3::Transform( to, invWorldTM);
		CVector3 lineDir = lineEnd - lineStart;

		CONE_MAKE_PARAM param;
		param.radius = 5;
		param.height = 15;
		param.direction = X_AXIS;
		param.offset = CVector3(50, 0, 0);
		param.segment = 10;

		CVertexPC* pVertex = NULL;
		uint16* pIndex = NULL;
		int vertexCount = param.segment +1;
		int primitiveCount = param.segment * 2 - 1;

		CGEOMETRY_CONSTRUCTOR::CreateCone(param, &pVertex, &pIndex);

		for( int iAxis = 0; iAxis < 3; ++iAxis)
		{
			if( iAxis == 1)
			{
				for( int i =0 ; i < vertexCount; ++i)
					TSWAP( pVertex[i].vPos.x, pVertex[i].vPos.y);
			}
			else
			{
				for( int i =0 ; i < vertexCount; ++i)
					TSWAP( pVertex[i].vPos.y, pVertex[i].vPos.z);
			}


			for( int i = 0; i < primitiveCount; ++i)
			{
				int a = pIndex[i*3];
				int b = pIndex[i*3 + 1];
				int c = pIndex[i*3 + 2];

				if( IntersectTriangle( lineStart, lineDir,	pVertex[a].vPos, pVertex[b].vPos, pVertex[c].vPos) )
				{
					if( iAxis == 0 )
						grabDirection = X_AXIS;
					else if( iAxis == 1)
						grabDirection = Y_AXIS;
					else
						grabDirection = Z_AXIS;

					SAFE_DELETE_ARRAY(pVertex);
					SAFE_DELETE_ARRAY(pIndex);
					return true;
				}
			}
		}

		SAFE_DELETE_ARRAY(pVertex);
		SAFE_DELETE_ARRAY(pIndex);

		return false;
	}
};