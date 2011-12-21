#pragma once

#include <WTypes.h>
#include "xnamath.h"
#include <assert.h>

struct CMatrix;
struct CVector2;
struct CVector3;
struct CVector4;


struct CQuat
{
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

	CQuat()	{}
	CQuat(float x_, float y_, float z_, float w_)
		: x(x_), y(y_), z(z_), w(w_)	{}
	CQuat(XMVECTOR _m128)	{ m128 = _m128; }

	static XMVECTOR		Conjugate(const CQuat& q);
	static XMVECTOR		Normalise(const CQuat& q);
	static XMVECTOR		Identity()		{ return XMQuaternionIdentity(); }

	static XMVECTOR		Ratate(const CVector3& axis, float angle);

	// in radians, Roll(Z) -> Pitch(X) -> Yaw(Y) order
	static XMVECTOR		RatateRadian(float Pitch, float Yaw, float Roll);
	static XMVECTOR		RatateDegree(float Pitch, float Yaw, float Roll);

	static XMVECTOR		Slerp(const CQuat& q1, const CQuat& q2, float t);
	static XMVECTOR		DotProduct(const CQuat& q1, const CQuat& q2);
	static XMVECTOR		Inverse(const CQuat& q);
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
	static CVector2	Normalize(const CVector2& v);

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

	XMVECTOR ToXMVEECTOR()	const { return XMVectorSet(x, y, z, 0);}

	static float		Length(const CVector3& v)						{ return XMVectorGetX( XMVector3Length( v.ToXMVEECTOR() ) ); }
	static CVector3		Normalize(const CVector3& v)					{ return XMVector3Normalize( v.ToXMVEECTOR() ); }
	static CVector3		Cross(const CVector3& v1, const CVector3& v2)	{ return XMVector3Cross( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ); }
	static float		Dot(const CVector3& v1, const CVector3& v2)		{ return XMVectorGetX( XMVector3Dot( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ) ); }
	static CVector3		Min(const CVector3& v1, const CVector3& v2)		{ return XMVectorMin( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ); }
	static CVector3		Max(const CVector3& v1, const CVector3& v2)		{ return XMVectorMax( v1.ToXMVEECTOR(), v2.ToXMVEECTOR() ); }

	static CVector3		Lerp(const CVector3& v1, const CVector3& v2, float ratio)	{	return XMVectorLerp(v1.ToXMVEECTOR(), v2.ToXMVEECTOR(), ratio);	}
	static CVector3		Transform(const CVector3& v, const CMatrix& mt );
	static CVector3		TransformCoord(const CVector3& v, const CMatrix& mt );
	static CVector3		TransformNormal(const CVector3& v, const CMatrix& mt );

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
	CVector4(CVector3 v)		{ m128 = XMVectorSet(v.x, v.y, v.z, 0); }

	static float		Length(const CVector4& v)						{ return XMVectorGetX( XMVector4Length( v.m128) ); }
	static CVector4		Normalize(const CVector4& v)					{ return XMVector4Normalize( v.m128); }
	static float		Dot(const CVector4& v1, const CVector4& v2)		{ return XMVectorGetX(XMVector4Dot(v1.m128, v2.m128)); }
	static CVector4		Min(const CVector4& v1, const CVector4& v2)		{ return XMVectorMin( v1.m128, v2.m128 ); }
	static CVector4		Max(const CVector4& v1, const CVector4& v2)		{ return XMVectorMax( v1.m128, v2.m128 ); }

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

	bool operator == ( const CVector4& in) const	{ return !(!XMVector3Equal( m128, in.m128)); }
	bool operator != ( const CVector4& in) const	{ return !XMVector3Equal( m128, in.m128); }

	bool operator < ( const CVector4& in)			{ return Length(*this) < Length(in);}
	bool operator > ( const CVector4& in)			{ return Length(*this) < Length(in);}
	bool operator <= ( const CVector4& in)			{ return Length(*this) <= Length(in);}
	bool operator >= ( const CVector4& in)			{ return Length(*this) <= Length(in);}
};


struct CMatrix
{
	CMatrix(){}
	CMatrix(const XMMATRIX& mt)
	{
		_m = mt;
	}

	XMMATRIX _m;

	CVector3			GetRow(int row) const;
	void				SetRow(const CVector3& vec, int row);

	static BOOL			IsValid(const CMatrix& mt);
	static BOOL         IsIdentity(const CMatrix& mt);

	static XMMATRIX		Multiply(const CMatrix& mt1, const CMatrix& mt2);
	static XMMATRIX		MultiplyTranspose(const CMatrix& mt1, const CMatrix& mt2);
	static XMMATRIX		Transpose(const CMatrix& mt);

	static XMVECTOR		Determinant(const CMatrix& mt);
	static XMMATRIX		Inverse(CVector3* pDeterminant, const CMatrix& mt);
	static BOOL         Decompose(CVector3* pScale, CQuat* pRot, CVector3* pPos, const CMatrix& mt);

	static XMMATRIX		Identity()											{ return XMMatrixIdentity(); }
	static XMMATRIX		Translation(float x, float y, float z)				{  return XMMatrixTranslation(x, y, z); }
	static XMMATRIX		Scaling(float x, float y, float z)					{  return XMMatrixScaling(x, y, z); }
	static XMMATRIX		RotateX(FLOAT Angle)								{  return XMMatrixRotationX(Angle);	};
	static XMMATRIX		RotateY(FLOAT Angle)								{  return XMMatrixRotationY(Angle);	};
	static XMMATRIX		RotateZ(FLOAT Angle)								{  return XMMatrixRotationZ(Angle);	};
	static XMMATRIX		RotationRollPitchYawRadian(FLOAT Pitch, FLOAT Yaw, FLOAT Roll)	{ return XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll); }
	static XMMATRIX		RotationRollPitchYawDegree(FLOAT Pitch, FLOAT Yaw, FLOAT Roll)	{ return XMMatrixRotationRollPitchYaw( XMConvertToRadians(Pitch), XMConvertToRadians(Yaw), XMConvertToRadians(Roll) ); }
	static XMMATRIX		RotateAxis(const CVector3& Axis, FLOAT Angle)		{  return XMMatrixRotationAxis( Axis.ToXMVEECTOR(), Angle); }
	static XMMATRIX		RotateQuaternion(const CQuat& Quaternion)			{  return XMMatrixRotationQuaternion(Quaternion.m128); }
	static CQuat		ToQuaternion(const CMatrix& mt)						{  return XMQuaternionRotationMatrix(mt._m);  }	
	static void			ToRollPitchYaw(float& pitch, float& yaw, float& roll, const CMatrix& mt);
	static XMMATRIX		TransformationAffine(const CVector3& scale, const CVector3& rotOrigin, const CQuat& rot, const CVector3& pos) { return XMMatrixAffineTransformation(scale.ToXMVEECTOR(), pos.ToXMVEECTOR(), rot.m128, pos.ToXMVEECTOR()); }

	static XMMATRIX		LookAtLH(const CVector4& Eye, const CVector4& Focus, const CVector4& Up)	{ return XMMatrixLookAtLH(Eye.m128, Focus.m128, Up.m128); }
	static XMMATRIX		LookAtRH(const CVector4& Eye, const CVector4& Focus, const CVector4& Up)	{ return XMMatrixLookAtRH(Eye.m128, Focus.m128, Up.m128); }
	static XMMATRIX		LookToLH(const CVector4& EyePos, const CVector4& EyeDir, const CVector4& Up)	{ return XMMatrixLookToLH(EyePos.m128, EyeDir.m128, Up.m128); }
	static XMMATRIX		LookToRH(const CVector4& EyePos, const CVector4& EyeDir, const CVector4& Up)	{ return XMMatrixLookToRH(EyePos.m128, EyeDir.m128, Up.m128); }
	static XMMATRIX		PerspectiveLH(FLOAT w, FLOAT h, FLOAT NearZ, FLOAT FarZ)	{ return XMMatrixPerspectiveLH(w, h, NearZ, FarZ); }
	static XMMATRIX		PerspectiveRH(FLOAT w, FLOAT h, FLOAT NearZ, FLOAT FarZ)	{ return XMMatrixPerspectiveRH(w, h, NearZ, FarZ); }
	static XMMATRIX		PerspectiveFovLH(FLOAT FovAngleY, FLOAT AspectHByW, FLOAT NearZ, FLOAT FarZ)	{	return XMMatrixPerspectiveFovLH( FovAngleY, AspectHByW, NearZ, FarZ); }
	static XMMATRIX		PerspectiveFovRH(FLOAT FovAngleY, FLOAT AspectHByW, FLOAT NearZ, FLOAT FarZ)	{	return XMMatrixPerspectiveFovRH( FovAngleY, AspectHByW, NearZ, FarZ); }
	static XMMATRIX		PerspectiveOffCenterLH(FLOAT ViewLeft, FLOAT ViewRight, FLOAT ViewBottom, FLOAT ViewTop, FLOAT NearZ, FLOAT FarZ) { return XMMatrixPerspectiveOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ); }
	static XMMATRIX		PerspectiveOffCenterRH(FLOAT ViewLeft, FLOAT ViewRight, FLOAT ViewBottom, FLOAT ViewTop, FLOAT NearZ, FLOAT FarZ) { return XMMatrixPerspectiveOffCenterRH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ); }
	static XMMATRIX		OrthographicLH(FLOAT ViewWidth, FLOAT ViewHeight, FLOAT NearZ, FLOAT FarZ)	{ return XMMatrixOrthographicLH( ViewWidth, ViewHeight, NearZ, FarZ);	}
	static XMMATRIX		OrthographicRH(FLOAT ViewWidth, FLOAT ViewHeight, FLOAT NearZ, FLOAT FarZ)	{ return XMMatrixOrthographicRH( ViewWidth, ViewHeight, NearZ, FarZ);	}
	static XMMATRIX		OrthographicOffCenterLH(FLOAT ViewLeft, FLOAT ViewRight, FLOAT ViewBottom, FLOAT ViewTop, FLOAT NearZ, FLOAT FarZ)	{ return XMMatrixOrthographicOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ); }
	static XMMATRIX		OrthographicOffCenterRH(FLOAT ViewLeft, FLOAT ViewRight, FLOAT ViewBottom, FLOAT ViewTop, FLOAT NearZ, FLOAT FarZ)	{ return XMMatrixOrthographicOffCenterRH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ); }

	CMatrix&  operator*= (CONST CMatrix& M)			{ *this = XMMatrixMultiply( _m, M._m);	return *this;}
	CMatrix   operator* (CONST CMatrix& M) CONST	{ return XMMatrixMultiply( _m, M._m);}
};



#define	COLOR_RED	0xff0000ff
#define	COLOR_GREEN	0xff00ff00
#define	COLOR_BLUE	0xffff0000
#define	COLOR_WHITE	0xffffffff
#define	COLOR_BLACK	0xff000000
#define	COLOR_GRAY	0xff808080


#include "CVector.inl"
#include "CQuat.inl"
#include "CMatrix.inl"
