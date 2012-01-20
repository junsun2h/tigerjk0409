#pragma once

#include "RDefine.h"
#include "CColor.h"
#include "CResource.h"


struct RDX11MainFrameBuffer
{
	RDX11MainFrameBuffer();
	~RDX11MainFrameBuffer();

	IDXGISwapChain*				pSwapChain;
	ID3D11ShaderResourceView*	pSRV;
	ID3D11RenderTargetView*		pRTV;
	ID3D11DepthStencilView*		pDSV;
	CColor						clearColor;

	bool	Create();
	void	ReleaseTexture();
	bool	Resize(int cx, int cy, bool bFullScreen);
	void	Present();
	void	Destroy();

	CResourceTexture*			m_RenderTargets[3];
};
