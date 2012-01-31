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
	//Calculate the line segment PaPb that is the shortest route between
	//two lines P1P2 and P3P4. Calculate also the values of mua and mub where
	//	Pa = P1 + mua (P2 - P1)
	//	Pb = P3 + mub (P4 - P3)
	//Return FALSE if no solution exists.
	//--------------------------------------------------------------------------------------
	bool LineLineIntersect( CVector3 p1,CVector3 p2,CVector3 p3,CVector3 p4, CVector3 *pa,CVector3 *pb);

	//--------------------------------------------------------------------------------------
	// Given a ray origin (orig) and direction (dir), and three vertices of a triangle, this
	// function returns TRUE and the interpolated texture coordinates if the ray intersects 
	// the triangle
	// brought from d3dx example
	//--------------------------------------------------------------------------------------
	bool LineTriangleIntersect( const CVector3& orig, const CVector3& dir, 
								CVector3& v0, CVector3& v1, CVector3& v2,
								FLOAT* pDist = NULL, FLOAT* pu = NULL, FLOAT* pv = NULL );





	//--------------------------------------------------------------------------------------
	inline bool	LineTriangleIntersect( const CVector3& orig, const CVector3& dir,
							CVector3& v0, CVector3& v1, CVector3& v2,
							FLOAT* pDist, FLOAT* pu, FLOAT* pv)
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

		if( pDist ) *pDist = t;
		if( pu ) *pu = u;
		if( pv ) *pv = v;

		return TRUE;
	}

	//--------------------------------------------------------------------------------------
	inline bool LineLineIntersect( CVector3 p1,CVector3 p2,CVector3 p3,CVector3 p4, CVector3 *pa,CVector3 *pb)
	{
	   CVector3 p13,p43,p21;
	   float d1343,d4321,d1321,d4343,d2121;
	   float numer,denom;
	   float EPS = float(1e-8);

	   p13.x = p1.x - p3.x;
	   p13.y = p1.y - p3.y;
	   p13.z = p1.z - p3.z;
	   p43.x = p4.x - p3.x;
	   p43.y = p4.y - p3.y;
	   p43.z = p4.z - p3.z;

	   if ( fabs(p43.x) < EPS && fabs(p43.y) < EPS && fabs(p43.z) < EPS)
		  return false;
	   p21.x = p2.x - p1.x;
	   p21.y = p2.y - p1.y;
	   p21.z = p2.z - p1.z;

	   if ( fabs(p21.x) < EPS && fabs(p21.y) < EPS && fabs(p21.z) < EPS)
		  return false;

	   d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	   d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	   d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	   d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	   d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

	   denom = d2121 * d4343 - d4321 * d4321;
	   if ( fabs(denom) < EPS)
		  return false;
	   numer = d1343 * d4321 - d1321 * d4343;

	   float mua = numer / denom;
	   float mub = (d1343 + d4321 * mua) / d4343;

	   pa->x = p1.x + mua * p21.x;
	   pa->y = p1.y + mua * p21.y;
	   pa->z = p1.z + mua * p21.z;
	   pb->x = p3.x + mub * p43.x;
	   pb->y = p3.y + mub * p43.y;
	   pb->z = p3.z + mub * p43.z;

	   return true;
	}

};