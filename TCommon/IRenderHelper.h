#pragma once


struct IEntity;
struct RENDER_TEXT_QUAD;


struct IRenderHelper
{
	virtual void	RenderSkeleton(IEntity* pEntity) = 0;
	virtual void	RenderAxis(XMMATRIX& tm, float scale) = 0;
	virtual void	RenderScaler(XMMATRIX& tm, float scale) = 0;
	virtual void	RenderRotator(XMMATRIX& tm, float scale) =0;
	virtual	void	RenderMover(XMMATRIX& tm, float scale) =0;
	virtual void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) = 0;
	virtual void	RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount) = 0;
	virtual	void	RenderText(RENDER_TEXT_QUAD* pText) = 0;
};