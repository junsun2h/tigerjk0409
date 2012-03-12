#pragma once


class RDX11RenderHelper : public IRenderHelper
{
public:
	RDX11RenderHelper();

public:
	void	RenderLine( CVertexPC* pVetex, int count) override;
	void	RenderAxis(XMMATRIX& tm) override;
	void	RenderScaler(XMMATRIX& tm) override;
	void	RenderRotator(XMMATRIX& tm) override;
	void	RenderMover(XMMATRIX& tm) override;
	void	RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color) override;
	void	RenderWorldGrid(XMMATRIX& mtWorld, int size, int segmentCount) override;
	void	RenderText(RENDER_TEXT_QUAD* pText) override;
	void	RenderSphere(XMMATRIX& tm, float radius) override;
	void	RenderPlane(XMMATRIX& tm, CVector2 size) override;

	void	SetFontTexture( CResourceTexture* pTexture) override	{	m_pFontTexture = pTexture;	}
	void	CreateHelperObjects();

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
	void	SetWorldTM(XMMATRIX& pWorld);

	ID3D11Buffer*				m_pFontBuffer;
	UINT						m_FontBufferBytes;
	CGrowableArray<CVertexPCT>	m_FontVertices;
	CResourceTexture*			m_pFontTexture;

	//////////////////////////////////////////////////////////////////////////
	// helper objects
	CResourceGeometry			m_BoxX;
	CResourceGeometry			m_BoxY;
	CResourceGeometry			m_BoxZ;
	CResourceGeometry			m_BoxCenter;

	CResourceGeometry			m_ConeX;
	CResourceGeometry			m_ConeY;
	CResourceGeometry			m_ConeZ;

	CResourceGeometry			m_Sphere;
};