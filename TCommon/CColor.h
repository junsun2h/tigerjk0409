#pragma once

#include "CDefine.h"


#define	COLOR_RED	0xff0000ff
#define	COLOR_GREEN	0xff00ff00
#define	COLOR_BLUE	0xffff0000
#define	COLOR_WHITE	0xffffffff
#define	COLOR_BLACK	0xff000000
#define	COLOR_GRAY	0xff808080


struct CColor
{
public:
	CColor() {}
	CColor( DWORD argb );
	CColor( const float * );
	CColor( float r, float g, float b, float a );

	// casting
	operator DWORD () const;

	operator float* ();
	operator const float* () const;

	// assignment operators
	CColor& operator += ( const CColor& );
	CColor& operator -= ( const CColor& );
	CColor& operator *= ( float );
	CColor& operator /= ( float );

	// unary operators
	CColor operator + () const;
	CColor operator - () const;

	// binary operators
	CColor operator + ( const CColor& ) const;
	CColor operator - ( const CColor& ) const;
	CColor operator * ( float ) const;
	CColor operator / ( float ) const;

	friend CColor operator * ( float, const CColor& );

	bool operator == ( const CColor& ) const;
	bool operator != ( const CColor& ) const;

	float r, g, b, a;
};


inline CColor::CColor( DWORD dw )
{
	const float f = 1.0f / 255.0f;
	r = f * (float) (unsigned char) (dw >> 16);
	g = f * (float) (unsigned char) (dw >>  8);
	b = f * (float) (unsigned char) (dw >>  0);
	a = f * (float) (unsigned char) (dw >> 24);
}

inline CColor::CColor( const float* pf )
{
	if(!pf)
		return;

	r = pf[0];
	g = pf[1];
	b = pf[2];
	a = pf[3];
}

inline CColor::CColor( float fr, float fg, float fb, float fa )
{
	r = fr;
	g = fg;
	b = fb;
	a = fa;
}


// casting
inline CColor::operator DWORD () const
{
	DWORD dwR = r >= 1.0f ? 0xff : r <= 0.0f ? 0x00 : (DWORD) (r * 255.0f + 0.5f);
	DWORD dwG = g >= 1.0f ? 0xff : g <= 0.0f ? 0x00 : (DWORD) (g * 255.0f + 0.5f);
	DWORD dwB = b >= 1.0f ? 0xff : b <= 0.0f ? 0x00 : (DWORD) (b * 255.0f + 0.5f);
	DWORD dwA = a >= 1.0f ? 0xff : a <= 0.0f ? 0x00 : (DWORD) (a * 255.0f + 0.5f);

	return (dwA << 24) | (dwR << 16) | (dwG << 8) | dwB;
}

inline CColor::operator float * ()
{
	return (float *) &r;
}

inline CColor::operator const float * () const
{
	return (const float *) &r;
}


// assignment operators
inline CColor& CColor::operator += ( const CColor& c )
{
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return *this;
}

inline CColor& CColor::operator -= ( const CColor& c )
{
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	return *this;
}

inline CColor& CColor::operator *= ( float f )
{
	r *= f;
	g *= f;
	b *= f;
	a *= f;
	return *this;
}

inline CColor&
	CColor::operator /= ( float f )
{
	float fInv = 1.0f / f;
	r *= fInv;
	g *= fInv;
	b *= fInv;
	a *= fInv;
	return *this;
}


// unary operators
inline CColor CColor::operator + () const
{
	return *this;
}

inline CColor CColor::operator - () const
{
	return CColor(-r, -g, -b, -a);
}

inline CColor CColor::operator + ( const CColor& c ) const
{
	return CColor(r + c.r, g + c.g, b + c.b, a + c.a);
}

inline CColor CColor::operator - ( const CColor& c ) const
{
	return CColor(r - c.r, g - c.g, b - c.b, a - c.a);
}

inline CColor CColor::operator * ( float f ) const
{
	return CColor(r * f, g * f, b * f, a * f);
}

inline CColor CColor::operator / ( float f ) const
{
	float fInv = 1.0f / f;
	return CColor(r * fInv, g * fInv, b * fInv, a * fInv);
}


inline CColor operator * (float f, const CColor& c )
{
	return CColor(f * c.r, f * c.g, f * c.b, f * c.a);
}


inline bool CColor::operator == ( const CColor& c ) const
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

inline bool CColor::operator != ( const CColor& c ) const
{
	return r != c.r || g != c.g || b != c.b || a != c.a;
}
