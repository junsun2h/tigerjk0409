#include "RDX11Shader.h"
#include "RDX11Device.h"


//--------------------------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(SHADER_COMPILE_DESC& desc, ID3DBlob** ppBlobOut )
{
#if defined( DEBUG ) || defined( _DEBUG )
	desc.flag |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	TDXERROR( D3DX11CompileFromFileA( desc.szFileName, NULL, NULL, desc.szEntrypoint, desc.szShaderModel, desc.flag, 0, NULL, ppBlobOut, &pErrorBlob, NULL ) );

	if( pErrorBlob ) 
		pErrorBlob->Release();

	return S_OK;
}



//--------------------------------------------------------------------------------------------------------
HRESULT CompileShader( SHADER_COMPILE_DESC& desc, ID3DBlob** ppBlobOut )
{
	HRESULT hr = S_OK;
#if defined( DEBUG ) || defined( _DEBUG )
	desc.flag |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBlob = NULL;

	V_RETURN( D3DCompile( desc.pSrc, desc.SrcDataSize, "none", NULL, NULL, desc.szEntrypoint, desc.szShaderModel, desc.flag, 0, ppBlobOut, NULL ) );

	if( pErrorBlob ) 
		pErrorBlob->Release();

	return S_OK;
}


//--------------------------------------------------------------------------------------------------------
RDX11Shader::RDX11Shader()
	: m_pVertexShader(NULL)
	, m_pPixelShader(NULL)	
	, m_pGeometryShader(NULL)
	, m_pVertexLayout(NULL)
	, m_DepthStencilState(DEPTH_STENCIL_WRITE)
	, m_RasterizerState(RASTERIZER_CULL_BACK)
	, m_BlendState(BLEND_NONE)
{

}


RDX11Shader::~RDX11Shader()
{
	Destroy();
}


//--------------------------------------------------------------------------------------------------------
void RDX11Shader::Begin()
{
	GLOBAL::GetShaderMgr()->SetCurrentShader(this);
	GLOBAL::GetD3DStateMgr()->ApplyRenderState(	m_DepthStencilState,
												m_RasterizerState,
												m_BlendState,
												m_StencilRef,
												m_BlendFactor,
												m_SampleMask);

	ID3D11DeviceContext* pContext = GLOBAL::GetD3DContext();

	if( m_pVertexShader != NULL )
		pContext->VSSetShader( m_pVertexShader, NULL, 0 );
	else
		pContext->VSSetShader( NULL, NULL, 0 );

	if( m_pGeometryShader != NULL )
		pContext->GSSetShader( m_pGeometryShader, NULL, 0 );
	else
		pContext->GSSetShader( NULL, NULL, 0 );

	if( m_pPixelShader != NULL )
		pContext->PSSetShader( m_pPixelShader, NULL, 0 );
	else
		pContext->PSSetShader( NULL, NULL, 0 );
	

	pContext->IASetPrimitiveTopology( m_Topology );
	pContext->IASetInputLayout( m_pVertexLayout	);
}


//--------------------------------------------------------------------------------------------------------
void RDX11Shader::SetRenderState(	DEPTH_STENCIL_TYPE DepthStencilState,
									RASTERIZER_TYPE RasterizerState,
									ALPHA_BLEND_TYPE BlendState,
									UINT StencilRef,
									float* blendFactor,
									UINT sampleMask)
{
	m_DepthStencilState = DepthStencilState;
	m_RasterizerState = RasterizerState;
	m_BlendState = BlendState;
	m_StencilRef = StencilRef;

	if( blendFactor != NULL)
		memcpy( m_BlendFactor, blendFactor, 16 );
	else
		memset( m_BlendFactor, 0, 16);

	m_SampleMask = sampleMask;
}


//--------------------------------------------------------------------------------------------------------
void RDX11Shader::Destroy()
{
	SAFE_RELEASE(m_pVertexShader)
	SAFE_RELEASE(m_pPixelShader)
	SAFE_RELEASE(m_pGeometryShader)
	SAFE_RELEASE(m_pVertexLayout)
}


//--------------------------------------------------------------------------------------------------------
void RDX11Shader::CreateVS( SHADER_COMPILE_DESC& desc)
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	if( desc.szFileName == NULL)
		CompileShader(desc, &pBlob);
	else
		CompileShaderFromFile( desc, &pBlob );

	V( GLOBAL::GetD3DDevice()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pVertexShader ) );

#if defined( DEBUG ) || defined( _DEBUG )
	if( desc.debugName != 0)
		DXUT_SetDebugName( m_pVertexShader, desc.debugName );
#endif

	if( desc.layoutSize != 0 )
	{
		V( GLOBAL::GetD3DDevice()->CreateInputLayout( desc.pLayout, desc.layoutSize, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pVertexLayout ) );
	}

	SAFE_RELEASE( pBlob );
}


//--------------------------------------------------------------------------------------------------------
void RDX11Shader::CreatePS( SHADER_COMPILE_DESC& desc )
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	if( desc.szFileName == NULL)
		CompileShader(desc, &pBlob);
	else
		CompileShaderFromFile( desc, &pBlob );

	V( GLOBAL::GetD3DDevice()->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pPixelShader ) );

#if defined( DEBUG ) || defined( _DEBUG )
	DXUT_SetDebugName( m_pPixelShader, desc.debugName );
#endif

	SAFE_RELEASE( pBlob );
}
