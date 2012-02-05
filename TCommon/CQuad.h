#pragma once

#include "CColor.h"


struct CQuad
{
	int  left;
	int  top;
	int  right;
	int  bottom;

	bool	IsIn(float x, float y)
	{
		if( x < left || x > right ||
			y < bottom || y > top )
			return false;

		return true;
	}

	CVector2 Center()
	{
		return CVector2( (left + right)*0.5f, (bottom + top)*0.5f);
	}

	CVector2 Radius()
	{
		return CVector2( (right- left)*0.5f, (top- bottom)*0.5f );
	}
};


struct RENDER_TEXT_QUAD
{
	CQuad		rc;			// the box area which is filled with text
	wchar_t		strMsg[256];
	DWORD		clr;
	bool		bCenter;
};