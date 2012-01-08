#pragma once

#include "CColor.h"


struct CQuad
{
	int  left;
	int  top;
	int  right;
	int  bottom;
};

struct RENDER_TEXT_BUFFER
{
	CQuad		rc;			// the box area which is filled with text
	wchar_t		strMsg[256];
	DWORD		clr;
	bool		bCenter;
};