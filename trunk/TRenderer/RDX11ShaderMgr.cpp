#include "RDX11ShaderMgr.h"
#include "simpleFx.h"
#include "RDX11Device.h"


RDX11ShaderMgr::RDX11ShaderMgr()
	: m_MemPoolShader(20)
{

}

RDX11ShaderMgr::~RDX11ShaderMgr()
{
}

//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::init()
{
	CreateFontShader();
	CreateLineShader();
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

	POSITION pos = m_ShaderMap.GetStartPosition();
	SHADER_MAP::CPair* itr = NULL;

	while (pos)
	{
		itr = m_ShaderMap.GetNext(pos);
		m_MemPoolShader.Remove(itr->m_value );
	}

	m_ShaderMap.RemoveAll();
}


//------------------------------------------------------------------------------------------------------------
RDX11Shader* RDX11ShaderMgr::GetShader(EFFECT_TYPE type)
{
	SHADER_MAP::CPair* pShader = m_ShaderMap.Lookup( type );
	if( pShader != NULL )
		return pShader->m_value;

	assert(0);
	return NULL;
}


//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::BeginShader(EFFECT_TYPE type)
{
	RDX11Shader* pShader = GetShader(type);
	pShader->Begin();
}


//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::CreateFontShader()
{
	SHADER_MAP::CPair* pShader = m_ShaderMap.Lookup( EFFECT_FONT );
	if( pShader != NULL )
		return;

	RDX11Shader* pFontShader = m_MemPoolShader.GetNew();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R16G16_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	pFontShader->CreateVS(g_strFontFxFile, sizeof(g_strFontFxFile), 
						"VS", "vs_4_0_level_9_1", 
						D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY,
						layout, ARRAYSIZE( layout ), "FontVS" );

	pFontShader->CreatePS(g_strFontFxFile, sizeof(g_strFontFxFile), 
						"PS", "ps_4_0_level_9_1", 
						D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY, "FontPS");

	pFontShader->SetRenderState(DEPTH_STENCIL_OFF, RASTERIZER_CULL_BACK, BLEND_ADD_BY_ALPHA);
	
	pFontShader->SetTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	m_ShaderMap.SetAt( EFFECT_FONT, pFontShader );
}


//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::CreateLineShader()
{
	SHADER_MAP::CPair* pShader = m_ShaderMap.Lookup( EFFECT_LINE );
	if( pShader != NULL )
		return;

	RDX11Shader* pLineShader = m_MemPoolShader.GetNew();

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	pLineShader->CreateVS(g_strLineEffectFile, sizeof(g_strLineEffectFile), 
							"VS", "vs_4_0_level_9_1", 
							D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY,
							layout, ARRAYSIZE( layout ), "LineVS" );

	pLineShader->CreatePS(g_strLineEffectFile, sizeof(g_strLineEffectFile), 
							"PS", "ps_4_0_level_9_1", 
							D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY, "LinePS");

	pLineShader->SetRenderState(DEPTH_STENCIL_ON, RASTERIZER_CULL_BACK, BLEND_NONE);

	pLineShader->SetTopology( D3D_PRIMITIVE_TOPOLOGY_LINELIST );

	m_ShaderMap.SetAt( EFFECT_LINE, pLineShader );
}


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


void RDX11ShaderMgr::UpdateShaderConstant(void* pScr, size_t size, SHADER_CONST_BUFFER_SLOT slot, SHADER_TYPE type)
{
	HRESULT hr = S_OK;
	byte pData[256];

	memcpy( pData, pScr, size );

	ID3D11Buffer* pDXBuffer = NULL;
	ID3D11DeviceContext* pContext = GLOBAL::GetD3DContext();

	CONST_BUFFER_MAP::CPair* pBuffer = m_ConstBufferMap[type].Lookup( slot );
	if( pBuffer == NULL )
	{
		GLOBAL::GetRDX11Device()->RecreateBuffer( &pDXBuffer, pData, GetDXBufSize(slot), D3D11_BIND_CONSTANT_BUFFER );
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

