#include "CResource.h"
#include "CColor.h"

#include "IRDevice.h"
#include "IAssetMgr.h"

#include "RDX11Global.h"
#include "RDX11RenderTargetMgr.h"


RDX11RenderTargetMgr::RDX11RenderTargetMgr()
	: pSwapChain(NULL)
	, clearColor(0.25f, 0.25f, 0.55f, 1.0f)
{
	memset( m_DefferdRenderTargets, 0, 4*NUM_DEFFERED_RENDER_TARGET );
}

RDX11RenderTargetMgr::~RDX11RenderTargetMgr()
{
	Destroy();
}

void RDX11RenderTargetMgr::ReleaseTexture()
{
	SAFE_RELEASE(pDepthSRV);
	SAFE_RELEASE(pDSV);
	SAFE_RELEASE(pMainFrameRTV);
}

void RDX11RenderTargetMgr::Destroy()
{
	ReleaseTexture();
	SAFE_RELEASE(pSwapChain);
}

bool RDX11RenderTargetMgr::CreateMainFrameTarget()
{
	ID3D11Device* pD3Device = GLOBAL::D3DDevice();

	if( pSwapChain == NULL)
	{
		assert(0);
		return false;
	}


	ID3D11Texture2D* pMainFrameTexture;
	TDXERROR( pSwapChain->GetBuffer( 0, __uuidof( *pMainFrameTexture ), ( LPVOID* )&pMainFrameTexture ) );
	TDXERROR( pD3Device->CreateRenderTargetView( pMainFrameTexture, NULL, &pMainFrameRTV ) );
	SAFE_RELEASE( pMainFrameTexture );

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	pSwapChain->GetDesc(&swapChainDesc);

	//------------------------------------------------------------------------
	// Create depth stencil texture
	ID3D11Texture2D* pDepthTexture;

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = swapChainDesc.BufferDesc.Width;
	descDepth.Height = swapChainDesc.BufferDesc.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc = swapChainDesc.SampleDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	TDXERROR( pD3Device->CreateTexture2D( &descDepth, NULL, &pDepthTexture ) );

	/* only allow these formats
	DXGI_FORMAT_D16_UNORM
	DXGI_FORMAT_D24_UNORM_S8_UINT
	DXGI_FORMAT_D32_FLOAT
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT
	DXGI_FORMAT_UNKNOWN
	*/
	D3D11_DEPTH_STENCIL_VIEW_DESC	depthStencilViewDesc;
	// Create the depth stencil view
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	TDXERROR( pD3Device->CreateDepthStencilView( pDepthTexture, &depthStencilViewDesc, &pDSV ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	TDXERROR( pD3Device->CreateShaderResourceView( pDepthTexture, &shaderResourceViewDesc, &pDepthSRV) );
	SAFE_RELEASE( pDepthTexture );

	return true;
}

void RDX11RenderTargetMgr::CreateDefferedTarget(int cx, int cy)
{
	CreateRenderTarget( cx, cy, COLOR_FORMAT_R10G10B10A2_UNORM, RT_GPASS1, "GPassTarget1");
	CreateRenderTarget( cx, cy, COLOR_FORMAT_R8G8B8A8_UNORM, RT_GPASS3, "GPassTarget3");
	CreateRenderTarget( cx, cy, COLOR_FORMAT_R8G8B8A8_UNORM, RT_LPASS, "LPassTarget");
}

bool RDX11RenderTargetMgr::Resize(int cx, int cy, bool bFullScreen)
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

	if( !CreateMainFrameTarget() )
		return false;
	
	for( int i=0; i < NUM_DEFFERED_RENDER_TARGET; ++i)
	{
		GLOBAL::RDevice()->RemoveGraphicBuffer( m_DefferdRenderTargets[i] );
		m_DefferdRenderTargets[i]->Width = cx;
		m_DefferdRenderTargets[i]->height = cy;
		GLOBAL::RDevice()->CreateGraphicBuffer( m_DefferdRenderTargets[i] );
	}
	
	return true;
}

void RDX11RenderTargetMgr::Present()
{
	pSwapChain->Present(0,0);
}

void RDX11RenderTargetMgr::ClearAndSetMaineFrame()
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	pContext->OMSetRenderTargets( 1, &pMainFrameRTV, pDSV );
	pContext->ClearRenderTargetView( pMainFrameRTV, clearColor);
	pContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0, 0 );
}

void RDX11RenderTargetMgr::CreateRenderTarget(int width, int height, eTEXTURE_FORMAT format, eDEFFERED_RENDER_TARGET target, const char* name)
{
	IAssetMgr* pAssetMgr = GLOBAL::Engine()->AssetMgr();

	CResourceTexture* pRenderTargetTexture = (CResourceTexture*)pAssetMgr->CreateResource(RESOURCE_TEXTURE, name);
	pRenderTargetTexture->height = height;
	pRenderTargetTexture->Width = width;
	pRenderTargetTexture->usage = TEXTURE_RENDER_RAGET;
	pRenderTargetTexture->Format = format;
	pRenderTargetTexture->MipLevels = 1;

	GLOBAL::RDevice()->CreateGraphicBuffer(pRenderTargetTexture);

	m_DefferdRenderTargets[target] = pRenderTargetTexture;
}