#include "RDX11Window.h"



RDX11Window::RDX11Window()
	: m_pSwapChain(NULL)
	, m_pRenderTarget(NULL)
	, m_pDepthStencil(NULL)
{
}

RDX11Window::~RDX11Window()
{
	Release();
}

void RDX11Window::Release()
{
	SAFE_DELETE(m_pRenderTarget);
	SAFE_DELETE(m_pDepthStencil);
	SAFE_RELEASE(m_pSwapChain);
}

bool RDX11Window::Create(ID3D11Device* pD3Device)
{
	if( m_pSwapChain == NULL)
	{
		assert(0);
		return false;
	}
	
	SAFE_DELETE(m_pRenderTarget);
	SAFE_DELETE(m_pDepthStencil);

	m_pRenderTarget = new RDX11RenderTexture;
	m_pDepthStencil = new RDX11RenderTexture;

	RDX11_CREATE_SWAP_CHAIN_RENDERTARGET(m_pRenderTarget, pD3Device, m_pSwapChain);
	RDX11_CREATE_SWAP_CHAIN_DEPTH_STENCIL(m_pDepthStencil, pD3Device, m_pSwapChain);

	return true;
}

bool RDX11Window::Resize(ID3D11Device* pD3Device, int cx, int cy, bool bFullScreen)
{
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pDepthStencil);

	// Alternate between 0 and DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH when resizing buffers.
	// When in windowed mode, we want 0 since this allows the app to change to the desktop
	// resolution from windowed mode during alt+enter.  However, in fullscreen mode, we want
	// the ability to change display modes from the Device Settings dialog.  Therefore, we
	// want to set the DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH flag.
	UINT Flags = 0;
	if( bFullScreen )
		Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_DESC desc;
	m_pSwapChain->GetDesc(&desc);

	// ResizeBuffers
	TDXERROR(  m_pSwapChain->ResizeBuffers( desc.BufferCount, cx, cy, desc.BufferDesc.Format, Flags ) );
	m_pSwapChain->GetDesc(&desc);

	return Create(pD3Device);
}

void RDX11Window::Present()
{
	m_pSwapChain->Present(0,0);
}

