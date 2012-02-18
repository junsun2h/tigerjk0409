#include "RDefine.h"
#include "CResource.h"

#include "IRDevice.h"
#include "IEntityProxy.h"
#include "IShader.h"
#include "IRenderStateMgr.h"

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
// Vertext Shader
//--------------------------------------------------------------------------------------------------------
void RDX11VertexShaderBase::CreateVS( SHADER_COMPILE_DESC& desc)
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
	
	GLOBAL::RenderStateMgr()->CreateInputLayout(desc.eVertexyType, pBlob);

	SAFE_RELEASE( pBlob );
}

void RDX11VertexShaderBase::Begin()
{
	if( GLOBAL::ShaderMgr()->SetCurrentShader(this) == false)
		return;

	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	if( m_pVertexShader != NULL )
		pContext->VSSetShader( m_pVertexShader, NULL, 0 );
	else
		pContext->VSSetShader( NULL, NULL, 0 );
}


//--------------------------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------------------------
void RDX11PixelShaderBase::CreatePS( SHADER_COMPILE_DESC& desc )
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

void RDX11PixelShaderBase::SetRenderState(const GRAPHIC_DEVICE_DESC desc)
{
	m_RenderState = desc;
}

void RDX11PixelShaderBase::Begin()
{
	if( GLOBAL::ShaderMgr()->SetCurrentShader(this) == false)
		return;

	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	GLOBAL::RenderStateMgr()->ApplyRenderState(m_RenderState);

	if( m_pPixelShader != NULL )
		pContext->PSSetShader( m_pPixelShader, NULL, 0 );
	else
		pContext->PSSetShader( NULL, NULL, 0 );
}

//--------------------------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------------------------
void RDX11GeometryShaderBase::CreateGS( SHADER_COMPILE_DESC& desc)
{
	assert(0);
	/*
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;

	if( desc.szFileName == NULL)
		CompileShader(desc, &pBlob);
	else
		CompileShaderFromFile( desc, &pBlob );

	V( GLOBAL::D3DDevice()->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &m_pGeometryShader ) );

#if defined( DEBUG ) || defined( _DEBUG )
	DXUT_SetDebugName( m_pPixelShader, desc.debugName );
#endif

	SAFE_RELEASE( pBlob );

	TDXERROR( GLOBAL::D3DDevice()->CreateGeometryShaderWithStreamOutput( pBlob->GetBufferPointer()
																, pBlob->GetBufferSize()
																, desc.pOutPutLayout
																, desc.OuputLayoutSize
																, NULL
																, 0
																, 0
																, NULL
																, &m_pGeometryShader) ); 
	*/
}

void RDX11GeometryShaderBase::Begin()
{
	if( GLOBAL::ShaderMgr()->SetCurrentShader(this) == false)
		return;

	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	if( m_pGeometryShader != NULL )
		pContext->GSSetShader( m_pGeometryShader, NULL, 0 );
	else
		pContext->GSSetShader( NULL, NULL, 0 );
}
