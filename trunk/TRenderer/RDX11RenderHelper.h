#pragma once

#include "IRenderHelper.h"
#include "IResource.h"
#include "RDefine.h"
#include "CGrowableArray.h"
#include "RDX11Font.h"


class RDX11RenderHelper : public IRenderHelper
{
public:
	RDX11RenderHelper();

public:
	bool	SaveTextureToFile(const CResourceTexture* pTexture, IMAGE_FILE_FORMAT format, const char* fileName) override;
	bool	SaveTextureToMemory(const CResourceTexture* pTexture, IMAGE_FILE_FORMAT format, uint8** ppDest, size_t& size) override;

	void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max) override;
	void	RenderGrid(XMMATRIX& mtWorld, int size, int segmentCount) override;
	void	RenderText(RENDER_TEXT_BUFFER& text) override;
	
public:
	void	Init(const char* fontDDS);
	void	Destroy();
	void	ApplyRenderState();

private:
	UINT						m_LineBufferBytes;
	CGrowableArray<CVertexPC>	m_LineVertices;
	ID3D11Buffer*				m_pLineBuffer;

	RDX11FontRenderer			m_FontRenderer;
};