#pragma once

#include "RDefine.h"
#include "RDX11Resource.h"

struct RDX11Texture
{
	RDX11Texture();
	virtual ~RDX11Texture();
	virtual void Release();

	RDX11_SOURCELOCATION	m_Location;
	WCHAR					m_wszSource[MAX_PATH];
	HMODULE					m_hSrcModule;
	UINT					m_Width;
	UINT					m_Height;
	UINT					m_Depth;
	UINT					m_MipLevels;
	UINT					m_MiscFlags;

	D3D11_USAGE				m_Usage11;
	DXGI_FORMAT				m_Format;
	UINT					m_CpuAccessFlags;
	UINT					m_BindFlags;

	ID3D11ShaderResourceView*	m_pSRV;
};


struct RDX11RenderTexture : public RDX11Texture
{
public:
	RDX11RenderTexture();
	virtual ~RDX11RenderTexture();
	virtual void Release() override;

	ID3D11Texture2D*			m_pRT;
	ID3D11RenderTargetView*		m_pRTV;
	ID3D11DepthStencilView*		m_pDSV;
};



bool RDX11_CREATE_SWAP_CHAIN_RENDERTARGET(RDX11RenderTexture* pTexture, ID3D11Device* pDevice, IDXGISwapChain* pSwapChain);
bool RDX11_CREATE_SWAP_CHAIN_DEPTH_STENCIL(RDX11RenderTexture* pTexture, ID3D11Device* pDevice, IDXGISwapChain* pSwapChain);
