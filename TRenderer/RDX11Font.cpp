#include "RDX11Font.h"


CHAR g_strUIEffectFile[] = \
	"Texture2D g_Texture;"\
	""\
	"SamplerState Sampler"\
	"{"\
	"    Filter = MIN_MAG_MIP_LINEAR;"\
	"    AddressU = Wrap;"\
	"    AddressV = Wrap;"\
	"};"\
	""\
	"struct VS_OUTPUT"\
	"{"\
	"    float4 Pos : POSITION;"\
	"    float4 Dif : COLOR;"\
	"    float2 Tex : TEXCOORD;"\
	"};"\
	""\
	"VS_OUTPUT VS( float3 vPos : POSITION,"\
	"              float4 Dif : COLOR,"\
	"              float2 vTexCoord0 : TEXCOORD )"\
	"{"\
	"    VS_OUTPUT Output;"\
	""\
	"    Output.Pos = float4( vPos, 1.0f );"\
	"    Output.Dif = Dif;"\
	"    Output.Tex = vTexCoord0;"\
	""\
	"    return Output;"\
	"}"\
	""\
	"float4 PS( VS_OUTPUT In ) : SV_Target"\
	"{"\
	"    return g_Texture.Sample( Sampler, In.Tex ) * In.Dif;"\
	"}";

const UINT g_uUIEffectFileSize = sizeof( g_strUIEffectFile );




//--------------------------------------------------------------------------------------
RDX11Font::RDX11Font()
{
	m_pFontBuffer = NULL;
	m_FontBufferBytes = 0;
	m_pFontSRV = NULL;

	m_pVSRenderUI11 = NULL;
	m_pPSRenderUI11 = NULL;

	m_pDepthStencilStateUI11 = NULL;
	m_pRasterizerStateUI11 = NULL;
	m_pBlendStateUI11 = NULL;
	m_pSamplerStateUI11 = NULL;
}

//--------------------------------------------------------------------------------------
void RDX11Font::Destroy()
{
	SAFE_RELEASE( m_pFontBuffer );
	m_FontBufferBytes = 0;
	SAFE_RELEASE( m_pFontSRV );

	// Shaders
	SAFE_RELEASE( m_pVSRenderUI11 );
	SAFE_RELEASE( m_pPSRenderUI11 );

	// States
	SAFE_RELEASE( m_pDepthStencilStateUI11 );
	SAFE_RELEASE( m_pRasterizerStateUI11 );
	SAFE_RELEASE( m_pBlendStateUI11 );
	SAFE_RELEASE( m_pSamplerStateUI11 );
}


//--------------------------------------------------------------------------------------
HRESULT RDX11Font::Init(const char* fontDDS, ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	// Compile Shaders
	ID3DBlob* pVSBlob = NULL;
	ID3DBlob* pPSBlob = NULL;
	V_RETURN( D3DCompile( g_strUIEffectFile, g_uUIEffectFileSize, "none", NULL, NULL, "VS", "vs_4_0_level_9_1", 
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &pVSBlob, NULL ) );
	V_RETURN( D3DCompile( g_strUIEffectFile, g_uUIEffectFileSize, "none", NULL, NULL, "PS", "ps_4_0_level_9_1", 
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &pPSBlob, NULL ) );

	// Create Shaders
	V_RETURN( pd3dDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVSRenderUI11 ) );
	DXUT_SetDebugName( m_pVSRenderUI11, "CDXUTDialogResourceManager" );

	V_RETURN( pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPSRenderUI11 ) );
	DXUT_SetDebugName( m_pPSRenderUI11, "CDXUTDialogResourceManager" );
	
	// States
	D3D11_DEPTH_STENCIL_DESC DSDesc;
	ZeroMemory( &DSDesc, sizeof( D3D11_DEPTH_STENCIL_DESC ) );
	DSDesc.DepthEnable = FALSE;
	DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSDesc.DepthFunc = D3D11_COMPARISON_LESS;
	DSDesc.StencilEnable = FALSE;
	V_RETURN( pd3dDevice->CreateDepthStencilState( &DSDesc, &m_pDepthStencilStateUI11 ) );
	DXUT_SetDebugName( m_pDepthStencilStateUI11, "CDXUTDialogResourceManager" );

	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.AntialiasedLineEnable = FALSE;
	RSDesc.CullMode = D3D11_CULL_BACK;
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0.0f;
	RSDesc.DepthClipEnable = TRUE;
	RSDesc.FillMode = D3D11_FILL_SOLID;
	RSDesc.FrontCounterClockwise = FALSE;
	RSDesc.MultisampleEnable = TRUE;
	RSDesc.ScissorEnable = FALSE;
	RSDesc.SlopeScaledDepthBias = 0.0f;
	V_RETURN( pd3dDevice->CreateRasterizerState( &RSDesc, &m_pRasterizerStateUI11 ) );
	DXUT_SetDebugName( m_pRasterizerStateUI11, "CDXUTDialogResourceManager" );

	D3D11_BLEND_DESC BSDesc;
	ZeroMemory( &BSDesc, sizeof( D3D11_BLEND_DESC ) );

	BSDesc.RenderTarget[0].BlendEnable = TRUE;
	BSDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BSDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BSDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BSDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BSDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BSDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BSDesc.RenderTarget[0].RenderTargetWriteMask = 0x0F;

	V_RETURN( pd3dDevice->CreateBlendState( &BSDesc, &m_pBlendStateUI11 ) );
	DXUT_SetDebugName( m_pBlendStateUI11, "CDXUTDialogResourceManager" );


	D3D11_SAMPLER_DESC SSDesc;
	ZeroMemory( &SSDesc, sizeof( D3D11_SAMPLER_DESC ) );
	SSDesc.Filter = D3D11_FILTER_ANISOTROPIC   ;
	SSDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SSDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SSDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SSDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SSDesc.MaxAnisotropy = 16;
	SSDesc.MinLOD = 0;
	SSDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if ( pd3dDevice->GetFeatureLevel() < D3D_FEATURE_LEVEL_9_3 ) {
		SSDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		SSDesc.MaxAnisotropy = 0;
	}
	V_RETURN( pd3dDevice->CreateSamplerState( &SSDesc, &m_pSamplerStateUI11 ) );
	DXUT_SetDebugName( m_pSamplerStateUI11, "CDXUTDialogResourceManager" );


	// Create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	V_RETURN( pd3dDevice->CreateInputLayout( layout, ARRAYSIZE( layout ), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout11 ) );
	DXUT_SetDebugName( m_pInputLayout11, "CDXUTDialogResourceManager" );

	// Release the blobs
	SAFE_RELEASE( pVSBlob );
	SAFE_RELEASE( pPSBlob );
	
	V_RETURN( D3DX11CreateShaderResourceViewFromFileA( pd3dDevice, fontDDS, NULL, NULL, &m_pFontSRV, &hr) );

	return hr;
}


//--------------------------------------------------------------------------------------
void RDX11Font::FillVertex( RENDER_TEXT_BUFFER& text, float screenWidth, float screenHeight  )
{
	float fCharTexSizeX = 0.010526315f;
	float fGlyphSizeX = 15.0f / screenWidth;
	float fGlyphSizeY = 42.0f / screenHeight;


	float fRectLeft = text.rc.left / screenWidth;
	float fRectTop = 1.0f - text.rc.top / screenHeight;

	fRectLeft = fRectLeft * 2.0f - 1.0f;
	fRectTop = fRectTop * 2.0f - 1.0f;

	int NumChars = (int)wcslen( text.strMsg );
	if ( text.bCenter ) 
	{
		float fRectRight = text.rc.right / screenWidth;
		fRectRight = fRectRight * 2.0f - 1.0f;
		float fRectBottom = 1.0f - text.rc.bottom / screenHeight;
		fRectBottom = fRectBottom * 2.0f - 1.0f;
		float fcenterx = ((fRectRight - fRectLeft) - (float)NumChars*fGlyphSizeX) *0.5f;
		float fcentery = ((fRectTop - fRectBottom) - (float)1*fGlyphSizeY) *0.5f;
		fRectLeft += fcenterx ;    
		fRectTop -= fcentery;
	}

	float fOriginalLeft = fRectLeft;
	float fTexTop = 0.0f;
	float fTexBottom = 1.0f;

	float fDepth = 0.5f;
	for( int i=0; i<NumChars; i++ )
	{
		if( text.strMsg[i] == '\n' )
		{
			fRectLeft = fOriginalLeft;
			fRectTop -= fGlyphSizeY;

			continue;
		}
		else if( text.strMsg[i] < 32 || text.strMsg[i] > 126 )
		{
			continue;
		}

		// Add 6 sprite vertices
		DXUTSpriteVertex SpriteVertex;
		float fRectRight = fRectLeft + fGlyphSizeX;
		float fRectBottom = fRectTop - fGlyphSizeY;
		float fTexLeft = ( text.strMsg[i] - 32 ) * fCharTexSizeX;
		float fTexRight = fTexLeft + fCharTexSizeX;

		// tri1
		SpriteVertex.vPos = CVector3( fRectLeft, fRectTop, fDepth );
		SpriteVertex.vTex = CVector2( fTexLeft, fTexTop );
		SpriteVertex.vColor = text.clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectRight, fRectTop, fDepth );
		SpriteVertex.vTex = CVector2( fTexRight, fTexTop );
		SpriteVertex.vColor =  text.clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectLeft, fRectBottom, fDepth );
		SpriteVertex.vTex = CVector2( fTexLeft, fTexBottom );
		SpriteVertex.vColor =  text.clr;
		m_FontVertices.Add( SpriteVertex );

		// tri2
		SpriteVertex.vPos = CVector3( fRectRight, fRectTop, fDepth );
		SpriteVertex.vTex = CVector2( fTexRight, fTexTop );
		SpriteVertex.vColor =  text.clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectRight, fRectBottom, fDepth );
		SpriteVertex.vTex = CVector2( fTexRight, fTexBottom );
		SpriteVertex.vColor =  text.clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectLeft, fRectBottom, fDepth );
		SpriteVertex.vTex = CVector2( fTexLeft, fTexBottom );
		SpriteVertex.vColor =  text.clr;
		m_FontVertices.Add( SpriteVertex );

		fRectLeft += fGlyphSizeX;
	}
}


//--------------------------------------------------------------------------------------
void RDX11Font::RenderText( ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	// ensure our buffer size can hold our sprites
	UINT FontDataBytes = m_FontVertices.GetSize() * sizeof( DXUTSpriteVertex );
	if( m_FontBufferBytes < FontDataBytes )
	{
		SAFE_RELEASE( m_pFontBuffer );
		m_FontBufferBytes = FontDataBytes;

		D3D11_BUFFER_DESC BufferDesc;
		BufferDesc.ByteWidth = m_FontBufferBytes;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;

		pd3dDevice->CreateBuffer( &BufferDesc, NULL, &m_pFontBuffer );
		DXUT_SetDebugName( m_pFontBuffer, "DXUT Text11" );
	}

	// Copy the sprites over
	D3D11_BOX destRegion;
	destRegion.left = 0;
	destRegion.right = FontDataBytes;
	destRegion.top = 0;
	destRegion.bottom = 1;
	destRegion.front = 0;
	destRegion.back = 1;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if ( S_OK == pd3dImmediateContext->Map( m_pFontBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ) { 
		CopyMemory( MappedResource.pData, (void*)m_FontVertices.GetData(), FontDataBytes );
		pd3dImmediateContext->Unmap(m_pFontBuffer, 0);
	}

	ID3D11ShaderResourceView* pOldTexture = NULL;
	pd3dImmediateContext->PSGetShaderResources( 0, 1, &pOldTexture );
	pd3dImmediateContext->PSSetShaderResources( 0, 1, &m_pFontSRV );

	// Draw
	UINT Stride = sizeof( DXUTSpriteVertex );
	UINT Offset = 0;
	pd3dImmediateContext->IASetVertexBuffers( 0, 1, &m_pFontBuffer, &Stride, &Offset );
	pd3dImmediateContext->IASetInputLayout( m_pInputLayout11 );
	pd3dImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	pd3dImmediateContext->Draw( m_FontVertices.GetSize(), 0 );

	pd3dImmediateContext->PSSetShaderResources( 0, 1, &pOldTexture );
	SAFE_RELEASE( pOldTexture );

	m_FontVertices.Reset();
}


void RDX11Font::DrawText(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, RENDER_TEXT_BUFFER& text, float sw, float sh )
{
	FillVertex( text, sw, sh );
	RenderText( pd3dDevice, pd3dImmediateContext );
}

//--------------------------------------------------------------------------------------
void RDX11Font::ApplyRenderState( ID3D11DeviceContext* pd3dImmediateContext )
{
	// Shaders
	pd3dImmediateContext->VSSetShader( m_pVSRenderUI11, NULL, 0 );
	pd3dImmediateContext->HSSetShader( NULL, NULL, 0 );
	pd3dImmediateContext->DSSetShader( NULL, NULL, 0 );
	pd3dImmediateContext->GSSetShader( NULL, NULL, 0 );
	pd3dImmediateContext->PSSetShader( m_pPSRenderUI11, NULL, 0 );

	// States
	pd3dImmediateContext->OMSetDepthStencilState( m_pDepthStencilStateUI11, 0 );
	pd3dImmediateContext->RSSetState( m_pRasterizerStateUI11 );
	float BlendFactor[4] = { 0, 0, 0, 0 };
	pd3dImmediateContext->OMSetBlendState( m_pBlendStateUI11, BlendFactor, 0xFFFFFFFF );
    pd3dImmediateContext->PSSetSamplers( 0, 1, &m_pSamplerStateUI11 );
}
