#include <atlcoll.h>
#include "RDefine.h"
#include "CResource.h"
#include "CRenderElement.h"

#include "IRenderStateMgr.h"
#include "IShader.h"
#include "IRDevice.h"

#include "RDX11Global.h"

#include "RDX11ShaderMgr.h"

#define SHADER_FILE	"Shader\\TShader.fx"



void Compile( RDX11Shader* pShader)
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

#if defined( DEBUG ) || defined( _DEBUG )
	pShader->m_Desc.compileFlag1 |= D3DCOMPILE_DEBUG;
	pShader->m_Desc.compileFlag1 |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = NULL;
	if( pShader->m_Desc.szFileName == NULL)
	{
		V( D3DCompile( pShader->m_Desc.pSrc, 
						pShader->m_Desc.SrcDataSize, 
						"none", 
						(D3D10_SHADER_MACRO*)pShader->m_Desc.shader_Macros, 
						NULL, 
						pShader->m_Desc.szEntrypoint, 
						pShader->m_Desc.szShaderModel, 
						pShader->m_Desc.compileFlag1, 0, &pBlob, &pErrorBlob ) );
	}
	else
	{
		V( D3DX11CompileFromFileA( pShader->m_Desc.szFileName, 
									(D3D10_SHADER_MACRO*)pShader->m_Desc.shader_Macros, 
									NULL, 
									pShader->m_Desc.szEntrypoint, 
									pShader->m_Desc.szShaderModel, 
									pShader->m_Desc.compileFlag1, 0, NULL, &pBlob, &pErrorBlob, NULL ) );
	}
	if( pErrorBlob ) 
		pErrorBlob->Release();

	if( pShader->m_Desc.shaderType == VERTEX_SHADER)
	{
		V( GLOBAL::D3DDevice()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pShader->m_pVertexShader ) );
#if defined( DEBUG ) || defined( _DEBUG )
		if( pShader->m_Desc.debugName != 0)
			DXUT_SetDebugName( pShader->m_pVertexShader, pShader->m_Desc.debugName );
#endif)
		GLOBAL::RenderStateMgr()->CreateInputLayout(pShader->m_Desc.eVertexyType, pBlob);
	}
	else
	{
		V( GLOBAL::D3DDevice()->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pShader->m_pPixelShader ) );
#if defined( DEBUG ) || defined( _DEBUG )
		if( pShader->m_Desc.debugName != 0)
			DXUT_SetDebugName( pShader->m_pVertexShader, pShader->m_Desc.debugName );
#endif
	}

	SAFE_RELEASE( pBlob );
}


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

	if( flag & RENDER_FLAG_BUMP_MAP )
	{
		macroBuf[bufIndex].Name = "_BUMP";
		macroBuf[bufIndex].Definition = "0";
		bufIndex++;
	}

	if( flag & RENDER_FLAG_SPECULAR_MAP )
	{
		macroBuf[bufIndex].Name = "_SPECULA_MAP";
		macroBuf[bufIndex].Definition = "0";
		bufIndex++;
	}
	
	macroBuf[bufIndex].Name = NULL;
	macroBuf[bufIndex].Definition = NULL;
}

//------------------------------------------------------------------------------------------------------------
void GetVertexFormat(UINT flag, SHADER_COMPILE_DESC& desc)
{
	if( ( flag & RENDER_FLAG_SKIN ) &&	( flag &RENDER_FLAG_BUMP_MAP ) )
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT_4BT_4BW;
	}
	else if( flag & RENDER_FLAG_SKIN )
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT_4BW;
	}
	else if( flag & RENDER_FLAG_BUMP_MAP )
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT_4BT;
	}
	else 
	{
		desc.eVertexyType = FVF_4HP_4BN_2HT;
	}
}

//------------------------------------------------------------------------------------------------------------
void MakeRenderState(UINT flag, RDX11Shader* pShader)
{
	if( flag & RENDER_FLAG_DEPTH_TEST_OFF )
		pShader->m_RenderState.DepthStencil = DEPTH_OFF_STENCIL_OFF;
	else if( flag & RENDER_FLAG_DEPTH_TEST_OFF )
		pShader->m_RenderState.DepthStencil = DEPTH_ON_STENCIL_OFF;
	else
		pShader->m_RenderState.DepthStencil = DEPTH_WRITE_STENCIL_OFF;

	pShader->m_RenderState.RasterizerState = RASTERIZER_CULL_BACK;

	if( flag & RENDER_FLAG_BLEND_CONSTANT)
		pShader->m_RenderState.BlendState = BLEND_ADD_BY_COLOR;
	else if( flag & RENDER_FLAG_BLEND_ALPHA )	
		pShader->m_RenderState.BlendState = BLEND_ADD_BY_ALPHA;
	else
		pShader->m_RenderState.BlendState = BLEND_NONE;
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
	//////////////////////////////////////////////////////////////////////////
	// Vertex shaders
	{
		RDX11Shader* pVS = new RDX11Shader;

		pVS->m_Desc.szEntrypoint = "VS_FONT";
		pVS->m_Desc.szShaderModel = "vs_4_0_level_9_3";
		pVS->m_Desc.eVertexyType = FVF_3FP_1DC_2HT;
		pVS->m_Desc.shaderType = VERTEX_SHADER;
		pVS->m_Desc.szFileName = SHADER_FILE;
		Compile(pVS);

		m_Shaders[VERTEX_SHADER].SetAt( SHADER_FONT_VS, pVS);
	}

	{
		RDX11Shader* pVS = new RDX11Shader;

		pVS->m_Desc.szEntrypoint = "VS_POS";
		pVS->m_Desc.szShaderModel = "vs_4_0_level_9_3";
		pVS->m_Desc.eVertexyType = FVF_3FP;
		pVS->m_Desc.shaderType = VERTEX_SHADER;
		pVS->m_Desc.szFileName = SHADER_FILE;
		Compile(pVS);

		m_Shaders[VERTEX_SHADER].SetAt( SHADER_POS_VS, pVS);		
	}

	{
		RDX11Shader* pVS = new RDX11Shader;

		pVS->m_Desc.szEntrypoint = "VS_QUAD";
		pVS->m_Desc.szShaderModel = "vs_4_0";
		pVS->m_Desc.eVertexyType = FVF_QUAD;
		pVS->m_Desc.shaderType = VERTEX_SHADER;
		pVS->m_Desc.szFileName = SHADER_FILE;
		Compile(pVS);

		m_Shaders[VERTEX_SHADER].SetAt( SHADER_QUAD_VS, pVS);
	}

	{
		RDX11Shader* pVS = new RDX11Shader;

		pVS->m_Desc.szEntrypoint = "VS_COLOR";
		pVS->m_Desc.szShaderModel = "vs_4_0_level_9_3";
		pVS->m_Desc.eVertexyType = FVF_3FP_1DC;
		pVS->m_Desc.shaderType = VERTEX_SHADER;
		pVS->m_Desc.szFileName = SHADER_FILE;
		Compile(pVS);

		m_Shaders[VERTEX_SHADER].SetAt( SHADER_COLOR_VS, pVS);
	}


	//////////////////////////////////////////////////////////////////////////
	// Pixel shaders
	{
		RDX11Shader* pPS = new RDX11Shader;

		pPS->m_Desc.szEntrypoint = "PS_FONT";
		pPS->m_Desc.szShaderModel = "ps_4_0_level_9_3";
		pPS->m_Desc.shaderType = PIXEL_SHADER;
		pPS->m_Desc.szFileName = SHADER_FILE;
		pPS->m_RenderState.DepthStencil = DEPTH_OFF_STENCIL_OFF;
		pPS->m_RenderState.RasterizerState = RASTERIZER_CULL_BACK;
		pPS->m_RenderState.BlendState = BLEND_ADD_BY_ALPHA;
		Compile(pPS);

		m_Shaders[PIXEL_SHADER].SetAt( SHADER_FONT_PS, pPS);
	}

	{
		RDX11Shader* pPS = new RDX11Shader;

		pPS->m_Desc.szEntrypoint = "PS_COLOR";
		pPS->m_Desc.szShaderModel = "ps_4_0_level_9_3";
		pPS->m_Desc.shaderType = PIXEL_SHADER;
		pPS->m_Desc.szFileName = SHADER_FILE;
		pPS->m_RenderState.DepthStencil = DEPTH_ON_STENCIL_OFF;
		pPS->m_RenderState.RasterizerState = RASTERIZER_CULL_BACK;
		pPS->m_RenderState.BlendState = BLEND_NONE;
		Compile(pPS);

		m_Shaders[PIXEL_SHADER].SetAt( SHADER_COLOR_PS, pPS);
	}

	{
		RDX11Shader* pPS = new RDX11Shader;

		pPS->m_Desc.szEntrypoint = "PS_TEXTURE";
		pPS->m_Desc.szShaderModel = "ps_4_0_level_9_3";
		pPS->m_Desc.shaderType = PIXEL_SHADER;
		pPS->m_Desc.szFileName = SHADER_FILE;
		pPS->m_RenderState.DepthStencil = DEPTH_WRITE_STENCIL_OFF;
		pPS->m_RenderState.RasterizerState = RASTERIZER_CULL_BACK;
		pPS->m_RenderState.BlendState = BLEND_NONE;
		Compile(pPS);

		m_Shaders[PIXEL_SHADER].SetAt( SHADER_TEXURE_PS, pPS);
	}
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
void RDX11ShaderMgr::Begin(IShader* pVS, IShader* pPS)
{
	if( pVS->GetDesc().shaderType != VERTEX_SHADER ||
		pPS->GetDesc().shaderType != PIXEL_SHADER)
	{
		assert(0);
		return;
	}
	
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	if( m_pCurrentShader[VERTEX_SHADER] != pVS )
	{
		RDX11Shader* pRDX11VS = (RDX11Shader*)pVS;
		pContext->VSSetShader( pRDX11VS->m_pVertexShader, NULL, 0 );

		m_pCurrentShader[VERTEX_SHADER] = pRDX11VS;
	}

	if( m_pCurrentShader[PIXEL_SHADER] != pPS )
	{
		RDX11Shader* pRDX11PS = (RDX11Shader*)pPS;
		pContext->PSSetShader( pRDX11PS->m_pPixelShader, NULL, 0 );
		GLOBAL::RenderStateMgr()->ApplyRenderState(pRDX11PS->m_RenderState);

		m_pCurrentShader[PIXEL_SHADER] = pRDX11PS;
	}
}

void RDX11ShaderMgr::Begin(UINT vsFlag, UINT psFlag)
{
	IShaderMgr* pShaderMgr = GLOBAL::ShaderMgr();

	IShader* pVS = GetShader( vsFlag , VERTEX_SHADER );
	IShader* pPS = GetShader( psFlag , PIXEL_SHADER );

	Begin(pVS, pPS);
}

//------------------------------------------------------------------------------------------------------------
RDX11Shader* RDX11ShaderMgr::GetShader(UINT flag, eSHADER_TYPE type)
{
	SHADER_MAP::CPair* pShader = m_Shaders[type].Lookup( flag );
	if( pShader != NULL )
		return pShader->m_value;

	return CreateShader(flag, type);
}


//------------------------------------------------------------------------------------------------------------
RDX11Shader* RDX11ShaderMgr::CreateShader(UINT flag, eSHADER_TYPE shaderType )
{
	D3D10_SHADER_MACRO macroBuf[10];
	RDX11Shader* pShader = new RDX11Shader;

	MakeShaderMacro( flag, macroBuf);
	GetVertexFormat( flag, pShader->m_Desc);
	pShader->m_Desc.shader_Macros = macroBuf;
	pShader->m_Desc.szFileName = SHADER_FILE;
	pShader->m_Desc.uberFlag = flag;
	pShader->m_Desc.shaderType = shaderType;

	if( shaderType == PIXEL_SHADER )
	{
		pShader->m_Desc.szEntrypoint = "PS";
		pShader->m_Desc.szShaderModel = "ps_4_0_level_9_3";
		MakeRenderState(flag, pShader);
	}
	else if( shaderType == VERTEX_SHADER )
	{
		pShader->m_Desc.szEntrypoint = "VS";
		pShader->m_Desc.szShaderModel = "vs_4_0_level_9_3";
	}

	Compile( pShader );
	m_Shaders[shaderType].SetAt( flag, pShader);
	return pShader;
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
	if( (size % 16) != 0 )	// data should be aligned by 16
		assert(0);

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

void RDX11ShaderMgr::Reload(UINT flag)
{
	D3D10_SHADER_MACRO macroBuf[10];
	if( flag == 0) // reload all
	{
		for( UINT i=0; i < NUM_SHADER_TYPE; ++i)
		{
			POSITION pos = m_Shaders[i].GetStartPosition();
			SHADER_MAP::CPair* itr = NULL;

			while (pos)
			{
				itr = m_Shaders[i].GetNext(pos);
				RDX11Shader* pShader = itr->m_value;
				pShader->Release();
				
				MakeShaderMacro( pShader->m_Desc.uberFlag, macroBuf);
				pShader->m_Desc.shader_Macros = macroBuf;

				Compile( pShader );
			}
		}
	}
	else
	{
		RDX11Shader* pShader = GetShader(flag, PIXEL_SHADER);

		MakeShaderMacro( pShader->m_Desc.uberFlag, macroBuf);
		pShader->m_Desc.shader_Macros = macroBuf;

		pShader->Release();
		Compile( pShader );

		pShader = GetShader(flag, VERTEX_SHADER);
		MakeShaderMacro( pShader->m_Desc.uberFlag, macroBuf);
		pShader->m_Desc.shader_Macros = macroBuf;

		pShader->Release();
		Compile( pShader );
	}
}
