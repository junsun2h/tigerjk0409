#pragma once

#include "RDX11Texture.h"


struct RDX11Window
{
	RDX11Window();
	~RDX11Window();

	IDXGISwapChain*			m_pSwapChain;
	RDX11RenderTexture*		m_pRenderTarget;
	RDX11RenderTexture*		m_pDepthStencil;

	bool	Create(ID3D11Device* pD3Device);
	void	Release();
	bool	Resize(ID3D11Device* pD3Device, int cx, int cy, bool bFullScreen);
	void	Present();
};

