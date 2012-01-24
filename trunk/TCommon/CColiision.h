#pragma once


#include "CMathType.h"


enum eCOLLISION_CHECK_OPTION
{
	LINE_COLLISION
};

struct ICollisionDesc
{
	virtual	eCOLLISION_CHECK_OPTION Type() = 0; 
};

struct CCollisionDescLine : ICollisionDesc
{
	eCOLLISION_CHECK_OPTION Type() override { return LINE_COLLISION; };

	CVector3	from;
	CVector3	to;
};

