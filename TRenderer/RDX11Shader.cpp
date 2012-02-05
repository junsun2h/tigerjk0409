#include "RDX11Global.h"
#include "RDX11Shader.h"


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
RDX11ShaderBase::RDX11ShaderBase()
	: m_pVertexShader(NULL)
	, m_pPixelShader(NULL)	
	, m_pGeometryShader(NULL)
	, m_pVertexLayout(NULL)
{
}


RDX11ShaderBase::~RDX11ShaderBase()
{
	Destroy();
}


//--------------------------------------------------------------------------------------------------------
void RDX11ShaderBase::Begin()
{
	GLOBAL::ShaderMgr()->SetCurrentShader(this);
	GLOBAL::RenderStateMgr()->ApplyRenderState(	m_RenderState );

	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

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
void RDX11ShaderBase::SetRenderState(const GRAPHIC_DEVICE_DESC desc)
{
	m_RenderState = desc;
}


//--------------------------------------------------------------------------------------------------------
void RDX11ShaderBase::Destroy()
{
	SAFE_RELEASE(m_pVertexShader)
	SAFE_RELEASE(m_pPixelShader)
	SAFE_RELEASE(m_pGeometryShader)
	SAFE_RELEASE(m_pVertexLayout)
}


//--------------------------------------------------------------------------------------------------------
void RDX11ShaderBase::CreateVS( SHADER_COMPILE_DESC& desc)
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	if( desc.szFileName == NULL)
		CompileShader(desc, &pBlob);
	else
		CompileShaderFromFile( desc, &pBlob );

	V( GLOBAL::D3DDevice()->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pVertexShader ) );

#if defined( DEBUG ) || defined( _DEBUG )
	if( desc.debugName != 0)
		DXUT_SetDebugName( m_pVertexShader, desc.debugName );
#endif

	if( desc.layoutSize != 0 )
	{
		V( GLOBAL::D3DDevice()->CreateInputLayout( desc.pLayout, desc.layoutSize, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pVertexLayout ) );
	}

	SAFE_RELEASE( pBlob );
}


//--------------------------------------------------------------------------------------------------------
void RDX11ShaderBase::CreatePS( SHADER_COMPILE_DESC& desc )
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	if( desc.szFileName == NULL)
		CompileShader(desc, &pBlob);
	else
		CompileShaderFromFile( desc, &pBlob );

	V( GLOBAL::D3DDevice()->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pPixelShader ) );

#if defined( DEBUG ) || defined( _DEBUG )
	DXUT_SetDebugName( m_pPixelShader, desc.debugName );
#endif

	SAFE_RELEASE( pBlob );
}
