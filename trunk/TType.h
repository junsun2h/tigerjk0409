#pragma once

//Unsigned types.
typedef unsigned __int64	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		ubyte;
typedef unsigned char		uint8;

//Signed types
typedef __int64 int64;
typedef int				int32;
typedef short			int16;
typedef unsigned char	byte;
typedef char			int8;

//floats
typedef float			float32;
typedef double			float64;


typedef D3DXVECTOR2		TVector2;
typedef D3DXVECTOR3		TVector3;
typedef D3DXVECTOR4		TVector4;
typedef D3DXMATRIX		TMatrix;


struct sRGBColor
{
	float32 fRed;
	float32 fGreen;
	float32 fBlue;
	float32 fAlpha;

	sRGBColor(float32 fR = 1.0f, float32 fG = 1.0f, float32 fB = 1.0f, float32 fA = 1.0f) 
		: fRed(fR), 
		fGreen(fG), 
		fBlue(fB), 
		fAlpha(fA) {}
};
