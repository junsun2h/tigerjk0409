#include "CResource.h"
#include "CQuad.h"
#include "CCamera.h"
#include "CCamera.h"
#include "CEngineParam.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IRDevice.h"

#include "RDX11Global.h"
#include "RDX11Device.h"
#include "RDX11RenderStrategeDeffered.h"



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
bool RDX11Device::StartUp(const CENGINE_INIT_PARAM* pParam, IEngine* pEngine)
{
	m_HWND = (HWND)pParam->hWnd;
	return GLOBAL::StartUp(pParam, pEngine);
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
void RDX11Device::RenderFrame(CCAMERA_DESC* pCameraDesc)
{
	CCAMERA_DESC desc;

	desc = *pCameraDesc;
	desc.ProjTM._33 /= desc.farClip;
	desc.ProjTM._43 /= desc.farClip;

	GLOBAL::SetCameraDesc(&desc);

	// update global shader constant
	CCAMERA_DESC cameraConstant = desc;
	cameraConstant.ViewTM = XMMatrixTranspose( desc.ViewTM );
	cameraConstant.ProjTM = XMMatrixTranspose( desc.ProjTM );
	GLOBAL::ShaderMgr()->UpdateShaderConstant( &cameraConstant, sizeof( CCAMERA_DESC), SM_BUF12_192BYTE_SLOT1, PS_SHADER );

	m_pCurrentRenderStrategy->RenderScene();
}


//----------------------------------------------------------------------------------------------------------
void RDX11Device::RenderElement( CResourceGeometry*	pGeometry, CResourceMtrl* pMtrl, IEntityProxyRender* pRenderProxy)
{
	GLOBAL::ShaderMgr()->GetCurrentShader()->SetShaderContants( pMtrl );
	GLOBAL::ShaderMgr()->GetCurrentShader()->SetShaderContants( pRenderProxy->GetEntity()->GetWorldTM() );
	RenderGeometry(pGeometry);
}

void RDX11Device::RenderGeometry(CResourceGeometry*	pGeometry)
{
	if( pGeometry->loadState != RESOURCE_LOAD_FINISHED )
	{
		assert(0);
		return;
	}

	UINT offset[1] = { 0 };
	UINT stride[1];

	if( pGeometry->pGraphicMemoryVertexBufferOut != NULL)
	{
		stride[0] = VERTEX_STRIDE(pGeometry->eVertexType) + 4;
		GLOBAL::D3DContext()->IASetVertexBuffers( 0, 1, (ID3D11Buffer**)&pGeometry->pGraphicMemoryVertexBufferOut, stride, offset );
	}
	else
	{
		stride[0] = VERTEX_STRIDE(pGeometry->eVertexType);
		GLOBAL::D3DContext()->IASetVertexBuffers( 0, 1, (ID3D11Buffer**)&pGeometry->pGraphicMemoryVertexBuffer, stride, offset );
	}

	if( pGeometry->pIndexBuffer != NULL)
	{
		if( pGeometry->eIndexType == INDEX_16BIT_TYPE )
			GLOBAL::D3DContext()->IASetIndexBuffer( (ID3D11Buffer*)pGeometry->pGraphicMemoryIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
		else
			GLOBAL::D3DContext()->IASetIndexBuffer( (ID3D11Buffer*)pGeometry->pGraphicMemoryIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

		GLOBAL::D3DContext()->DrawIndexed( pGeometry->primitiveCount * 3, 0, 0 );
	}
}

//----------------------------------------------------------------------------------------------------------
void RDX11Device::Present()
{
	GLOBAL::Present();
}


//----------------------------------------------------------------------------------------------------------
bool RDX11Device::Resize(int width, int height)
{
	return GLOBAL::Resize(height, width);
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
		TDXERROR( GLOBAL::D3DDevice()->CreateBuffer( &bd, &InitData, ppBuffer ) );
	}
	else
	{
		TDXERROR( GLOBAL::D3DDevice()->CreateBuffer( &bd, NULL, ppBuffer ) );
	}
}

CResourceTexture* RDX11Device::CreateTextureFromFile(const char* fileName, CResourceTexture* pTexture)
{
	HRESULT hr = S_OK;
	ID3D11Resource *pDXTexture = NULL;

	hr = D3DX11CreateTextureFromFileA( GLOBAL::D3DDevice(), fileName, NULL, NULL, &pDXTexture, NULL );

	if( FAILED( hr ) )
		return NULL;

	ID3D11Texture2D* pRT = (ID3D11Texture2D*)pDXTexture;

	D3D11_TEXTURE2D_DESC desc;
	pRT->GetDesc(&desc);

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
	static RDX11RenderStrategeDeffered s_DefferedStrategy;

	if( strategy == RS_DEFFERED )
		m_pCurrentRenderStrategy = &s_DefferedStrategy;
}