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
		m_pDeviceSetting.m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain( NULL, m_pDeviceSetting.m_DriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &m_MainWindow.pSwapChain, &m_pD3Device, &m_pDeviceSetting.m_FeatureLevel, m_pImmediateContext.GetContextP() );
		if( SUCCEEDED( hr ) )
			break;
	}
	TDXERROR(hr);

	m_StateRepository.CreateStates(m_pD3Device);

	m_MainWindow.Create(m_pD3Device);
	m_pImmediateContext.SetDefaultState(&m_StateRepository);

	return true;
}

void RDX11Device::ShutDown()
{
	m_MainWindow.ReleaseTexture();
	m_StateRepository.Destroy();
	m_pImmediateContext.Destroy();
	m_pD3Device->Release();
}

void RDX11Device::Render(uint32 index)
{
	m_pImmediateContext.SetTarget( &m_MainWindow );

	m_MainWindow.Present();
}

bool RDX11Device::Resize(const CENGINE_INIT_PARAM &param)
{
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
