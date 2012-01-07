#include "RDX11Device.h"


namespace GLOBAL
{
	ID3D11Device*			g_D3Device = NULL;
	ID3D11DeviceContext*	g_D3DeviceContext = NULL;
	RDX11Device*			g_RDX11Device = NULL;
	RDX11Setting			g_DeviceSetting;
	RDX11FontRenderer		g_FontRenderer;
	RDX11RenderStateMgr		g_StateRepository;
	RDX11Window				g_MainWindow;
	RDX11RenderHelper		g_RenderHelper;
	RDX11ShaderMgr			g_ShaderMgr;
	CCAMERA_DESC			g_pCurrentCameraDesc;

	ID3D11Device*			GetD3DDevice()		{ return g_D3Device;}
	ID3D11DeviceContext*	GetD3DContext()		{ return g_D3DeviceContext; }
	RDX11Device*			GetRDX11Device()	{ return g_RDX11Device; }
	RDX11RenderStateMgr*	GetD3DStateMgr()	{ return &g_StateRepository; }
	const RDX11Setting&		GetDeviceInfo()		{ return g_DeviceSetting; }
	RDX11ShaderMgr*			GetShaderMgr()		{ return &g_ShaderMgr; }
	const CCAMERA_DESC&		GetCameraDesc()		{ return g_pCurrentCameraDesc; }
};

using namespace GLOBAL;

RDX11Device::RDX11Device()
	: m_pD3Device(NULL)
{

}

RDX11Device::~RDX11Device()
{

}

IRenderHelper* RDX11Device::GetRenderHelper()
{
	return &g_RenderHelper;
}

bool RDX11Device::StartUp(const CENGINE_INIT_PARAM &param)
{
	m_HWND = (HWND)param.hWnd;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = param.width;
	swapChainDesc.BufferDesc.Height = param.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = (HWND)param.hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = true;

	HRESULT hr = S_OK;
	ID3D11DeviceContext* pImmediateContext = NULL;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		g_DeviceSetting.driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( NULL, g_DeviceSetting.driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &g_MainWindow.pSwapChain, &m_pD3Device, &g_DeviceSetting.featureLevel, &m_pContext );
		if( SUCCEEDED( hr ) )
			break;
	}
	TDXERROR(hr);
	
	// set global variables
	g_D3Device = m_pD3Device;
	g_D3DeviceContext = m_pContext;
	g_RDX11Device = this;

	g_DeviceSetting.width = param.width;
	g_DeviceSetting.height = param.height;

	g_StateRepository.Init();
	g_MainWindow.Create(m_pD3Device);
	SetViewport( (float)g_DeviceSetting.width, (float)g_DeviceSetting.height);

	// initialize subsystem
	g_ShaderMgr.init();
	g_FontRenderer.SetFontFile( "Font.dds");

	return true;
}

void RDX11Device::ShutDown()
{
	g_RenderHelper.Destroy();
	g_FontRenderer.Destroy();
	g_MainWindow.Destroy();
	g_StateRepository.Destroy();
	g_ShaderMgr.Destroy();
	SAFE_RELEASE( m_pContext )
	SAFE_RELEASE( m_pD3Device )
}

void RDX11Device::SetViewport(float width, float height, float MinDepth, float MaxDepth, float TopLeftX, float TopLeftY)
{
	D3D11_VIEWPORT vp;

	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = MinDepth;
	vp.MaxDepth = MaxDepth;
	vp.TopLeftX = TopLeftX;
	vp.TopLeftY = TopLeftY;

	m_pContext->RSSetViewports( 1, &vp );
}

void RDX11Device::Render(const CCAMERA_DESC& cameraDesc)
{
	g_pCurrentCameraDesc = cameraDesc;

	m_pContext->OMSetRenderTargets( 1, &g_MainWindow.pRTV, g_MainWindow.pDSV );
	m_pContext->ClearRenderTargetView( g_MainWindow.pRTV, g_MainWindow.clearColor);
	m_pContext->ClearDepthStencilView( g_MainWindow.pDSV, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0, 0 );
}


void RDX11Device::RenderUI()
{
	g_StateRepository.StoreCurrentState();

	GLOBAL::GetShaderMgr()->BeginShader(EFFECT_FONT);
	
	RENDER_TEXT_BUFFER ff;
	wcscpy_s( ff.strMsg , L"Rotate model: Left mouse button\n");
	ff.rc.left = 0;
	ff.rc.top = 0;
	ff.rc.right = 100;
	ff.rc.bottom = 100;
	ff.clr = CColor( 1.0f, 1.0f, 1.0f, 1.0f );

	g_FontRenderer.Render( ff, g_DeviceSetting.width,  g_DeviceSetting.height );

	g_StateRepository.RestoreSavedState();
}

void RDX11Device::Present()
{
	g_MainWindow.Present();
}


bool RDX11Device::Resize(int width, int height)
{
	g_DeviceSetting.width = width;
	g_DeviceSetting.height = height;

	SetViewport( (float)g_DeviceSetting.width, (float)g_DeviceSetting.height);

	return g_MainWindow.Resize(m_pD3Device, width, height, false);
}

void RDX11Device::TS_CreateDPResource(DEVICE_DEPENDENT_RESOURCE type, void* pBuf ,int size, IResource* pResource)
{
	switch( type )
	{
	case DP_RESOURCE_VERTEX:		TS_CreateVB(pBuf, size, pResource); break;
	case DP_RESOURCE_VERTEX_OUT:	TS_CreateVBOut(pBuf, size, pResource);	break;
	case DP_RESOURCE_INDEX:			TS_CreateIB(pBuf, size, pResource);	break;
	case DP_RESOURCE_TEXTURE:		TS_CreateTexture(pBuf, size, pResource);	break;
	case DP_RESOURCE_SHADER:		TS_CreateShader(pBuf, size, pResource);	break;
	}
}

void RDX11Device::TS_CreateVB(void* pBuf ,int size, IResource* pResource)
{

}

void RDX11Device::TS_CreateVBOut(void* pBuf ,int size, IResource* pResource)
{

}

void RDX11Device::TS_CreateIB(void* pBuf ,int size, IResource* pResource)
{

}

void RDX11Device::TS_CreateTexture(void* pBuf ,int size, IResource* pResource)
{

}

void RDX11Device::TS_CreateShader(void* pBuf ,int size, IResource* pResource)
{

}
