#pragma once


#include "RDefine.h"
#include "CGrowableArray.h"


struct DXUTSpriteVertex
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
	ID3D11Buffer*				m_pFontBuffer;
	UINT						m_FontBufferBytes;
	CGrowableArray<DXUTSpriteVertex> m_FontVertices;
	ID3D11ShaderResourceView*	m_pFontSRV;

	// UI States
	ID3D11DepthStencilState*	m_pDepthStencilStateUI;
	ID3D11RasterizerState*		m_pRasterizerStateUI;
	ID3D11BlendState*			m_pBlendStateUI;
    ID3D11SamplerState*			m_pSamplerStateUI;

	// UI Shaders
	ID3D11VertexShader*			m_pVSRenderUI;
	ID3D11PixelShader*			m_pPSRenderUI;
	ID3D11InputLayout*			m_pInputLayout11;

	void		FillVertex( RENDER_TEXT_BUFFER& text, float screenWidth, float screenHeight );
	void		RenderText( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext );

public:
	RDX11FontRenderer();

	HRESULT		Init(const char* fontDDS, ID3D11Device* pd3dDevice);
	void		Destroy();

	void		ApplyRenderState( ID3D11DeviceContext* pd3dImmediateContext );
	void		Draw(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, RENDER_TEXT_BUFFER& text, int sw, int sh );
};