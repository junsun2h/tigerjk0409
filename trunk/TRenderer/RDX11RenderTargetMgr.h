#pragma once


struct RDX11RenderTargetMgr : public IRenderTargetMgr
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
	void	ClearAndSetMaineFrame() override;

	CResourceTexture*			m_DefferdRenderTargets[NUM_DEFFERED_RENDER_TARGET];
};
