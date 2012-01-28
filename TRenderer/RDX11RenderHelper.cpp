#include "RDX11RenderHelper.h"
#include "CResource.h"
#include "RDX11Device.h"



RDX11RenderHelper::RDX11RenderHelper()
	: m_LineBufferBytes(0)
	, m_pLineBuffer(NULL)
{

}

void RDX11RenderHelper::Init(const char* fontDDS)
{
	m_FontRenderer.SetFontFile(fontDDS);
}

void RDX11RenderHelper::Destroy()
{
	m_FontRenderer.Destroy();
	SAFE_RELEASE( m_pLineBuffer )
	m_LineVertices.RemoveAll();
	m_LineBufferBytes = 0;
}

void RDX11RenderHelper::RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color)
{
	CVector3 tmpVertex[8];

	tmpVertex[0] = min;

	tmpVertex[1] = tmpVertex[0];
	tmpVertex[1].x = max.x;

	tmpVertex[2] = max;
	tmpVertex[2].y = min.y;

	tmpVertex[3] = min;
	tmpVertex[3].z = max.z;

	tmpVertex[4] = tmpVertex[0];
	tmpVertex[4].y = max.y;

	tmpVertex[5] = tmpVertex[1];
	tmpVertex[5].y = max.y;

	tmpVertex[6] = tmpVertex[2];
	tmpVertex[6].y = max.y;

	tmpVertex[7] = tmpVertex[3];
	tmpVertex[7].y = max.y;

	CVertexPC pVertices[24];
	// bottom
	pVertices[0].vPos = tmpVertex[0];
	pVertices[1].vPos = tmpVertex[1];
	pVertices[2].vPos = tmpVertex[1];
	pVertices[3].vPos = tmpVertex[2];
	pVertices[4].vPos = tmpVertex[2];
	pVertices[5].vPos = tmpVertex[3];
	pVertices[6].vPos = tmpVertex[3];
	pVertices[7].vPos = tmpVertex[0];

	// top
	pVertices[8].vPos = tmpVertex[4];
	pVertices[9].vPos = tmpVertex[5];
	pVertices[10].vPos = tmpVertex[5];
	pVertices[11].vPos = tmpVertex[6];
	pVertices[12].vPos = tmpVertex[6];
	pVertices[13].vPos = tmpVertex[7];
	pVertices[14].vPos = tmpVertex[7];
	pVertices[15].vPos = tmpVertex[4];

	// side
	pVertices[16].vPos = tmpVertex[0];
	pVertices[17].vPos = tmpVertex[4];
	pVertices[18].vPos = tmpVertex[1];
	pVertices[19].vPos = tmpVertex[5];
	pVertices[20].vPos = tmpVertex[2];
	pVertices[21].vPos = tmpVertex[6];
	pVertices[22].vPos = tmpVertex[3];
	pVertices[23].vPos = tmpVertex[7];

	for (int i =0 ; i< 24; i++)
	{
		pVertices[i].color = color;
		m_LineVertices.Add(pVertices[i]);
	}

	XMMATRIX wvp = XMMatrixMultiply( mtWorld, GLOBAL::GetCameraDesc().ViewTM );
	wvp = XMMatrixMultiply( wvp, GLOBAL::GetCameraDesc().ProjTM );

	wvp = XMMatrixTranspose(wvp);
	GLOBAL::GetShaderMgr()->UpdateShaderConstant( &wvp, sizeof( XMMATRIX), SM_BUF6_64BYTE_SLOT0, VS_SHADER );

	DrawLine();
}

void RDX11RenderHelper::RenderGrid(XMMATRIX& mtWorld, int size, int lineCount)
{
	float halfWidth = size/2.f;
	float lineWidth = size/50.f;

	CVertexPC v1,v2;

	for(int i = 0 ; i < lineCount ;  i += 2)
	{

		v1.vPos.x = halfWidth - i/2 * lineWidth;
		v2.vPos.x = v1.vPos.x;

		v1.vPos.z = 0;
		v2.vPos.z = 0;

		if( i== 50 )
		{
			v1.vPos.y = 0;
			v2.vPos.y = -halfWidth;
		}
		else
		{
			v1.vPos.y = halfWidth;
			v2.vPos.y = -halfWidth;
		}

		v1.color = COLOR_GRAY;
		v2.color = COLOR_GRAY;

		m_LineVertices.Add(v1);
		m_LineVertices.Add(v2);
	}

	for(int i = lineCount ; i < lineCount *2 ; i += 2)
	{
		v1.vPos.y = halfWidth - ( i - lineCount )/2 * lineWidth;
		v2.vPos.y = v1.vPos.y;

		v1.vPos.z = 0;
		v2.vPos.z = 0;

		if( i== 150 )
		{
			v1.vPos.x = 0;
			v2.vPos.x = -halfWidth;
		}
		else
		{
			v1.vPos.x = halfWidth;
			v2.vPos.x = -halfWidth;
		}

		v1.color = COLOR_GRAY;
		v2.color = COLOR_GRAY;

		m_LineVertices.Add(v1);
		m_LineVertices.Add(v2);
	}

	v1.vPos = CVector3(0.0f, 0.0f, 0.0f);
	v1.color = COLOR_RED;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(halfWidth, 0.0f, 0.0f);
	v1.color = COLOR_RED;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0.0f, 0.0f);
	v1.color = COLOR_BLUE;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0.0f, halfWidth);
	v1.color = COLOR_BLUE;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0, 0.0f);
	v1.color = COLOR_GREEN;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, halfWidth, 0.0f);
	v1.color = COLOR_GREEN;
	m_LineVertices.Add(v1);

	XMMATRIX wvp = GLOBAL::GetCameraDesc().ViewTM;
	wvp = XMMatrixMultiply( wvp, GLOBAL::GetCameraDesc().ProjTM );

	wvp = XMMatrixTranspose(wvp);
	GLOBAL::GetShaderMgr()->UpdateShaderConstant( &wvp, sizeof( XMMATRIX), SM_BUF6_64BYTE_SLOT0, VS_SHADER );

	DrawLine();
}

void RDX11RenderHelper::DrawLine()
{
	UINT dataBytes = m_LineVertices.GetSize() * sizeof( CVertexPC );
	if( m_LineBufferBytes < dataBytes )
	{
		SAFE_RELEASE( m_pLineBuffer );
		m_LineBufferBytes = dataBytes;

		D3D11_BUFFER_DESC BufferDesc;
		BufferDesc.ByteWidth = m_LineBufferBytes;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;

		GLOBAL::GetD3DDevice()->CreateBuffer( &BufferDesc, NULL, &m_pLineBuffer );
		DXUT_SetDebugName( m_pLineBuffer, "LineBuffer" );
	}

	//////////////////////////////////////////////////////////////////////////
	// Copy the sprites over
	ID3D11DeviceContext* pContext = GLOBAL::GetD3DContext();

	D3D11_BOX destRegion;
	destRegion.left = 0;
	destRegion.right = dataBytes;
	destRegion.top = 0;
	destRegion.bottom = 1;
	destRegion.front = 0;
	destRegion.back = 1;
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if ( S_OK == pContext->Map( m_pLineBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ) 
	{ 
		CopyMemory( MappedResource.pData, (void*)m_LineVertices.GetData(), dataBytes );
		pContext->Unmap(m_pLineBuffer, 0);
	}

	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->Begin();

	//////////////////////////////////////////////////////////////////////////
	// Draw
	UINT Stride = sizeof( CVertexPC );
	UINT Offset = 0;

	pContext->IASetVertexBuffers( 0, 1, &m_pLineBuffer, &Stride, &Offset );
	pContext->Draw( m_LineVertices.GetSize(), 0 );

	m_LineVertices.Reset();
}


void RDX11RenderHelper::RenderText(RENDER_TEXT_BUFFER& text)
{
	m_FontRenderer.Render(text);
}

