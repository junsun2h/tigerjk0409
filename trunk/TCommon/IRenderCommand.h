#pragma once

#include "CMathType.h"
#include "CCommandBuffer.h"

class CResourceGeometry;
class CResourceMtrl;
struct CCAMERA_DESC;
struct IRenderingCallback;


enum eRENDER_BUFFER
{
	RENDER_BUFFER_0,
	RENDER_BUFFER_1,
	NUM_RENDER_BUFFER,
};

enum eRENDER_COMMAND
{
	RC_RenderFrame,
	RC_DRAW_OBJECT,
	RC_DRAW_OBJECT_SKIN,

	RC_DRAW_HELPER_Skeleton,
	RC_DRAW_HELPER_Axis,
	RC_DRAW_HELPER_Scaler,
	RC_DRAW_HELPER_Rotator,
	RC_DRAW_HELPER_Mover,
	RC_DRAW_HELPER_Box,
	RC_DRAW_HELPER_WorldGrid,
	RC_DRAW_HELPER_Text
};


struct CRenderParam
{
	CResourceGeometry*	pGeometry;
	XMMATRIX			worldTM;
	CResourceMtrl*		pMaterial;
};

struct CRenderParamSkin : public CRenderParam
{
	XMMATRIX*			refSkinTM;
	UINT				refSkinTMCount;
};



struct IRenderCommandMgr
{
	virtual void	FlushAndWait() = 0;
	virtual void	FlushWithoutWait() = 0;

	virtual	bool	InitAsyncRenderThreadObjects() =0;
	virtual void	AsyncRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback) = 0;

	virtual CCommandBuffer<eRENDER_COMMAND>*	GetFillCommandQueue() =0;
};
