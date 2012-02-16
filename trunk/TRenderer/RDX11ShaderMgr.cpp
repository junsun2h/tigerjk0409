#include <atlcoll.h>
#include "RDefine.h"
#include "CResource.h"

#include "IEntityProxy.h"
#include "IShader.h"
#include "IRDX11Device.h"

#include "RDX11Global.h"
#include "RDX11Shader.h"
#include "RDX11ShaderLine.h"
#include "RDX11ShaderFont.h"
#include "RDX11ShaderGPassLambert.h"
#include "RDX11ShaderMPassMesh.h"

#include "RDX11ShaderMgr.h"


RDX11ShaderMgr::RDX11ShaderMgr()
	: m_pCurrentShader(NULL)
{

}

RDX11ShaderMgr::~RDX11ShaderMgr()
{
}

//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::init()
{
	m_ShaderMap.SetAt( EFFECT_FONT, new RDX11ShaderFont);
	m_ShaderMap.SetAt( EFFECT_LINE, new RDX11ShaderLine);
	m_ShaderMap.SetAt( EFFECT_GPASS_LAMBERT, new RDX11ShaderGPassLambert);
	m_ShaderMap.SetAt( EFFECT_MPASS_MESH, new RDX11ShaderMPassMesh );
}


//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::Destroy()
{
	for( int i=0; i < NUM_SHADER_TYPE; ++i)
	{
		POSITION pos = m_ConstBufferMap[i].GetStartPosition();
		CONST_BUFFER_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_ConstBufferMap[i].GetNext(pos);
			SAFE_RELEASE( itr->m_value );
		}		
	}

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
IShader* RDX11ShaderMgr::GetShader(EFFECT_TYPE type)
{
	SHADER_MAP::CPair* pShader = m_ShaderMap.Lookup( type );
	if( pShader != NULL )
		return pShader->m_value;

	return NULL;
}


//------------------------------------------------------------------------------------------------------------
UINT RDX11ShaderMgr::GetDXBufSize(SHADER_CONST_BUFFER_SLOT slot)
{
	switch(slot)
	{
	case SM_BUF0_16BYTE_SLOT0: return 16;		
	case SM_BUF1_16BYTE_SLOT1: return 16;
	case SM_BUF2_16BYTE_SLOT2: return 16;
	case SM_BUF3_32BYTE_SLOT0: return 32;		
	case SM_BUF4_32BYTE_SLOT1: return 32;
	case SM_BUF5_32BYTE_SLOT2: return 32;
	case SM_BUF6_64BYTE_SLOT0: return 64;		
	case SM_BUF7_64BYTE_SLOT1: return 64;
	case SM_BUF8_64BYTE_SLOT2: return 64;
	case SM_BUF9_128BYTE_SLOT0: return 128;	
	case SM_BUF10_128BYTE_SLOT1: return 128;
	case SM_BUF11_192BYTE_SLOT0: return 192;
	case SM_BUF12_192BYTE_SLOT1: return 192;
	case SM_BUF13_256BYTE_SLOT0: return 256;
	default:
		assert(0);
	}

	return 0;
}


//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::UpdateShaderConstant(void* pScr, size_t size, SHADER_CONST_BUFFER_SLOT slot, SHADER_TYPE type)
{
	HRESULT hr = S_OK;
	byte pData[256];

	memcpy( pData, pScr, size );

	ID3D11Buffer* pDXBuffer = NULL;
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	CONST_BUFFER_MAP::CPair* pBuffer = m_ConstBufferMap[type].Lookup( slot );
	if( pBuffer == NULL )
	{
		GLOBAL::RDevice()->RecreateBuffer( &pDXBuffer, pData, GetDXBufSize(slot), D3D11_BIND_CONSTANT_BUFFER );
		m_ConstBufferMap[type].SetAt( slot, pDXBuffer);
	}
	else
	{
		pDXBuffer = pBuffer->m_value;
		pContext->UpdateSubresource( pDXBuffer, 0, NULL, pData, 0, 0 );
	}

	if( type == VS_SHADER )
		pContext->VSSetConstantBuffers( slot, 1, &pDXBuffer );
	else if( type == GS_SHADER )
		pContext->GSSetConstantBuffers( slot, 1, &pDXBuffer );
	else if( type == PS_SHADER )
		pContext->PSSetConstantBuffers( slot, 1, &pDXBuffer );
}


void RDX11ShaderMgr::UpdateShaderResourceView(CResourceMtrl* pMtrl, eTEXTURE_TYPE textureType)
{
	IAssetMgr* pAssetMgr = GLOBAL::Engine()->AssetMgr();
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();
}
