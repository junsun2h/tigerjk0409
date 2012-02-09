#pragma once

#define _XM_NO_INTRINSICS_

#include <WTypes.h>
#include "xnamath.h"
#include <assert.h>

struct CVector2;
struct CVector3;


struct CQuat
{
	union
	{
		struct
		{
			FLOAT x;
			FLOAT y;
			FLOAT z;
			FLOAT w;
		};
		XMVECTOR m128;
	};

	CQuat()	{}
	CQuat(float x_, float y_, float z_, float w_)
		: x(x_), y(y_), z(z_), w(w_)	{}
	CQuat(XMVECTOR _m128)	{ m128 = _m128; }

	static bool EQual(const CQuat& a, const CQuat& b, float tolerance)
	{
		if( fabs(a.x - b.x) > tolerance ||
			fabs(a.y - b.y) > tolerance ||
			fabs(a.z - b.z) > tolerance ||
			fabs(a.w - b.w) > tolerance )
			return false;

		return true;
	}

	static XMVECTOR		Conjugate(const CQuat& q)	{ return XMQuaternionConjugate(q.m128);}
	static XMVECTOR		Normalise(const CQuat& q)	{ return XMQuaternionNormalize(q.m128);}
	static XMVECTOR		Identity()					{ return XMQuaternionIdentity(); }

	static XMVECTOR		Rotate(const CVector3& axis, float angle);

	// in radians, Roll(Z) -> Pitch(X) -> Yaw(Y) order
	static XMVECTOR		RotateRadian(float Pitch, float Yaw, float Roll)	{ return XMQuaternionRotationRollPitchYaw(Pitch, Yaw, Roll);  }
	static XMVECTOR		RotateDegree(float Pitch, float Yaw, float Roll)	{ return RotateRadian( XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll));  }

	static XMVECTOR		Slerp(const CQuat& q1, const CQuat& q2, float t)	{ return XMQuaternionSlerp( q1.m128, q2.m128, t); }
	static XMVECTOR		DotProduct(const CQuat& q1, const CQuat& q2)		{ return XMQuaternionDot( q1.m128, q2.m128);  }
	static XMVECTOR		Inverse(const CQuat& q)								{ return XMQuaternionInverse( q.m128); }
	static void			ToAxisAngle(const CQuat& q1, CVector3* pAxis, float* pAnlge);
	
	// assignment operators
	CQuat operator *= ( const CQuat& q)			{ m128 = XMQuaternionMultiply(m128, q.m128);  return *this; }
	XMVECTOR operator * ( const CQuat& q)		{ return XMQuaternionMultiply(m128, q.m128);}

	// casting
	operator float* ()							{	return (float*)this; }
	operator const float* () const				{	return (float*)this; }

	BOOL operator == ( const CQuat& q) const	{ return XMQuaternionEqual(m128, q.m128); }
	BOOL operator != ( const CQuat& q) const	{ return !XMQuaternionEqual(m128, q.m128);}
};

struct CVector2
{
	union {
		struct {
			float x,y;
		};

		float f[2];
	};

	CVector2(){}
	CVector2(float x_, float y_)
		: x(x_)	, y(y_)	{}
	explicit CVector2( const float *in )			{ memcpy( f, in, sizeof(CVector2)); }

	static float	Length(const CVector2& v)		{ return sqrtf(v.x * v.x + v.y * v.y); }
	static CVector2	Normalize(const CVector2& v)							
	{
		CVector2 out = v;
		float length = Length(v); 

		if( length == 0)
			return out;

		out.x /= length; 
		out.y /= length;
		return out;
	}

	float		DotProduct(CVector2& p)				{ return x*p.x + y*p.y; }

	// assignment operators
	CVector2& operator += ( const CVector2& in)		{	x = x + in.x; y = y + in.y; return *this; }
	CVector2& operator -= ( const CVector2& in)		{	x = x - in.x; y = y - in.y; return *this;}
	CVector2& operator *= ( float in)				{	x = x*in; y = y*in; return *this;}
	CVector2& operator /= ( float in)				{	assert(in); x = x/in; y = y/in; return *this;}

	// binary operators
	CVector2 operator + ( const CVector2& in) const	{	return CVector2(x + in.x, y + in.y); }
	CVector2 operator - ( const CVector2& in) const	{	return CVector2(x - in.x, y - in.y); }
	CVector2 operator * ( const CVector2& in) const	{	return CVector2(x * in.x, y * in.y); }

	CVector2 operator * ( float in) const			{	return CVector2(x * in, y * in); }
	CVector2 operator / ( float in) const			{	assert(in); return CVector2(x / in, y / in); }

	// casting
	operator float* ()								{	return (float*)this; }
	operator const float* () const					{	return (float*)this; }

	bool operator == ( const CVector2& in) const	{ if( x == in.x && y == in.y ) return true; return false;}
	bool operator != ( const CVector2& in) const	{ if( x != in.x || y != in.y ) return true; return false;}

	bool operator < ( CVector2& in)					{ return Length(*this) < Length(in);}
	bool operator > ( CVector2& in)					{ return Length(*this) < Length(in);}
	bool operator <= ( CVector2& in)				{ return Length(*this) <= Length(in);}
	bool operator >= ( CVector2& in)				{ return Length(*this) <= Length(in);}
};

struct CVector3{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
	};

	CVector3(){}
	CVector3(float x_, float y_, float z_)
		: x(x_)	, y(y_)	, z(z_){}
	CVector3(XMVECTOR _m128)
	{
		x = XMVectorGetX(_m128);  
		y = XMVectorGetY(_m128);
		z = XMVectorGetZ(_m128);
	}

	static bool EQual(const CVector3& a, const CVector3& b, float tolerance)
	{
		if( fabs(a.x - b.x) > tolerance ||
			fabs(a.y - b.y) > tolerance ||
			fabs(a.z - b.z) > tolerance )
			return false;

		return true;
	}

	XMVECTOR ToXMVEECTOR()	const { return XMVectorSet(x, y, z, 1);}

	static float		Length(const CVector3& v)						{ return XMVectorGetX( XMVector3Length( v.ToXMVEECTOR() ) ); }
	static CVector3		Normalize(const CVector3& v)					{ return XMVector3Normalize( v.ToXMVEECTOR() ); }
	static CVector3		Cross(const CVector3& v1, const CVector3& v2)	{ return XMVector3Cross( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ); }
	static float		Dot(const CVector3& v1, const CVector3& v2)		{ return XMVectorGetX( XMVector3Dot( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ) ); }
	static CVector3		Min(const CVector3& v1, const CVector3& v2)		{ return XMVectorMin( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ); }
	static CVector3		Max(const CVector3& v1, const CVector3& v2)		{ return XMVectorMax( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ); }

	static CVector3		Lerp(const CVector3& v1, const CVector3& v2, float ratio)	{	return XMVectorLerp(v1.ToXMVEECTOR(), v2.ToXMVEECTOR(), ratio);	}
	static CVector3		Transform(const CVector3& v, const XMMATRIX& mt )			{	return XMVector3Transform( v.ToXMVEECTOR(), mt);  }
	static CVector3		TransformCoord(const CVector3& v, const XMMATRIX& mt )		{	return XMVector3TransformCoord( v.ToXMVEECTOR(), mt); }
	static CVector3		TransformNormal(const CVector3& v, const XMMATRIX& mt )		{	return XMVector3TransformNormal( v.ToXMVEECTOR(), mt);}


	// assignment operators
	CVector3& operator += ( const CVector3& in)		{	x = x + in.x; y = y + in.y; z = z + in.z; return *this; }
	CVector3& operator -= ( const CVector3& in)		{	x = x - in.x; y = y - in.y; z = z - in.z; return *this;}
	CVector3& operator *= ( const CVector3& in)		{	x = x * in.x; y = y * in.y; z = z * in.z; return *this; }

	CVector3& operator *= ( float in)				{	x = x*in; y = y*in; z = z*in; return *this;}
	CVector3& operator /= ( float in)				{	assert(in); x = x/in; y = y/in; z = z/in; return *this;}

	// binary operators
	CVector3 operator + ( const CVector3& in) const	{	return CVector3(x + in.x, y + in.y, z + in.z); }
	CVector3 operator - ( const CVector3& in) const	{	return CVector3(x - in.x, y - in.y, z - in.z); }
	CVector3 operator * ( const CVector3& in) const	{	return CVector3(x * in.x, y * in.y, z * in.z); }

	CVector3 operator + ( float in) const			{	return CVector3(x + in, y + in, z + in); }
	CVector3 operator - ( float in) const			{	return CVector3(x - in, y - in, z - in); }
	CVector3 operator * ( float in) const			{	return CVector3(x * in, y * in, z * in); }
	CVector3 operator / ( float in) const			{	assert(in); return CVector3(x / in, y / in, z / in); }

	// casting
	operator float* ()								{	return (float*)this; }
	operator const float* () const					{	return (float*)this; }

	bool operator == ( const CVector3& in) const	{ if( x == in.x && y == in.y && z == in.z ) return true; return false;}
	bool operator != ( const CVector3& in) const	{ if( x != in.x || y != in.y || z != in.z ) return true; return false;}

	bool operator < ( const CVector3& in)			{ return Length(*this) < Length(in);}
	bool operator > ( const CVector3& in)			{ return Length(*this) < Length(in);}
	bool operator <= ( const CVector3& in)			{ return Length(*this) <= Length(in);}
	bool operator >= ( const CVector3& in)			{ return Length(*this) <= Length(in);}
};

struct CVector4{
	union
	{
		struct
		{
			FLOAT w;
			FLOAT z;
			FLOAT y;
			FLOAT x;
		};
		XMVECTOR m128;
	};

	CVector4() {}
	CVector4(float x_, float y_, float z_, float w_)
		: x(x_)	, y(y_)	, z(z_), w(w_)	{}
	CVector4(XMVECTOR _m128)	{ m128 = _m128; }
	CVector4(CVector3 v)		{ m128 = XMVectorSet(v.x, v.y, v.z, 1); }
	XMVECTOR			ToXMVEECTOR()	const { return XMVectorSet(x, y, z, w);}

	static bool EQual(const CVector4& a, const CVector4& b, float tolerance)
	{
		if( fabs(a.x - b.x) > tolerance ||
			fabs(a.y - b.y) > tolerance ||
			fabs(a.z - b.z) > tolerance ||
			fabs(a.w - b.w) > tolerance )
			return false;

		return true;
	}


	static float		Length(const CVector4& v)						{ return XMVectorGetX( XMVector4Length( v.m128) ); }
	static CVector4		Normalize(const CVector4& v)					{ return XMVector4Normalize( v.m128); }
	static float		Dot(const CVector4& v1, const CVector4& v2)		{ return XMVectorGetX(XMVector4Dot(v1.m128, v2.m128)); }
	static CVector4		Min(const CVector4& v1, const CVector4& v2)		{ return XMVectorMin( v1.m128, v2.m128 ); }
	static CVector4		Max(const CVector4& v1, const CVector4& v2)		{ return XMVectorMax( v1.m128, v2.m128 ); }

	static CVector4		Transform(const CVector4& v, const XMMATRIX& mt )			{	return XMVector4Transform( v.m128, mt);  }
	static CVector4		Lerp(const CVector4& v1, const CVector4& v2, float ratio)	{	return XMVectorLerp(v1.m128, v2.m128, ratio);	}

	// assignment operators
	CVector4& operator += ( const CVector4& in)		{	*this = *this + in; }
	CVector4& operator -= ( const CVector4& in)		{	*this = *this - in; }
	CVector4& operator *= ( const CVector4& in)		{	*this = *this * in; }
	CVector4& operator /= ( const CVector4& in)		
	{	
		in.x != 0 ? x = x / in.x: x = 0;
		in.y != 0 ? y = y / in.y: y = 0;
		in.z != 0 ? z = z / in.z: z = 0;
		return *this;
	}

	CVector4& operator *= ( float in)				{	*this = *this + in;}
	CVector4& operator /= ( float in)				{	*this = *this + in;}

	// binary operators
	CVector4 operator + ( const CVector4& in) const	{	return XMVectorAdd( m128, in.m128); }
	CVector4 operator - ( const CVector4& in) const	{	return XMVectorSubtract( m128, in.m128); }
	CVector4 operator * ( const CVector4& in) const	{	return XMVectorMultiply( m128, in.m128); }

	CVector4 operator + ( float in) const			{	return XMVectorAdd( m128, XMVectorSet(in,in,in,in) ); }
	CVector4 operator - ( float in) const			{	return XMVectorSubtract( m128, XMVectorSet(in,in,in,in)); }
	CVector4 operator * ( float in) const			{	return XMVectorMultiply( m128, XMVectorSet(in,in,in,in)); }
	CVector4 operator / ( float in) const			{	return XMVectorDivide( m128, XMVectorSet(in,in,in,in)); }

	// casting
	operator float* ()								{	return (float*)this; }
	operator const float* () const					{	return (float*)this; }

	bool operator == ( const CVector4& in) const	{ return !(!XMVector4Equal( m128, in.m128)); }
	bool operator != ( const CVector4& in) const	{ return !XMVector4Equal( m128, in.m128); }

	bool operator < ( const CVector4& in)			{ return Length(*this) < Length(in);}
	bool operator > ( const CVector4& in)			{ return Length(*this) < Length(in);}
	bool operator <= ( const CVector4& in)			{ return Length(*this) <= Length(in);}
	bool operator >= ( const CVector4& in)			{ return Length(*this) <= Length(in);}
};


namespace XMMATRIX_UTIL
{
	inline XMMATRIX	Inverse(CVector4* pDeterminant, const XMMATRIX& mt)		
	{  
		XMVECTOR _m128;
		XMMATRIX inv = XMMatrixInverse( &_m128, mt);

		if( pDeterminant != NULL)
			*pDeterminant = _m128;

		return inv;
	}

	inline BOOL Decompose(CVector3* pScale, CQuat* pRot, CVector3* pPos, const XMMATRIX& mt)
	{
		XMVECTOR outScale;
		XMVECTOR outRotQuat;
		XMVECTOR outTrans;

		if( !XMMatrixDecompose( &outScale, &outRotQuat, &outTrans, mt) )
		{
			assert(0);
			return false;
		}

		if( pScale != NULL )
			*pScale = outScale;

		if( pRot != NULL )
			*pRot = outRotQuat;

		if( pPos != NULL )
			*pPos = outTrans;

		return true;
	}

	inline void ToRollPitchYaw(float& pitch, float& yaw, float& roll, const XMMATRIX& mt)
	{
		//	since xna math rotate Roll(Z) -> Pitch(X) -> Yaw(Y), we have to decompose from Yaw, followed by Pitch and finally roll
		CVector3 y = mt.r[1];
		CVector3 z = mt.r[2];

		// decompose y-aix( yaw )
		yaw = XMConvertToDegrees( atan2f( z.x, z.z ) );

		// decompose x-aix( pitch )
		XMMATRIX rot_y = XMMatrixRotationY( XMConvertToRadians( -yaw)  );

		y = CVector3::Transform( y , rot_y );
		z = CVector3::Transform( z , rot_y );

		pitch = XMConvertToDegrees( - atan2f( z.y, z.z ) );

		// decompose z-aix( roll )
		XMMATRIX rot_z = XMMatrixRotationX( XMConvertToRadians( -pitch)  );

		y = CVector3::Transform( y , rot_z );

		roll = XMConvertToDegrees( -atan2f( y.x, y.y ) );
	}

	inline XMMATRIX ToMatrix(float& pitch, float& yaw, float& roll)
	{
		return XMMatrixRotationRollPitchYaw( XMConvertToRadians(pitch ),
										XMConvertToRadians(yaw ),
										XMConvertToRadians(roll ));
	}

	inline XMMATRIX	TransformationAffine(const CVector3& scale, const CQuat& rot, const CVector3& pos) 
	{ 
		return XMMatrixAffineTransformation(scale.ToXMVEECTOR(), XMVectorZero(), rot.m128, pos.ToXMVEECTOR()); 
	}
}


inline XMVECTOR CQuat::Rotate(const CVector3& axis, float angle)	
{ 
	return XMQuaternionRotationAxis( axis.ToXMVEECTOR(), angle);
}

inline void	CQuat::ToAxisAngle(const CQuat& q1, CVector3* pAxis, float* pAnlge)
{
	XMVECTOR axis;
	XMQuaternionToAxisAngle(&axis, pAnlge, q1.m128);
	*pAxis = axis;
}