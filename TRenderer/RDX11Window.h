#pragma once

#include "RDefine.h"
#include "CColor.h"


struct RDX11Window
{
	RDX11Window();
	~RDX11Window();

	IDXGISwapChain*				pSwapChain;
	ID3D11ShaderResourceView*	pSRV;
	ID3D11Texture2D*			pRT;
	ID3D11RenderTargetView*		pRTV;
	ID3D11DepthStencilView*		pDSV;
	CColor						clearColor;

	bool	Create(ID3D11Device* pD3Device);
	void	ReleaseTexture();
	bool	Resize(ID3D11Device* pD3Device, int cx, int cy, bool bFullScreen);
	void	Present();
	void	Destroy();
};