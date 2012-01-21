#pragma once

#include "CMathType.h"


enum eLINE_TYPE
{
	LINE_INFINITE,
	LINE_RAY,			// P0 to infinite
	LINE_SEGMENT,		// P0 to P1
};


struct CLine
{
	CLine(CVector3 p0_, CVector3 p1_, eLINE_TYPE type_)
		: p0(p0_)
		, p1(p1_)
		, type(type_)
	{
	}

	CVector3	p0,p1;
	eLINE_TYPE	type;
};