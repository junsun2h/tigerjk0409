#pragma once


#include "RDefine.h"
#include "CGrowableArray.h"
#include <d3dx9math.h>


struct DXUTSpriteVertex
{
	CVector3 vPos;
	D3DXCOLOR vColor;
	CVector2 vTex;
};

struct DXUT_SCREEN_VERTEX_10
{
	float x, y, z;
	D3DCOLORVALUE color;
	float tu, tv;
};

struct RENDER_TEXT_BUFFER
{
	RECT		rc;
	WCHAR		strMsg[256];
	D3DXCOLOR	clr;
	bool		bCenter;
};

class RDX11Font
{
	ID3D11Buffer*				m_pFontBuffer;
	UINT						m_FontBufferBytes;
	CGrowableArray<DXUTSpriteVertex> m_FontVertices;
	ID3D11ShaderResourceView*	m_pFontSRV;

	// UI States
	ID3D11DepthStencilState*	m_pDepthStencilStateUI11;
	ID3D11RasterizerState*		m_pRasterizerStateUI11;
	ID3D11BlendState*			m_pBlendStateUI11;
    ID3D11SamplerState*			m_pSamplerStateUI11;

	// UI Shaders
	ID3D11VertexShader*			m_pVSRenderUI11;
	ID3D11PixelShader*			m_pPSRenderUI11;
	ID3D11InputLayout*			m_pInputLayout11;

	int							m_nLineHeight;

	void		FillVertex( RENDER_TEXT_BUFFER& text, float screenWidth, float screenHeight );
	void		RenderText( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext );

public:
	RDX11Font();

	HRESULT		Init(const char* fontDDS, ID3D11Device* pd3dDevice);
	void		Destroy();

	void		ApplyRenderState( ID3D11DeviceContext* pd3dImmediateContext );
	void		DrawText(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, RENDER_TEXT_BUFFER& text, float sw, float sh );
};