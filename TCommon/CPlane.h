#pragma once

#include "CDefine.h"
#include "CLine.h"
#include <math.h>
#include <stdlib.h>


struct CPlane
{
public:
	CPlane();
	CPlane( const float*  pf );
	CPlane( float a_, float b_, float c_, float d_ );


	float	a,b,c,d;	// formula: ax + by + cz = d
	CVector3 n;			// unit normal
	CVector3 p;			// the point that closet to (0,0,0)


	void		Normalize();
	float		Dot(const CVector3& v)					{ return a * v.x + b * v.y + c * v.z + d; }
	bool		IntersectPlane(CPlane& p, CLine* pLine);

	// casting
	operator float* ()								{ return (float *) &a; }
	operator const float* () const					{ return (const float *) &a; }

	// assignment operators
	CPlane& operator *= ( float f)					{ a *= f;	b *= f;	c *= f;	d *= f;	return *this;}
	CPlane& operator /= ( float f);

	// unary operators
	CPlane operator + () const						{ return *this; }
	CPlane operator - () const						{ return CPlane(-a, -b, -c, -d); }

	// binary operators
	CPlane operator * ( float f) const				{ return CPlane(a * f, b * f, c * f, d * f); }
	CPlane operator / ( float f) const;

	friend CPlane operator * ( float f, const CPlane& p)	{ return CPlane(f * p.a, f * p.b, f * p.c, f * p.d); }

	bool operator == ( const CPlane& p) const		{ 	return a == p.a && b == p.b && c == p.c && d == p.d; }
	bool operator != ( const CPlane& p) const		{ 	return a != p.a || b != p.b || c != p.c || d != p.d; }
};

inline CPlane& CPlane::operator /= ( float f )
{
	float fInv = 1.0f / f;
	a *= fInv;
	b *= fInv;
	c *= fInv;
	d *= fInv;
	return *this;
}

inline CPlane CPlane::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return CPlane(a * fInv, b * fInv, c * fInv, d * fInv);
}


inline CPlane::CPlane()
	: a(0)
	, b(0)
	, c(0)
	, d(0)
	, n(0,0,0)
	, p(0,0,0)
{
}

inline CPlane::CPlane( const float*  pf )
{
	a = pf[0]; 
	b = pf[1]; 
	c = pf[2]; 
	d = pf[3];

	float denominator = a*a + b*b + c*c;
	p = CVector3( a*d/ denominator, b*d/denominator, c*d/denominator );
	n = CVector3( a, b, c);
	n - CVector3::Normalize( n );
}

inline CPlane::CPlane( float a_, float b_, float c_, float d_ )
{ 
	a = a_; 
	b = b_; 
	c = c_; 
	d = d_;

	float denominator = a*a + b*b + c*c;
	p = CVector3( a*d/ denominator, b*d/denominator, c*d/denominator );
	n = CVector3( a, b, c);
	n = CVector3::Normalize( n );
}

inline void CPlane::Normalize()
{
	float norm;

	norm = sqrt(a * a + b * b + c * c);

	if ( norm )
	{
		a = a / norm;
		b = b / norm;
		c = c / norm;
		d = d / norm;
	}
	else
	{
		a = 0.0f;
		b = 0.0f;
		c = 0.0f;
		d = 0.0f;
	}
}

inline bool CPlane::IntersectPlane(CPlane& plane, CLine* pLine)
{
	CVector3   u =  CVector3::Cross( n , plane.n );         // cross product
	float    ax = (u.x >= 0 ? u.x : -u.x);
	float    ay = (u.y >= 0 ? u.y : -u.y);
	float    az = (u.z >= 0 ? u.z : -u.z);

	if ((ax+ay+az) < 0.00000001 ) 
		return false;

	// Pn1 and Pn2 intersect in a line
	// first determine max abs coordinate of cross product
	int      maxc;                      // max coordinate
	if (ax > ay) {
		if (ax > az)
			maxc = 1;
		else maxc = 3;
	}
	else {
		if (ay > az)
			maxc = 2;
		else maxc = 3;
	}

	// next, to get a point on the intersect line
	// zero the max coord, and solve for the other two
	CVector3    iP;               // intersect point
	float    d1, d2;           // the constants in the 2 plane equations
	d1 = - CVector3::Dot( n, p );  // note: could be pre-stored with plane
	d2 = - CVector3::Dot( plane.n , plane.p );  // ditto

	switch (maxc) {            // select max coordinate
	case 1:                    // intersect with x=0
		iP.x = 0;
		iP.y = (d2 * n.z - d1 * plane.n.z) / u.x;
		iP.z = (d1 * plane.n.y - d2 * n.y) / u.x;
		break;
	case 2:                    // intersect with y=0
		iP.x = (d1 * plane.n.z - d2 * n.z) / u.y;
		iP.y = 0;
		iP.z = (d2 * n.x - d1 * plane.n.x) / u.y;
		break;
	case 3:                    // intersect with z=0
		iP.x = (d2 * n.y - d1 * plane.n.y) / u.z;
		iP.y = (d1 * plane.n.x - d2 * n.x) / u.z;
		iP.z = 0;
	}

	pLine->p0 = iP;
	pLine->p1 = iP + u;
	return true;
}