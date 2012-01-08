#pragma once

#include "CDefine.h"
#include "CText.h"


struct IRenderHelper
{
	virtual void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max) = 0;
	virtual void	RenderGrid(XMMATRIX& mtWorld, int size, int segmentCount) = 0;
	virtual	void	RenderText(RENDER_TEXT_BUFFER& text) = 0;
};