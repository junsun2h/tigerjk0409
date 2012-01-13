#pragma once

#include "RDefine.h"
#include "CColor.h"
#include "IResource.h"


enum RENDER_TARGET_SLOT
{
	RENDER_TARGET_GEOMERTY,
	RENDER_TARGET_LIGHT
};

struct RDX11MainFrameBuffer
{
	RDX11MainFrameBuffer();
	~RDX11MainFrameBuffer();

	IDXGISwapChain*				pSwapChain;
	ID3D11ShaderResourceView*	pSRV;
	ID3D11RenderTargetView*		pRTV;
	ID3D11DepthStencilView*		pDSV;
	CColor						clearColor;

	bool	Create(ID3D11Device* pD3Device);
	void	ReleaseTexture();
	bool	Resize(ID3D11Device* pD3Device, int cx, int cy, bool bFullScreen);
	void	Present();
	void	Destroy();

	CResourceTexture*			m_RenderTargets[3];
};


class RDX11RenderTargetMgr
{
	bool				Resize(ID3D11Device* pD3Device, int cx, int cy, bool bFullScreen);
	void				SetRenderTarget(RENDER_TARGET_SLOT slot, CResourceTexture* pTexture);
	CResourceTexture*	GetRenderTarget(RENDER_TARGET_SLOT slot);

private:
	typedef	ATL::CAtlMap<RENDER_TARGET_SLOT, CResourceTexture*>	RENDER_TARGET_MAP;

	RENDER_TARGET_MAP	m_RenderTargetS;
};