#include <atlcoll.h>
#include "RDefine.h"
#include "CResource.h"

#include "IEntityProxy.h"
#include "IShader.h"
#include "IRDevice.h"

#include "RDX11Global.h"
#include "RDX11Shader.h"

#include "RDX11ShaderGPass.h"
#include "RDX11ShaderMPass.h"

#include "RDX11ShaderMgr.h"


RDX11ShaderMgr::RDX11ShaderMgr()
	: m_pCurrentVS(NULL)
	, m_pCurrentPS(NULL)
	, m_pCurrentGS(NULL)
{
	memset(m_ConstBuffer, 0, 4 * NUM_SHADER_TYPE * MAX_SHADER_CONSTANT_SLOT );
	memset(m_ConstBufferSize, 0, 4 * NUM_SHADER_TYPE * MAX_SHADER_CONSTANT_SLOT );
}

RDX11ShaderMgr::~RDX11ShaderMgr()
{
}

//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::init()
{
	// Geometry Pass
	m_ShaderMap.SetAt( GPASS_VS_LAMBERT, new RDX11VSGPassLambert);
	m_ShaderMap.SetAt( GPASS_VS_LAMBERT_WEIGHT, new RDX11VSGPassLambertWeight);
	m_ShaderMap.SetAt( GPASS_VS_NORMALMAP, new RDX11VSGPassNormalMap);
	m_ShaderMap.SetAt( GPASS_VS_NORMALMAP_WEIGHT, new RDX11VSGPassNormalMapWeight);

	m_ShaderMap.SetAt( GPASS_PS_LAMBERT, new RDX11PSGPassBase);

	// Material Pass
	m_ShaderMap.SetAt( MPASS_VS_COLOR, new RDX11VSMPassColor);
	m_ShaderMap.SetAt( MPASS_VS_FONT, new RDX11VSMPassFont);
	m_ShaderMap.SetAt( MPASS_PS_COLOR, new RDX11PSMPassColor);
	m_ShaderMap.SetAt( MPASS_PS_FONT, new RDX11PSMPassFont);
}

bool RDX11ShaderMgr::SetCurrentShader(IShader* pShader)
{
	if( pShader == NULL)
	{
		assert(0);
		return false;
	}

	if( pShader->ShaderType() == VERTEX_SHADER )
	{
		if( m_pCurrentVS == pShader )
			return false;

		m_pCurrentVS = pShader;
		pShader->Begin();
	}
	else if( pShader->ShaderType() == PIXEL_SHADER )
	{
		if( m_pCurrentPS == pShader )
			return false;

		m_pCurrentPS = pShader;
		pShader->Begin();
	}
	else if( pShader->ShaderType() == GEOMETRY_SHADER )
	{
		if( m_pCurrentGS == pShader )
			return false;

		m_pCurrentGS = pShader;
		pShader->Begin();
	}

	return true;
}

//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::Destroy()
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();
	pContext->VSSetShader( NULL, NULL, 0 );
	pContext->PSSetShader( NULL, NULL, 0 );
	pContext->GSSetShader( NULL, NULL, 0 );

	for( int i=0; i < NUM_SHADER_TYPE; ++i)
	{
		for( int slot = 0; slot < MAX_SHADER_CONSTANT_SLOT; slot++)
			SAFE_RELEASE( m_ConstBuffer[i][slot] );
	}

	memset(m_ConstBuffer, 0, 4 * NUM_SHADER_TYPE * MAX_SHADER_CONSTANT_SLOT );
	memset(m_ConstBufferSize, 0, 4 * NUM_SHADER_TYPE * MAX_SHADER_CONSTANT_SLOT );

	// clear shader
	{
		POSITION pos = m_ShaderMap.GetStartPosition();
		SHADER_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_ShaderMap.GetNext(pos);
			SAFE_DELETE( itr->m_value );
		}

		m_ShaderMap.RemoveAll();
	}
}


//------------------------------------------------------------------------------------------------------------
IShader* RDX11ShaderMgr::GetShader(eEFFECT_TYPE type)
{
	SHADER_MAP::CPair* pShader = m_ShaderMap.Lookup( type );
	if( pShader != NULL )
		return pShader->m_value;

	return NULL;
}


void CreateConstantBuffer(ID3D11Buffer** ppBuffer, void* pData ,int size, UINT bindFlag, D3D11_USAGE usage = D3D11_USAGE_DEFAULT)
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

//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::UpdateShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type)
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	if( m_ConstBuffer[type][slot] == NULL )
	{
		CreateConstantBuffer( &m_ConstBuffer[type][slot], pScr, size, D3D11_BIND_CONSTANT_BUFFER );
		m_ConstBufferSize[type][slot] = size;
	}
	else
	{
		if( size > m_ConstBufferSize[type][slot] )
		{
			SAFE_RELEASE( m_ConstBuffer[type][slot] );
			CreateConstantBuffer( &m_ConstBuffer[type][slot], pScr, size, D3D11_BIND_CONSTANT_BUFFER );
			m_ConstBufferSize[type][slot] = size;
		}
		else
		{
			byte pData[1024 * 10];	// 10k
			memcpy( pData, pScr, size );
			pContext->UpdateSubresource( m_ConstBuffer[type][slot], 0, NULL, pData, 0, 0 );
		}
	}

	if( type == VERTEX_SHADER )
		pContext->VSSetConstantBuffers( slot, 1, &m_ConstBuffer[type][slot] );
	else if( type == GEOMETRY_SHADER )
		pContext->GSSetConstantBuffers( slot, 1, &m_ConstBuffer[type][slot] );
	else if( type == PIXEL_SHADER )
		pContext->PSSetConstantBuffers( slot, 1, &m_ConstBuffer[type][slot] );
}


void RDX11ShaderMgr::UpdateShaderResourceView(CResourceMtrl* pMtrl, eTEXTURE_TYPE textureType)
{
}

void RDX11ShaderMgr::UpdateTexture(CResourceTexture* pTexture, UINT slot)
{
	if( pTexture == NULL)
	{
		ID3D11ShaderResourceView* pSRV = NULL;
		GLOBAL::D3DContext()->PSSetShaderResources( slot, 1, &pSRV );
	}
	else
		GLOBAL::D3DContext()->PSSetShaderResources( slot, 1, (ID3D11ShaderResourceView**)&pTexture->pShaderResourceView );
}
