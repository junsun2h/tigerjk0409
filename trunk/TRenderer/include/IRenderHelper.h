#pragma once


struct RENDER_TEXT_QUAD;
class CResourceTexture;


struct IRenderHelper
{
	virtual void	RenderLine( CVertexPC* pVetex, int count) = 0;
	virtual void	RenderAxis(XMMATRIX& tm) = 0;
	virtual void	RenderScaler(XMMATRIX& tm) = 0;
	virtual void	RenderRotator(XMMATRIX& tm) =0;
	virtual	void	RenderMover(XMMATRIX& tm) =0;
	virtual void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) = 0;
	virtual void	RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount) = 0;
	virtual	void	RenderText(RENDER_TEXT_QUAD* pText) = 0;
	virtual void	RenderSphere(CVector3* pos, float radius) = 0;
	virtual void	RenderPlane(XMMATRIX& tm, CVector2 size) = 0;

	virtual void	SetFontTexture( CResourceTexture* pTexture) =0;
};