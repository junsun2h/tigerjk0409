#include "RDX11Window.h"



RDX11Window::RDX11Window()
	: pSwapChain(NULL)
{
}

RDX11Window::~RDX11Window()
{
	Destroy();
}

void RDX11Window::ReleaseTexture()
{
	SAFE_RELEASE(pSRV);
	SAFE_RELEASE(pDSV);
	SAFE_RELEASE(pRT);
	SAFE_RELEASE(pRTV);
}

void RDX11Window::Destroy()
{
	ReleaseTexture();
	SAFE_RELEASE(pSwapChain);
}

bool RDX11Window::Create(ID3D11Device* pD3Device)
{
	if( pSwapChain == NULL)
	{
		assert(0);
		return false;
	}
	
	TDXERROR( pSwapChain->GetBuffer( 0, __uuidof( *pRT ), ( LPVOID* )&pRT ) );
	TDXERROR( pD3Device->CreateRenderTargetView( pRT, NULL, &pRTV ) );
	SAFE_RELEASE( pRT );

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

	TDXERROR( pD3Device->CreateTexture2D( &descDepth, NULL, &pRT ) );

	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	// Create the depth stencil view
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	TDXERROR( pD3Device->CreateDepthStencilView( pRT, &depthStencilViewDesc, &pDSV ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	TDXERROR( pD3Device->CreateShaderResourceView( pRT, &shaderResourceViewDesc, &pSRV) );

	return true;
}

bool RDX11Window::Resize(ID3D11Device* pD3Device, int cx, int cy, bool bFullScreen)
{
	ReleaseTexture();

	// Alternate between 0 and DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH when resizing buffers.
	// When in windowed mode, we want 0 since this allows the app to change to the desktop
	// resolution from windowed mode during alt+enter.  However, in fullscreen mode, we want
	// the ability to change display modes from the Device Settings dialog.  Therefore, we
	// want to set the DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH flag.
	UINT Flags = 0;
	if( bFullScreen )
		Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_DESC desc;
	pSwapChain->GetDesc(&desc);

	// ResizeBuffers
	TDXERROR(  pSwapChain->ResizeBuffers( desc.BufferCount, cx, cy, desc.BufferDesc.Format, Flags ) );
	pSwapChain->GetDesc(&desc);

	return Create(pD3Device);
}

void RDX11Window::Present()
{
	pSwapChain->Present(0,0);
}