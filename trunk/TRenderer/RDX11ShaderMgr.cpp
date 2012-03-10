#include <atlcoll.h>
#include "RDefine.h"
#include "CResource.h"
#include "CRenderElement.h"

#include "IShader.h"
#include "IRDevice.h"

#include "RDX11Global.h"
#include "RDX11Shader.h"

#include "RDX11ShaderMgr.h"

//------------------------------------------------------------------------------------------------------------
void MakeShaderMacro(UINT flag, D3D10_SHADER_MACRO macroBuf[])
{
	UINT bufIndex = 0;
	
	if( flag & RENDER_FLAG_SKIN  )
	{
		macroBuf[bufIndex].Name = "_SKIN";
		macroBuf[bufIndex].Definition = "0";
		bufIndex++;
	}

	if( flag & RENDER_FLAG_BUMP )
	{
		macroBuf[bufIndex].Name = "_BUMP";
		macroBuf[bufIndex].Definition = "0";
		bufIndex++;
	}

	if( flag & RENDER_FLAG_LIGHT )
	{
		macroBuf[bufIndex].Name = "_LIGHT";
		macroBuf[bufIndex].Definition = "0";
		bufIndex++;
	}

	macroBuf[bufIndex].Name = NULL;
	macroBuf[bufIndex].Definition = NULL;
}


//------------------------------------------------------------------------------------------------------------
void GetVertexFormat(UINT flag, SHADER_COMPILE_DESC& desc)
{
	if( ( flag & RENDER_FLAG_SKIN ) &&	( flag &RENDER_FLAG_BUMP ) )
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT_4BT_4BW;
	}
	else if( flag & RENDER_FLAG_SKIN )
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT_4BW;
	}
	else if( flag & RENDER_FLAG_BUMP )
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT_4BT;
	}
	else 
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT;
	}

}

//------------------------------------------------------------------------------------------------------------
void MakeRenderState(UINT flag, RDX11PixelShader* pShader)
{
	GRAPHIC_DEVICE_DESC renderDesc;

	renderDesc.DepthStencil = DEPTH_STENCIL_WRITE;
	renderDesc.RasterizerState = RASTERIZER_CULL_BACK;

	if( flag & RENDER_FLAG_TRANSPARENT )	renderDesc.BlendState = BLEND_ADD_BY_ALPHA;
	else									renderDesc.BlendState = BLEND_NONE;

	pShader->SetRenderState( renderDesc );
}



//////////////////////////////////////////////////////////////////////////
//  RDX11ShaderMgr
//////////////////////////////////////////////////////////////////////////
RDX11ShaderMgr::RDX11ShaderMgr()
{
	memset(m_pCurrentShader, 0, 4 * NUM_SHADER_TYPE );
	memset(m_ConstBuffer, 0, 4 * NUM_SHADER_TYPE * MAX_SHADER_CONSTANT_SLOT );
	memset(m_ConstBufferSize, 0, 4 * NUM_SHADER_TYPE * MAX_SHADER_CONSTANT_SLOT );
}

RDX11ShaderMgr::~RDX11ShaderMgr()
{
}

//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::init()
{
	SHADER_COMPILE_DESC desc;
	GRAPHIC_DEVICE_DESC renderDesc;
	desc.szFileName = "Shader\\SpacialShaders.fx";

	{// font shader
		RDX11VertexShader* pVS = new RDX11VertexShader;
		RDX11PixelShader* pPS = new RDX11PixelShader;

		desc.szEntrypoint = "VS_FONT";
		desc.szShaderModel = "vs_4_0_level_9_3";
		desc.eVertexyType = FVF_3FP_1DC_2HT;

		pVS->CreateVS(desc);
		m_Shaders[VERTEX_SHADER].SetAt( RENDER_FLAG_FONT, pVS);

		desc.szEntrypoint = "PS_FONT";
		desc.szShaderModel = "ps_4_0_level_9_3";

		pPS->CreatePS(desc);

		renderDesc.DepthStencil = DEPTH_STENCIL_OFF;
		renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
		renderDesc.BlendState = BLEND_ADD_BY_ALPHA;

		pPS->SetRenderState(renderDesc);
		m_Shaders[PIXEL_SHADER].SetAt( RENDER_FLAG_FONT, pPS);
	}

	{// color helper mesh shader
		RDX11VertexShader* pVS = new RDX11VertexShader;
		RDX11PixelShader* pPS = new RDX11PixelShader;

		desc.szEntrypoint = "VS_COLOR";
		desc.szShaderModel = "vs_4_0_level_9_3";
		desc.eVertexyType = FVF_3FP_1DC;

		pVS->CreateVS(desc);
		m_Shaders[VERTEX_SHADER].SetAt( RENDER_FLAG_COLOR, pVS);

		desc.szEntrypoint = "PS_COLOR";
		desc.szShaderModel = "ps_4_0_level_9_3";

		pPS->CreatePS(desc);

		renderDesc.DepthStencil = DEPTH_STENCIL_ON;
		renderDesc.RasterizerState = RASTERIZER_CULL_BACK;
		renderDesc.BlendState = BLEND_NONE;

		pPS->SetRenderState(renderDesc);
		m_Shaders[PIXEL_SHADER].SetAt( RENDER_FLAG_COLOR, pPS);
	}
}

bool RDX11ShaderMgr::CheckAndSet(IShader* pShader)
{
	if( pShader == NULL)
	{
		assert(0);
		return false;
	}

	if( m_pCurrentShader[pShader->ShaderType()] == pShader )
		return false;

	m_pCurrentShader[pShader->ShaderType()] = pShader;
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
	for( UINT i=0; i < NUM_SHADER_TYPE; ++i)
	{
		POSITION pos = m_Shaders[i].GetStartPosition();
		SHADER_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_Shaders[i].GetNext(pos);
			SAFE_DELETE( itr->m_value );
		}

		m_Shaders[i].RemoveAll();
	}
}

//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::Begin(UINT flag)
{
	GetShader( flag , VERTEX_SHADER )->Begin();
	GetShader( flag , PIXEL_SHADER )->Begin();
}

//------------------------------------------------------------------------------------------------------------
IShader* RDX11ShaderMgr::GetShader(UINT flag, eSHADER_TYPE type)
{
	SHADER_MAP::CPair* pShader = m_Shaders[type].Lookup( flag );
	if( pShader != NULL )
		return pShader->m_value;

	return CreateShader(flag, type);
}


//------------------------------------------------------------------------------------------------------------
IShader* RDX11ShaderMgr::CreateShader(UINT flag, eSHADER_TYPE shaderType)
{
	D3D10_SHADER_MACRO macroBuf[10];
	SHADER_COMPILE_DESC desc;

	MakeShaderMacro( flag, macroBuf);
	GetVertexFormat( flag, desc);
	desc.shader_Macros = macroBuf;
	desc.szFileName = "Shader\\GPass.fx";
	
	if( shaderType == PIXEL_SHADER )
	{
		RDX11PixelShader* pShader = new RDX11PixelShader;

		desc.szEntrypoint = "PS";
		desc.szShaderModel = "ps_4_0_level_9_3";

		pShader->CreatePS(desc);
		MakeRenderState(flag, pShader);

		m_Shaders[PIXEL_SHADER].SetAt( flag, pShader);
		return pShader;
	}
	else if( shaderType == VERTEX_SHADER )
	{
		RDX11VertexShader* pShader = new RDX11VertexShader;

		desc.szEntrypoint = "VS";
		desc.szShaderModel = "vs_4_0_level_9_3";

		pShader->CreateVS(desc);

		m_Shaders[VERTEX_SHADER].SetAt( flag, pShader);
		return pShader;
	}

	return NULL;
}


//------------------------------------------------------------------------------------------------------------
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
void RDX11ShaderMgr::SetShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type)
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


//------------------------------------------------------------------------------------------------------------
void RDX11ShaderMgr::SetTexture(const CResourceTexture* pTexture, UINT slot)
{
	if( pTexture == NULL)
	{
		ID3D11ShaderResourceView* pSRV = NULL;
		GLOBAL::D3DContext()->PSSetShaderResources( slot, 1, &pSRV );
	}
	else
		GLOBAL::D3DContext()->PSSetShaderResources( slot, 1, (ID3D11ShaderResourceView**)&pTexture->pShaderResourceView );
}


//------------------------------------------------------------------------------------------------------------
bool RDX11ShaderMgr::CheckShader(CRenderElement* pRenderElement)
{
	pRenderElement->pVertexShader = GetShader( pRenderElement->flag , VERTEX_SHADER );
	pRenderElement->pPixelShader = GetShader( pRenderElement->flag , PIXEL_SHADER );

	if( pRenderElement->pVertexShader == NULL ||
		pRenderElement->pPixelShader == NULL )
	{
		assert(0);
		return false;
	}

	return true;
}
