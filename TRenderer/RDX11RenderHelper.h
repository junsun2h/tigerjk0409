#pragma once

#include "IRenderHelper.h"
#include "CResource.h"
#include "RDefine.h"
#include "CGrowableArray.h"
#include "RDX11Font.h"


class RDX11RenderHelper : public IRenderHelper
{
public:
	RDX11RenderHelper();

public:
	void	RenderAxis(XMMATRIX& tm, float scale) override;
	void	RenderScaler(XMMATRIX& tm, float scale) override;
	void	RenderRotator(XMMATRIX& tm, float scale) override;
	void	RenderMover(XMMATRIX& tm, float scale) override;
	void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) override;
	void	RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount) override;
	void	RenderText(RENDER_TEXT_BUFFER& text) override;
	
public:
	void	Init(const char* fontDDS);
	void	Destroy();
	void	ApplyRenderState();

private:
	void	DrawLine();

	UINT						m_LineBufferBytes;
	CGrowableArray<CVertexPC>	m_LineVertices;
	ID3D11Buffer*				m_pLineBuffer;

	RDX11FontRenderer			m_FontRenderer;
};