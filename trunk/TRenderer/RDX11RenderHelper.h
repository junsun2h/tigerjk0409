#pragma once


class RDX11RenderHelper : public IRenderHelper
{
public:
	RDX11RenderHelper();

public:
	void	RenderSkeleton(IEntity* pEntity) override;
	void	RenderAxis(XMMATRIX& tm, float scale) override;
	void	RenderScaler(XMMATRIX& tm, float scale) override;
	void	RenderRotator(XMMATRIX& tm, float scale) override;
	void	RenderMover(XMMATRIX& tm, float scale) override;
	void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) override;
	void	RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount) override;
	void	RenderText(RENDER_TEXT_QUAD* pText) override;

	void	SetFontTexture( CResourceTexture* pTexture) override	{	m_pFontTexture = pTexture;	}

	void	Destroy();

	//////////////////////////////////////////////////////////////////////////
	// Line
private:
	void	DrawLine();

	UINT						m_LineBufferBytes;
	CGrowableArray<CVertexPC>	m_LineVertices;
	ID3D11Buffer*				m_pLineBuffer;

	//////////////////////////////////////////////////////////////////////////
	// Font
private:
	void	FillFontVertex( RENDER_TEXT_QUAD* pText );

	ID3D11Buffer*				m_pFontBuffer;
	UINT						m_FontBufferBytes;
	CGrowableArray<CVertexPCT>	m_FontVertices;
	CResourceTexture*			m_pFontTexture;
};