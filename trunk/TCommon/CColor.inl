inline CColor::CColor( DWORD dw )
{
	const FLOAT f = 1.0f / 255.0f;
	r = f * (FLOAT) (unsigned char) (dw >> 16);
	g = f * (FLOAT) (unsigned char) (dw >>  8);
	b = f * (FLOAT) (unsigned char) (dw >>  0);
	a = f * (FLOAT) (unsigned char) (dw >> 24);
}

inline CColor::CColor( const FLOAT* pf )
{
	if(!pf)
		return;

	r = pf[0];
	g = pf[1];
	b = pf[2];
	a = pf[3];
}

inline CColor::CColor( FLOAT fr, FLOAT fg, FLOAT fb, FLOAT fa )
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

inline CColor::operator FLOAT * ()
{
	return (FLOAT *) &r;
}

inline CColor::operator const FLOAT * () const
{
	return (const FLOAT *) &r;
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

inline CColor& CColor::operator *= ( FLOAT f )
{
	r *= f;
	g *= f;
	b *= f;
	a *= f;
	return *this;
}

inline CColor&
	CColor::operator /= ( FLOAT f )
{
	FLOAT fInv = 1.0f / f;
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

inline CColor CColor::operator * ( FLOAT f ) const
{
	return CColor(r * f, g * f, b * f, a * f);
}

inline CColor CColor::operator / ( FLOAT f ) const
{
	FLOAT fInv = 1.0f / f;
	return CColor(r * fInv, g * fInv, b * fInv, a * fInv);
}


inline CColor operator * (FLOAT f, const CColor& c )
{
	return CColor(f * c.r, f * c.g, f * c.b, f * c.a);
}


inline BOOL CColor::operator == ( const CColor& c ) const
{
	return r == c.r && g == c.g && b == c.b && a == c.a;
}

inline BOOL CColor::operator != ( const CColor& c ) const
{
	return r != c.r || g != c.g || b != c.b || a != c.a;
}
