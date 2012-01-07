#include "RDX11Shader.h"
#include "RDX11Device.h"


RDX11Shader::RDX11Shader()
	: m_pVertexShader(NULL)
	, m_pPixelShader(NULL)	
	, m_pGeometryShader(NULL)
	, m_pVertexLayout(NULL)
	, m_pVertexLayoutPos4(NULL)
	, m_DepthStencilState(DEPTH_STENCIL_WRITE)
	, m_RasterizerState(RASTERIZER_CULL_BACK)
	, m_BlendState(BLEND_NONE)
{

}

RDX11Shader::~RDX11Shader()
{
	Destroy();
}

void RDX11Shader::Begin()
{
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

	ApplyRenderState();

	pContext->IASetPrimitiveTopology( m_Topology );
	pContext->IASetInputLayout( m_pVertexLayout	);
}

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

void RDX11Shader::ApplyRenderState()
{
	GLOBAL::GetD3DStateMgr()->ApplyRenderState(	m_DepthStencilState,
												m_RasterizerState,
												m_BlendState,
												m_StencilRef,
												m_BlendFactor,
												m_SampleMask);
}

void RDX11Shader::Destroy()
{

}


HRESULT CompileShader( void* pSrc, 
						SIZE_T SrcDataSize, 
						LPCSTR szEntryPoint, 
						LPCSTR szShaderModel, 
						UINT flag,
						ID3DBlob** ppBlobOut )
{
	HRESULT hr = S_OK;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	flag |= D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* pErrorBlob = NULL;

	V_RETURN( D3DCompile( pSrc, SrcDataSize, "none", NULL, NULL, szEntryPoint, szShaderModel, flag, 0, ppBlobOut, NULL ) );

	if( pErrorBlob ) 
		pErrorBlob->Release();

	return S_OK;
}

void RDX11Shader::CreateVS( void* pSrc, 
							SIZE_T SrcDataSize, 
							LPCSTR pEntrypoint, 
							LPCSTR pTarget, 
							UINT flag,
							D3D11_INPUT_ELEMENT_DESC layout[],
							UINT layoutSize, 
							LPCSTR debugName)
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	CompileShader(pSrc, SrcDataSize, pEntrypoint, pTarget, flag, &pBlob);

	V( GLOBAL::GetD3DDevice()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pVertexShader ) );

#if defined( DEBUG ) || defined( _DEBUG )
	DXUT_SetDebugName( m_pVertexShader, debugName );
#endif

	if( layoutSize != 0 )
	{
		V( GLOBAL::GetD3DDevice()->CreateInputLayout( layout, layoutSize, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pVertexLayout ) );
	}

	SAFE_RELEASE( pBlob );
}

void RDX11Shader::CreatePS( void* pSrc, 
							SIZE_T SrcDataSize, 
							LPCSTR pEntrypoint, 
							LPCSTR pTarget, 
							UINT flag,
							LPCSTR debugName)
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	CompileShader(pSrc, SrcDataSize, pEntrypoint, pTarget, flag, &pBlob);

	V( GLOBAL::GetD3DDevice()->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pPixelShader ) );

#if defined( DEBUG ) || defined( _DEBUG )
	DXUT_SetDebugName( m_pPixelShader, debugName );
#endif

	SAFE_RELEASE( pBlob );
}
