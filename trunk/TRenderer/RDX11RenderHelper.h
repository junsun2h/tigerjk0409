#pragma once

#include "IRenderHelper.h"
#include "IResource.h"
#include "RDefine.h"
#include "CGrowableArray.h"


class RDX11RenderHelper : IRenderHelper
{
public:
	RDX11RenderHelper();

	bool Init();
	void Destroy();
	void RenderBox(CMatrix& mtWorld, CVector3& min, CVector3& max) override;
	void RenderGrid(CMatrix& mtWorld, int size, int segmentCount) override;
	void ApplyRenderState();

private:
	UINT						m_LineBufferBytes;
	ID3D11VertexShader*			m_pVSRender;
	ID3D11PixelShader*			m_pPSRender;
	ID3D11InputLayout*			m_pInputLayout;

	CGrowableArray<CVertexPC>	m_LineVertices;
	ID3D11Buffer*				m_pLineBuffer;
};