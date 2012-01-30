#pragma once

#include "CDefine.h"
#include "CText.h"
#include "CResource.h"


enum eIMAGE_FILE_FORMAT
{
	IMG_FILE_BMP         = 0,
	IMG_FILE_JPG         = 1,
	IMG_FILE_PNG         = 3,
	IMG_FILE_DDS         = 4,
	IMG_FILE_TIFF		  = 10,
	IMG_FILE_GIF		  = 11,
	IMG_FILE_WMP		  = 12,
	IMG_FILE_FORCE_DWORD = 0x7fffffff
};



struct IRenderHelper
{
	virtual	void	RenderPosMover(XMMATRIX& tm) =0;
	virtual void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) = 0;
	virtual void	RenderGrid(XMMATRIX& mtWorld, int size, int segmentCount) = 0;
	virtual	void	RenderText(RENDER_TEXT_BUFFER& text) = 0;
};