#pragma once


#include "RDefine.h"
#include "CGrowableArray.h"


struct RSpriteVertex
{
	CVector3 vPos;
	CColor vColor;
	CVector2 vTex;
};


struct RENDER_TEXT_BUFFER
{
	RECT		rc;
	WCHAR		strMsg[256];
	CColor		clr;
	bool		bCenter;
};

class RDX11FontRenderer
{
public:
	RDX11FontRenderer();

	HRESULT		SetFontFile(const char* fontDDS);
	void		Destroy();

	void		Render( RENDER_TEXT_BUFFER& text, int sw, int sh );

private:
	void		FillVertex( RENDER_TEXT_BUFFER& text, float screenWidth, float screenHeight );


	ID3D11Buffer*					m_pFontBuffer;
	UINT							m_FontBufferBytes;
	CGrowableArray<RSpriteVertex>	m_FontVertices;
	ID3D11ShaderResourceView*		m_pFontSRV;
};