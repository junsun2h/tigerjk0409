#include "CResource.h"
#include "CCamera.h"
#include "CEngineParam.h"

#include "RDefine.h"
#include "IRDevice.h"
#include "IShader.h"
#include "IRenderStateMgr.h"

#include "RDX11Global.h"
#include "RDX11RenderStrategyForward.h"

#include "RDX11Device.h"




RDX11Device::RDX11Device()
	: m_pCurrentRenderStrategy(NULL)
{
}

RDX11Device::~RDX11Device()
{

}

IRenderHelper* RDX11Device::GetRenderHelper()
{
	return GLOBAL::RenderHelper();
}

RDeviceDesc	RDX11Device::GetDeviceSetting()
{
	return GLOBAL::DeviceInfo();
}

//----------------------------------------------------------------------------------------------------------
bool RDX11Device::StartUp(const CENGINE_INIT_PARAM* pParam)
{
	m_HWND = (HWND)pParam->hWnd;
	return GLOBAL::StartUp(pParam);
}


//----------------------------------------------------------------------------------------------------------
void RDX11Device::ShutDown()
{
	GLOBAL::ShutDown();
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

	GLOBAL::D3DContext()->RSSetViewports( 1, &vp );
}

//----------------------------------------------------------------------------------------------------------
void RDX11Device::Present()
{
	GLOBAL::Present();
}


//----------------------------------------------------------------------------------------------------------
bool RDX11Device::Resize(int width, int height)
{
	return GLOBAL::Resize(width, height);
}

//----------------------------------------------------------------------------------------------------------
void RDX11Device::CreateGraphicBuffer(CResourceBase* pResource)
{
	eRESOURCE_TYPE type = pResource->Type();

	if( type == RESOURCE_GEOMETRY )
	{
		CreateDXGeometryBuffer( (CResourceGeometry*)pResource );
	}
	else if( type == RESOURCE_TEXTURE)
	{
		CreateDXTextureBuffer( (CResourceTexture*)pResource );
	}
}

void RDX11Device::CreateDXGeometryBuffer(CResourceGeometry* pGeometry)
{
	if( pGeometry->vertexCount <= 0 || pGeometry->pGraphicMemoryVertexBuffer != NULL )
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

void RDX11Device::CreateDXTextureBuffer(CResourceTexture* pTexture)
{
	if( pTexture->pShaderResourceView != NULL)
		return;

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
		GLOBAL::D3DDevice()->CreateTexture2D(&textureDesc, NULL, &pRT);

	if( pTexture->usage == TEXTURE_RENDER_RAGET )
	{
		ID3D11RenderTargetView*	pRTV;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		GLOBAL::D3DDevice()->CreateRenderTargetView( pRT, &renderTargetViewDesc, &pRTV);
		pTexture->pRenderTargetView = pRTV;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* pSRV;
	GLOBAL::D3DDevice()->CreateShaderResourceView(pRT, &shaderResourceViewDesc, &pSRV);
	pTexture->pShaderResourceView = pSRV;

	if( pTexture->bDeleteMemoryAfterLoading )
	{
		SAFE_RELEASE(pRT)
	}
	else
		pTexture->pTextureSource = pRT;
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
		GLOBAL::D3DDevice()->CreateBuffer( &bd, &InitData, &pBuffer );
	}
	else
	{
		GLOBAL::D3DDevice()->CreateBuffer( &bd, NULL, &pBuffer );
	}

	return pBuffer;
}

//--------------------------------------------------------------------------------------
HRESULT LoadTextureDataFromFile( BYTE* pData, size_t fileSize, DDS_HEADER** ppHeader, BYTE** ppBitData, UINT* pBitSize )
{
	// DDS files always start with the same magic number ("DDS ")
	DWORD dwMagicNumber = *( DWORD* )( pData );
	if( dwMagicNumber != DDS_MAGIC )
		return E_FAIL;

	DDS_HEADER* pHeader = reinterpret_cast<DDS_HEADER*>( pData + sizeof( DWORD ) );

	// Verify header to validate DDS file
	if( pHeader->dwSize != sizeof(DDS_HEADER) || pHeader->ddspf.dwSize != sizeof(DDS_PIXELFORMAT) )
		return E_FAIL;

	// Check for DX10 extension
	bool bDXT10Header = false;
	if ( (pHeader->ddspf.dwFlags & DDS_FOURCC)	&& (MAKEFOURCC( 'D', 'X', '1', '0' ) == pHeader->ddspf.dwFourCC) )
	{
		// Must be long enough for both headers and magic value
		if( fileSize < (sizeof(DDS_HEADER)+sizeof(DWORD)+sizeof(DDS_HEADER_DXT10)) )
			return E_FAIL;

		bDXT10Header = true;
	}

	// setup the pointers in the process request
	*ppHeader = pHeader;
	INT offset = sizeof( DWORD ) + sizeof( DDS_HEADER )
		+ (bDXT10Header ? sizeof( DDS_HEADER_DXT10 ) : 0);
	*ppBitData = pData + offset;
	*pBitSize = fileSize - offset;

	return S_OK;
}


void RDX11Device::CreateTextureFromMemory(BYTE* pData, size_t size, CResourceTexture* pTexture)
{
	DDS_HEADER* pHeader = NULL;
	BYTE* pBitData = NULL;
	UINT BitSize = 0;

	HRESULT hr = LoadTextureDataFromFile( pData, size, &pHeader, &pBitData, &BitSize);

	UINT iWidth = pHeader->dwWidth;
	UINT iHeight = pHeader->dwHeight;
	UINT iMipCount = pHeader->dwMipMapCount;
	if( 0 == iMipCount )
		iMipCount = 1;

	D3D11_TEXTURE2D_DESC desc;
	if ((  pHeader->ddspf.dwFlags & DDS_FOURCC )
		&& (MAKEFOURCC( 'D', 'X', '1', '0' ) == pHeader->ddspf.dwFourCC ) )
	{
		DDS_HEADER_DXT10* d3d10ext = (DDS_HEADER_DXT10*)( (char*)pHeader + sizeof(DDS_HEADER) );

		if ( d3d10ext->resourceDimension != D3D11_RESOURCE_DIMENSION_TEXTURE2D )
		{
			assert(0);
			return;
		}
		desc.ArraySize = d3d10ext->arraySize;
		desc.Format = d3d10ext->dxgiFormat;
	}
	else
	{
		desc.ArraySize = 1;
		desc.Format = GetDXGIFormat( pHeader->ddspf );

		if (pHeader->dwCubemapFlags != 0 || (pHeader->dwHeaderFlags & DDS_HEADER_FLAGS_VOLUME) )
		{
			// For now only support 2D textures, not cubemaps or volumes
			assert(0);
			return;
		}

		if( desc.Format == DXGI_FORMAT_UNKNOWN )
		{
			D3DFORMAT fmt = GetD3D9Format( pHeader->ddspf );

			// Swizzle some RGB to BGR common formats to be DXGI (1.0) supported
			switch( fmt )
			{
			case D3DFMT_X8R8G8B8:
			case D3DFMT_A8R8G8B8:
				{
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

					if ( BitSize >= 3 )
					{
						for( UINT i = 0; i < BitSize; i += 4 )
						{
							BYTE a = pBitData[i];
							pBitData[i] = pBitData[i + 2];
							pBitData[i + 2] = a;
						}
					}
				}
				break;

				// Need more room to try to swizzle 24bpp formats
				// Could also try to expand 4bpp or 3:3:2 formats

			default:
				assert(0);
				return;
			}
		}
	}
	
	// Create the texture
	desc.Width = iWidth;
	desc.Height = iHeight;
	desc.MipLevels = iMipCount;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA* pInitData = new D3D11_SUBRESOURCE_DATA[iMipCount * desc.ArraySize];

	UINT NumBytes = 0;
	UINT RowBytes = 0;
	UINT NumRows = 0;
	BYTE* pSrcBits = pBitData;

	UINT index = 0;
	for( UINT j = 0; j < desc.ArraySize; j++ )
	{
		UINT w = iWidth;
		UINT h = iHeight;
		for( UINT i = 0; i < iMipCount; i++ )
		{
			GetSurfaceInfo( w, h, desc.Format, &NumBytes, &RowBytes, &NumRows );
			pInitData[index].pSysMem = ( void* )pSrcBits;
			pInitData[index].SysMemPitch = RowBytes;
			++index;

			pSrcBits += NumBytes;
			w = w >> 1;
			h = h >> 1;
			if( w == 0 )
				w = 1;
			if( h == 0 )
				h = 1;
		}
	}

	ID3D11Texture2D* pTex2D = NULL;
	hr = GLOBAL::D3DDevice()->CreateTexture2D( &desc, pInitData, &pTex2D );

	SAFE_DELETE_ARRAY( pInitData );

	pTexture->pTextureSource = pTex2D;
	pTexture->Width = desc.Width;
	pTexture->height = desc.Height;
	pTexture->MipLevels = desc.MipLevels;
	pTexture->Format = eTEXTURE_FORMAT(desc.Format);
	CreateGraphicBuffer(pTexture);
}

void RDX11Device::CreateTextureFromFile(const char* fileName, CResourceTexture* pTexture)
{
	HRESULT hr = S_OK;
	ID3D11Resource *pDXTexture = NULL;

	hr = D3DX11CreateTextureFromFileA( GLOBAL::D3DDevice(), fileName, NULL, NULL, &pDXTexture, NULL );

	if( FAILED( hr ) )
	{
		assert(0);
		return;
	}

	ID3D11Texture2D* pRT = (ID3D11Texture2D*)pDXTexture;

	D3D11_TEXTURE2D_DESC desc;
	pRT->GetDesc(&desc);

	pTexture->pTextureSource = pDXTexture;
	pTexture->Width = desc.Width;
	pTexture->height = desc.Height;
	pTexture->MipLevels = desc.MipLevels;
	pTexture->Format = eTEXTURE_FORMAT(desc.Format);

	CreateGraphicBuffer(pTexture);
}


bool RDX11Device::SaveTextureToFile(const CResourceTexture* pTexture, eIMAGE_FILE_FORMAT format, const char* fileName)
{
	ID3D11Resource* pResource = (ID3D11Resource*)pTexture->pTextureSource;

	HRESULT hr = S_OK;

	hr = D3DX11SaveTextureToFileA( GLOBAL::D3DContext(), pResource,  D3DX11_IMAGE_FILE_FORMAT(format), fileName );

	if( FAILED(hr) )
	{
		assert(0);
		return false;
	}

	return true;
}

void RDX11Device::SetRenderStrategy(eRENDER_STRATEGY strategy)
{
	static RDX11RenderStrategeForward s_DefferedStrategy;

	if( strategy == RENDER_STRATEGY_FORWARD )
		m_pCurrentRenderStrategy = &s_DefferedStrategy;
}