#pragma once

#include "RDefine.h"
#include "CColor.h"
#include "CResource.h"

enum eDEFFERED_RENDER_TARGET
{
	RT_GPASS1,
	RT_GPASS2,
	RT_GPASS3,

	RT_LPASS,

	NUM_DEFFERED_RENDER_TARGET
};

struct RDX11RenderTargetMgr
{
	RDX11RenderTargetMgr();
	~RDX11RenderTargetMgr();

	IDXGISwapChain*				pSwapChain;
	ID3D11ShaderResourceView*	pDepthSRV;
	ID3D11RenderTargetView*		pMainFrameRTV;
	ID3D11DepthStencilView*		pDSV;
	CColor						clearColor;

	bool	CreateMainFrameTarget();
	void	CreateDefferedTarget(int cx, int cy);
	void	CreateRenderTarget(int width, int height, eTEXTURE_FORMAT format, eDEFFERED_RENDER_TARGET target, const char* name);
	void	ReleaseTexture();
	bool	Resize(int cx, int cy, bool bFullScreen);
	void	Present();
	void	Destroy();
	void	ClearAndSet();

	CResourceTexture*			m_DefferdRenderTargets[NUM_DEFFERED_RENDER_TARGET];
};
