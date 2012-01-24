#include "RDX11Device.h"


namespace GLOBAL
{
	ID3D11Device*			g_D3Device = NULL;
	ID3D11DeviceContext*	g_D3DeviceContext = NULL;
	RDX11Device*			g_RDX11Device = NULL;
	RDeviceDesc				g_DeviceSetting;
	RDX11RenderStateMgr		g_StateRepository;
	RDX11MainFrameBuffer	g_MainWindow;
	RDX11RenderHelper		g_RenderHelper;
	RDX11ShaderMgr			g_ShaderMgr;
	CCAMERA_DESC			g_pCurrentCameraDesc;
	IEngine*				g_pEngine = NULL;
	
	IEngine*				Engine()			{ return g_pEngine; }
	ID3D11Device*			GetD3DDevice()		{ return g_D3Device;}
	ID3D11DeviceContext*	GetD3DContext()		{ return g_D3DeviceContext; }
	RDX11Device*			GetRDX11Device()	{ return g_RDX11Device; }
	RDX11RenderStateMgr*	GetD3DStateMgr()	{ return &g_StateRepository; }
	const RDeviceDesc&		GetDeviceInfo()		{ return g_DeviceSetting; }
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

RDeviceDesc	RDX11Device::GetDeviceSetting()
{
	return GLOBAL::GetDeviceInfo();
}


//----------------------------------------------------------------------------------------------------------
bool RDX11Device::StartUp(const CENGINE_INIT_PARAM &param, IEngine* pEngine)
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
		hr = D3D11CreateDeviceAndSwapChain( NULL, 
											driverTypes[driverTypeIndex], 
											NULL, 
											createDeviceFlags, 
											featureLevels, 
											numFeatureLevels,
											D3D11_SDK_VERSION, 
											&swapChainDesc, 
											&g_MainWindow.pSwapChain, 
											&m_pD3Device, 
											&m_FeatureLevel, 
											&m_pContext );
		if( SUCCEEDED( hr ) )
			break;
	}
	TDXERROR(hr);
	
	// set global variables
	g_D3Device = m_pD3Device;
	g_D3DeviceContext = m_pContext;
	g_RDX11Device = this;
	g_pEngine = pEngine;

	g_DeviceSetting.width = param.width;
	g_DeviceSetting.height = param.height;

	g_StateRepository.Init();
	g_MainWindow.Create();
	SetViewport( (float)g_DeviceSetting.width, (float)g_DeviceSetting.height);

	// initialize subsystem
	g_ShaderMgr.init();
	g_RenderHelper.Init( "Font.dds");

	return true;
}


//----------------------------------------------------------------------------------------------------------
void RDX11Device::ShutDown()
{
	g_RenderHelper.Destroy();
	g_MainWindow.Destroy();
	g_StateRepository.Destroy();
	g_ShaderMgr.Destroy();
	SAFE_RELEASE( m_pContext )
	SAFE_RELEASE( m_pD3Device )
}


//----------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------
void RDX11Device::Render(const CCAMERA_DESC& cameraDesc)
{
	g_pCurrentCameraDesc = cameraDesc;

	m_pContext->OMSetRenderTargets( 1, &g_MainWindow.pRTV, g_MainWindow.pDSV );
	m_pContext->ClearRenderTargetView( g_MainWindow.pRTV, g_MainWindow.clearColor);
	m_pContext->ClearDepthStencilView( g_MainWindow.pDSV, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0, 0 );
}

//----------------------------------------------------------------------------------------------------------
void RDX11Device::Present()
{
	g_MainWindow.Present();
}


//----------------------------------------------------------------------------------------------------------
bool RDX11Device::Resize(int width, int height)
{
	g_DeviceSetting.width = width;
	g_DeviceSetting.height = height;

	SetViewport( (float)g_DeviceSetting.width, (float)g_DeviceSetting.height);

	return g_MainWindow.Resize(width, height, false);
}

//----------------------------------------------------------------------------------------------------------
void RDX11Device::PT_CreateGraphicBuffer(CResourceBase* pResource)
{
	CreateGraphicBuffer( pResource);
}

//----------------------------------------------------------------------------------------------------------
void RDX11Device::CreateGraphicBuffer(CResourceBase* pResource)
{
	eRESOURCE_TYPE type = pResource->Type();

	if( type == RESOURCE_GEOMETRY )
	{
		CResourceGeometry* pGeometry = (CResourceGeometry*)pResource;

		if( pGeometry->vertexCount <= 0)
			return;

		size_t size = VERTEX_STRIDE(pGeometry->eVertexType) * pGeometry->vertexCount;
		pGeometry->pGraphicMemoryVertexBuffer = CreateBuffer(pGeometry->pVertexBuffer, size, D3D11_BIND_VERTEX_BUFFER);

		if( pGeometry->primitiveCount > 0)
		{
			if( pGeometry->eIndexType == INDEX_16BIT_TYPE )
				pGeometry->pGraphicMemoryIndexBuffer = CreateBuffer(pGeometry->pIndexBuffer, 2 * 3 * pGeometry->primitiveCount, D3D11_BIND_INDEX_BUFFER);
			else
				pGeometry->pGraphicMemoryIndexBuffer = CreateBuffer(pGeometry->pIndexBuffer, 4 * 3 * pGeometry->primitiveCount, D3D11_BIND_INDEX_BUFFER);
		}
	}
	else if( type == RESOURCE_TEXTURE)
	{
		CResourceTexture* pTexture = (CResourceTexture*)pResource;

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));
		textureDesc.Width = pTexture->Width;
		textureDesc.Height = pTexture->height;
		textureDesc.MipLevels = pTexture->MipLevels;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT( pTexture->Format );
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		if( pTexture->usage == TEXTURE_RENDER_RAGET )
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		else
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		ID3D11Texture2D* pRT = (ID3D11Texture2D*)pTexture->pTextureSource;
		if( pRT == NULL )
			m_pD3Device->CreateTexture2D(&textureDesc, NULL, &pRT);

		if( pTexture->usage == TEXTURE_RENDER_RAGET )
		{
			ID3D11RenderTargetView*	pRTV;

			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			renderTargetViewDesc.Format = textureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetViewDesc.Texture2D.MipSlice = 0;

			m_pD3Device->CreateRenderTargetView( pRT, &renderTargetViewDesc, &pRTV);
			pTexture->pRenderTargetView = pRTV;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		ID3D11ShaderResourceView* pSRV;
		m_pD3Device->CreateShaderResourceView(pRT, &shaderResourceViewDesc, &pSRV);
		pTexture->pShaderResourceView = pSRV;

		if( pTexture->bDeleteMemoryAfterLoading )
		{
			SAFE_RELEASE(pRT)
		}
		else
			pTexture->pTextureSource = pRT;
	}
}

void RDX11Device::RemoveGraphicBuffer(CResourceBase* pResource)
{
	eRESOURCE_TYPE type = pResource->Type();

	if( type == RESOURCE_GEOMETRY )
	{
		CResourceGeometry* pGeometry = (CResourceGeometry*)pResource;

		if( pGeometry->pGraphicMemoryVertexBuffer != NULL )
		{
			ID3D11Buffer* pBuffer = (ID3D11Buffer*)pGeometry->pGraphicMemoryVertexBuffer;
			pBuffer->Release();
			pGeometry->pGraphicMemoryVertexBuffer = NULL;
		}

		if( pGeometry->pGraphicMemoryIndexBuffer != NULL )
		{
			ID3D11Buffer* pBuffer = (ID3D11Buffer*)pGeometry->pGraphicMemoryIndexBuffer;
			pBuffer->Release();
			pGeometry->pGraphicMemoryIndexBuffer = NULL;
		}

		if( pGeometry->pGraphicMemoryVertexBufferOut != NULL )
		{
			ID3D11Buffer* pBuffer = (ID3D11Buffer*)pGeometry->pGraphicMemoryVertexBufferOut;
			pBuffer->Release();
			pGeometry->pGraphicMemoryVertexBufferOut = NULL;
		}
	}
	else if( type == RESOURCE_TEXTURE )
	{
		CResourceTexture* pTexture = (CResourceTexture*)pResource;

		if( pTexture->pRenderTargetView != NULL )
		{
			ID3D11RenderTargetView* pRTV = (ID3D11RenderTargetView*)pTexture->pRenderTargetView;
			pRTV->Release();
			pTexture->pRenderTargetView = NULL;
		}

		if( pTexture->pShaderResourceView != NULL )
		{
			ID3D11ShaderResourceView* pSRV = (ID3D11ShaderResourceView*)pTexture->pShaderResourceView;
			pSRV->Release();
			pTexture->pShaderResourceView = NULL;
		}

		if( pTexture->pTextureSource != NULL )
		{
			ID3D11Resource* pResource = (ID3D11Resource*)pTexture->pTextureSource;
			pResource->Release();
			pTexture->pTextureSource = NULL;
		}
	}
}


//----------------------------------------------------------------------------------------------------------
ID3D11Buffer* RDX11Device::CreateBuffer(void* pData ,int size, UINT bindFlag, D3D11_USAGE usage )
{
	ID3D11Buffer* pBuffer = NULL;
	
	RecreateBuffer( &pBuffer, pData, size, bindFlag, usage);

	return pBuffer;
}

void RDX11Device::RecreateBuffer(ID3D11Buffer** ppBuffer, void* pData ,int size, UINT bindFlag, D3D11_USAGE usage)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = usage;
	bd.ByteWidth = size;
	bd.BindFlags = bindFlag;

	if( usage == D3D11_USAGE_STAGING )
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	else
		bd.CPUAccessFlags = 0;
	
	if( pData != NULL)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem = pData;
		TDXERROR( m_pD3Device->CreateBuffer( &bd, &InitData, ppBuffer ) );
	}
	else
	{
		TDXERROR( m_pD3Device->CreateBuffer( &bd, NULL, ppBuffer ) );
	}
}

CResourceTexture* RDX11Device::CreateTextureFromFile(const char* fileName)
{
	HRESULT hr = S_OK;
	ID3D11Resource *pDXTexture = NULL;

	hr = D3DX11CreateTextureFromFileA( m_pD3Device, fileName, NULL, NULL, &pDXTexture, NULL );

	if( FAILED( hr ) )
		return NULL;

	ID3D11Texture2D* pRT = (ID3D11Texture2D*)pDXTexture;

	D3D11_TEXTURE2D_DESC desc;
	pRT->GetDesc(&desc);

	CResourceTexture* pTexture = (CResourceTexture*)GLOBAL::Engine()->EngineMemoryMgr()->GetNewResource(RESOURCE_TEXTURE);
	pTexture->pTextureSource = pDXTexture;
	pTexture->Width = desc.Width;
	pTexture->height = desc.Height;
	pTexture->MipLevels = desc.MipLevels;
	pTexture->Format = eTEXTURE_FORMAT(desc.Format);

	CreateGraphicBuffer(pTexture);

	return pTexture;
}

bool RDX11Device::SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName)
{
	ID3D11Resource* pResource = (ID3D11Resource*)pTexture->pTextureSource;

	HRESULT hr = S_OK;

	hr = D3DX11SaveTextureToFileA( GLOBAL::GetD3DContext(), pResource,  D3DX11_IMAGE_FILE_FORMAT(format), fileName );

	if( FAILED(hr) )
	{
		assert(0);
		return false;
	}

	return true;
}
