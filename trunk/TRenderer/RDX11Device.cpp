#include "RDX11Device.h"

RDX11Device::RDX11Device()
	: m_pD3Device(NULL)
{

}

RDX11Device::~RDX11Device()
{

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
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		m_DeviceSetting.driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( NULL, m_DeviceSetting.driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &m_MainWindow.pSwapChain, &m_pD3Device, &m_DeviceSetting.featureLevel, m_ImmediateContext.GetContextP() );
		if( SUCCEEDED( hr ) )
			break;
	}
	TDXERROR(hr);

	m_DeviceSetting.width = param.width;
	m_DeviceSetting.height = param.height;

	m_StateRepository.CreateStates(m_pD3Device);

	m_MainWindow.Create(m_pD3Device);
	m_ImmediateContext.SetDefaultState(&m_StateRepository);
	m_Font.Init( "fff", m_pD3Device );

	return true;
}

void RDX11Device::ShutDown()
{
	m_Font.Destroy();
	m_ImmediateContext.Destroy();
	m_MainWindow.Destroy();
	m_StateRepository.Destroy();
	SAFE_RELEASE( m_pD3Device )
}

void RDX11Device::Render(uint32 index)
{
	m_ImmediateContext.SetTarget( &m_MainWindow );

	m_ImmediateContext.StoreCurrentState();
	m_Font.ApplyRenderState( m_ImmediateContext.GetContext() );

	RENDER_TEXT_BUFFER ff;
	m_Font.DrawText( m_pD3Device, m_ImmediateContext.GetContext(), ff, 10,  10 );

	m_ImmediateContext.RestoreSavedState();

	m_MainWindow.Present();
}

bool RDX11Device::Resize(const CENGINE_INIT_PARAM &param)
{
	m_DeviceSetting.width = param.width;
	m_DeviceSetting.height = param.height;

	return m_MainWindow.Resize(m_pD3Device, param.width, param.height, param.bFullScreen);
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
