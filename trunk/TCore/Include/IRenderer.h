#pragma once

#include "CMathType.h"
#include "CCommandBuffer.h"

class CResourceGeometry;
class CResourceMtrl;
struct CCAMERA_DESC;
struct IRenderingCallback;
struct RENDER_TEXT_QUAD;


enum eRENDER_BUFFER
{
	RENDER_BUFFER_0,
	RENDER_BUFFER_1,
	NUM_RENDER_BUFFER,
};

enum eRENDER_COMMAND
{
	RC_RenderFrame,
	RC_DRAW_RENDER_ELEMENT,

	RC_DRAW_HELPER_Skeleton,
	RC_DRAW_HELPER_Axis,
	RC_DRAW_HELPER_Scaler,
	RC_DRAW_HELPER_Rotator,
	RC_DRAW_HELPER_Mover,
	RC_DRAW_HELPER_Box,
	RC_DRAW_HELPER_WorldGrid,
	RC_DRAW_HELPER_Text
};


struct IRenderingCallback
{
	virtual void PreRender(){}
	virtual void PostRender(){}
};

struct IRenderer
{
	virtual void	RenderLine(CVertexPC* pVertex,int count) = 0;
	virtual void	RenderAxis(XMMATRIX& tm) = 0;
	virtual void	RenderScaler(XMMATRIX& tm) = 0;
	virtual void	RenderRotator(XMMATRIX& tm) =0;
	virtual	void	RenderMover(XMMATRIX& tm) =0;
	virtual void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) = 0;
	virtual void	RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount) = 0;
	virtual	void	RenderText(RENDER_TEXT_QUAD* pText) = 0;
};


struct IEngineRenderer : public IRenderer
{
	virtual bool	Init() =0;
	virtual void	Destroy() =0;
	virtual void	AsyncRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback) = 0;

	virtual CCommandBuffer<eRENDER_COMMAND>*	GetFillCommandQueue() =0;
};
