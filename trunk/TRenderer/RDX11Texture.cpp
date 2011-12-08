#include "RDX11Texture.h"


RDX11Texture::RDX11Texture()
	: m_pSRV(NULL)
{
	Release();
}

RDX11Texture::~RDX11Texture()
{
	Release();
}

void RDX11Texture::Release()
{
	SAFE_RELEASE(m_pSRV);
}

RDX11RenderTexture::RDX11RenderTexture()
	: m_pRT(NULL)
	, m_pRTV(NULL)
	, m_pDSV(NULL)
{
}

RDX11RenderTexture::~RDX11RenderTexture()
{
	Release();
}

void RDX11RenderTexture::Release()
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pDSV);
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pRT);
}

bool RDX11_CREATE_SWAP_CHAIN_RENDERTARGET(RDX11RenderTexture* pTexture, ID3D11Device* pDevice, IDXGISwapChain* pSwapChain)
{
	TDXERROR( pSwapChain->GetBuffer( 0, __uuidof( *pTexture->m_pRT ), ( LPVOID* )&pTexture->m_pRT ) );
	TDXERROR( pDevice->CreateRenderTargetView( pTexture->m_pRT, NULL, &pTexture->m_pRTV ) );
	SAFE_RELEASE( pTexture->m_pRT );

	return true;
}

bool RDX11_CREATE_SWAP_CHAIN_DEPTH_STENCIL(RDX11RenderTexture* pTexture, ID3D11Device* pDevice, IDXGISwapChain* pSwapChain)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	pSwapChain->GetDesc(&swapChainDesc);

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = swapChainDesc.BufferDesc.Width;
	descDepth.Height = swapChainDesc.BufferDesc.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	descDepth.SampleDesc = swapChainDesc.SampleDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	TDXERROR( pDevice->CreateTexture2D( &descDepth, NULL, &pTexture->m_pRT ) );

	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	// Create the depth stencil view
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	TDXERROR( pDevice->CreateDepthStencilView( pTexture->m_pRT, &depthStencilViewDesc, &pTexture->m_pDSV ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	TDXERROR( pDevice->CreateShaderResourceView( pTexture->m_pRT, &shaderResourceViewDesc, &pTexture->m_pSRV) );

	return true;
}
