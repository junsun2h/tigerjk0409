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
	void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) override;
	void	RenderGrid(XMMATRIX& mtWorld, int size, int segmentCount) override;
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