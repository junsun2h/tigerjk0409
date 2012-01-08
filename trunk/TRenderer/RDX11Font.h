#pragma once


#include "RDefine.h"
#include "CGrowableArray.h"
#include "IResource.h"
#include "CText.h"


class RDX11FontRenderer
{
public:
	RDX11FontRenderer();

	HRESULT		SetFontFile(const char* fontDDS);
	void		Destroy();

	void		Render( RENDER_TEXT_BUFFER& text );

private:
	void		FillVertex( RENDER_TEXT_BUFFER& text );

	ID3D11Buffer*					m_pFontBuffer;
	UINT							m_FontBufferBytes;
	CGrowableArray<CVertexPCT>		m_FontVertices;
	ID3D11ShaderResourceView*		m_pFontSRV;
};