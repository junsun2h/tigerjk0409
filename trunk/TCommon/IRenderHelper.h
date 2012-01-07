#pragma once

#include "CDefine.h"

struct IRenderHelper
{
	virtual void RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max) = 0;
	virtual void RenderGrid(XMMATRIX& mtWorld, int size, int segmentCount) = 0;
};