#pragma once



class RDX11FontRenderer : public IFontRenderer
{
public:
	RDX11FontRenderer();

	void		SetFontTexture( CResourceTexture* pTexture) override;
	void		Destroy() override;
	void		Render( RENDER_TEXT_QUAD* pText ) override;

private:
	void		FillVertex( RENDER_TEXT_QUAD* pText );

	ID3D11Buffer*					m_pFontBuffer;
	UINT							m_FontBufferBytes;
	CGrowableArray<CVertexPCT>		m_FontVertices;
	CResourceTexture*				m_pTexture;
};