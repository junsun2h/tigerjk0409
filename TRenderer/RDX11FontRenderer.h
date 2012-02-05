#pragma once



class RDX11FontRenderer : public IFontRenderer
{
public:
	RDX11FontRenderer();

	bool		SetFontFile(const char* fontDDS) override;
	void		Destroy() override;
	void		Render( RENDER_TEXT_QUAD& text ) override;

private:
	void		FillVertex( RENDER_TEXT_QUAD& text );

	ID3D11Buffer*					m_pFontBuffer;
	UINT							m_FontBufferBytes;
	CGrowableArray<CVertexPCT>		m_FontVertices;
	ID3D11ShaderResourceView*		m_pFontSRV;
};