#include "RDX11Font.h"
#include "RDX11Device.h"


//--------------------------------------------------------------------------------------
RDX11FontRenderer::RDX11FontRenderer()
{
	m_pFontBuffer = NULL;
	m_FontBufferBytes = 0;
	m_pFontSRV = NULL;
}

//--------------------------------------------------------------------------------------
void RDX11FontRenderer::Destroy()
{
	SAFE_RELEASE( m_pFontBuffer );
	m_FontBufferBytes = 0;
	SAFE_RELEASE( m_pFontSRV );
}


//--------------------------------------------------------------------------------------
HRESULT RDX11FontRenderer::SetFontFile(const char* fontDDS)
{
	HRESULT hr = S_OK;

	V_RETURN( D3DX11CreateShaderResourceViewFromFileA( GLOBAL::GetD3DDevice(), fontDDS, NULL, NULL, &m_pFontSRV, &hr) );

	return hr;
}


//--------------------------------------------------------------------------------------
void RDX11FontRenderer::FillVertex( RENDER_TEXT_BUFFER& text, float screenWidth, float screenHeight  )
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
		RSpriteVertex SpriteVertex;
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
void RDX11FontRenderer::Render( RENDER_TEXT_BUFFER& text, int sw, int sh )
{
	FillVertex( text, float(sw), float(sh) );

	ID3D11Device* pd3dDevice = GLOBAL::GetD3DDevice();
	ID3D11DeviceContext* pd3dImmediateContext = GLOBAL::GetD3DContext();

	// ensure our buffer size can hold our sprites
	UINT FontDataBytes = m_FontVertices.GetSize() * sizeof( RSpriteVertex );
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
		DXUT_SetDebugName( m_pFontBuffer, "FontVertexBuffer" );
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
	if ( S_OK == pd3dImmediateContext->Map( m_pFontBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ) 
	{ 
		CopyMemory( MappedResource.pData, (void*)m_FontVertices.GetData(), FontDataBytes );
		pd3dImmediateContext->Unmap(m_pFontBuffer, 0);
	}

	ID3D11ShaderResourceView* pOldTexture = NULL;
	pd3dImmediateContext->PSGetShaderResources( 0, 1, &pOldTexture );
	pd3dImmediateContext->PSSetShaderResources( 0, 1, &m_pFontSRV );

	// Draw
	UINT Stride = sizeof( RSpriteVertex );
	UINT Offset = 0;
	pd3dImmediateContext->IASetVertexBuffers( 0, 1, &m_pFontBuffer, &Stride, &Offset );
	pd3dImmediateContext->Draw( m_FontVertices.GetSize(), 0 );

	pd3dImmediateContext->PSSetShaderResources( 0, 1, &pOldTexture );
	SAFE_RELEASE( pOldTexture );

	m_FontVertices.Reset();
}