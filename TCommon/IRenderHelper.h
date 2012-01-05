#pragma once

#include "CDefine.h"

struct IRenderHelper
{
	virtual void RenderBox(CMatrix& mtWorld, CVector3& min, CVector3& max) = 0;
	virtual void RenderGrid(CMatrix& mtWorld, int size, int segmentCount) = 0;
};