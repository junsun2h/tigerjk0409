#include "RDX11RenderHelper.h"
#include "CResource.h"
#include "RDX11Device.h"
#include "CGeometryConstructor.h"


//--------------------------------------------------------------------------------------------------------------------
RDX11RenderHelper::RDX11RenderHelper()
	: m_LineBufferBytes(0)
	, m_pLineBuffer(NULL)
{

}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::Init(const char* fontDDS)
{
	m_FontRenderer.SetFontFile(fontDDS);
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::Destroy()
{
	m_FontRenderer.Destroy();
	SAFE_RELEASE( m_pLineBuffer )
	m_LineVertices.RemoveAll();
	m_LineBufferBytes = 0;
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderAxis(XMMATRIX& tm, float scale)
{
	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->Begin();
	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->SetShaderContants(tm);

	CVertexPC v1;

	v1.vPos = CVector3(0.0f, 0.0f, 0.0f);
	v1.color = COLOR_RED;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(scale, 0.0f, 0.0f);
	v1.color = COLOR_RED;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0.0f, 0.0f);
	v1.color = COLOR_BLUE;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0.0f, scale);
	v1.color = COLOR_BLUE;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0, 0.0f);
	v1.color = COLOR_GREEN;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, scale, 0.0f);
	v1.color = COLOR_GREEN;
	m_LineVertices.Add(v1);

	GLOBAL::GetD3DStateMgr()->SetDepthStancil(DEPTH_STENCIL_OFF);
	DrawLine();
}

//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderScaler(XMMATRIX& tm, float scale)
{
	IEngineMemoryMgr* pMemoryPoolMgr = GLOBAL::Engine()->EngineMemoryMgr();
	RDX11Shader* pShader = GLOBAL::GetShaderMgr()->GetShader(EFFECT_MPASS_MESH);
	RDX11Device* pDevice = GLOBAL::GetRDX11Device();
	//////////////////////////////////////////////////////////////////////////
	// Create Geometry
	
	pShader->Begin();
	pShader->SetShaderContants( tm);

	CResourceGeometry* pBoxX = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);
	CResourceGeometry* pBoxY = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);
	CResourceGeometry* pBoxZ = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);
	CResourceGeometry* pBoxCenter = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);

	BOX_MAKE_PARAM param;
	param.color = COLOR_RED;
	param.min = CVector3(-5.f,-5.f,-5.f);
	param.max = CVector3(5.f,5.f,5.f);
	param.offset = CVector3(scale, 0, 0);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( pBoxX, param);

	param.color = COLOR_GREEN;
	param.offset = CVector3(0, scale, 0);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( pBoxY, param);

	param.color = COLOR_BLUE;
	param.offset = CVector3(0, 0, scale);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( pBoxZ, param);

	param.color = COLOR_GRAY;
	param.offset = CVector3(0, 0, 0);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( pBoxCenter, param);

	pDevice->PT_CreateGraphicBuffer( pBoxX);
	pDevice->PT_CreateGraphicBuffer( pBoxY);
	pDevice->PT_CreateGraphicBuffer( pBoxZ);
	pDevice->PT_CreateGraphicBuffer( pBoxCenter);

	pDevice->RenderGeometry(pBoxX);
	pDevice->RenderGeometry(pBoxY);
	pDevice->RenderGeometry(pBoxZ);
	pDevice->RenderGeometry(pBoxCenter);

	pDevice->RemoveGraphicBuffer( pBoxX);
	pDevice->RemoveGraphicBuffer( pBoxY);
	pDevice->RemoveGraphicBuffer( pBoxZ);
	pDevice->RemoveGraphicBuffer( pBoxCenter);

	pMemoryPoolMgr->RemoveResource(pBoxX);
	pMemoryPoolMgr->RemoveResource(pBoxY);
	pMemoryPoolMgr->RemoveResource(pBoxZ);
	pMemoryPoolMgr->RemoveResource(pBoxCenter);

	RenderAxis(tm, scale);
}

//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderRotator(XMMATRIX& tm, float scale)
{
	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->Begin();
	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->SetShaderContants(tm);
	GLOBAL::GetD3DStateMgr()->SetDepthStancil(DEPTH_STENCIL_OFF);

	CVertexPC v1;

	for( int iRing = 0; iRing < 2; ++iRing )
	{
		float radius = scale + iRing * 2;
		UINT segment = 20;

		v1.vPos.x = 0.0f;
		v1.color = COLOR_RED;
		for( UINT i= 0; i < segment ; ++i)
		{
			float angle = XM_2PI * i/float(segment);
			v1.vPos.y = radius * cos( angle);
			v1.vPos.z = radius * sin( angle);
			m_LineVertices.Add(v1);

			angle = XM_2PI * (i +1)/float(segment);
			v1.vPos.y = radius * cos( angle);
			v1.vPos.z = radius * sin( angle);
			m_LineVertices.Add(v1);
		}
		DrawLine();

		v1.vPos.y = 0.0f;
		v1.color = COLOR_GREEN;
		for( UINT i= 0; i < segment ; ++i)
		{
			float angle = XM_2PI * i/float(segment);
			v1.vPos.z = radius * cos( angle);
			v1.vPos.x = radius * sin( angle);
			m_LineVertices.Add(v1);

			angle = XM_2PI * (i +1)/float(segment);
			v1.vPos.z = radius * cos( angle);
			v1.vPos.x = radius * sin( angle);
			m_LineVertices.Add(v1);
		}

		DrawLine();

		v1.vPos.z = 0.0f;
		v1.color = COLOR_BLUE;
		for( UINT i= 0; i < segment ; ++i)
		{
			float angle = XM_2PI * i/float(segment);
			v1.vPos.x = radius * cos( angle);
			v1.vPos.y = radius * sin( angle);
			m_LineVertices.Add(v1);

			angle = XM_2PI * (i +1)/float(segment);
			v1.vPos.x = radius * cos( angle);
			v1.vPos.y = radius * sin( angle);
			m_LineVertices.Add(v1);
		}

		DrawLine();
	}

	RenderAxis(tm, scale);
}

//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderMover(XMMATRIX& tm, float scale)
{
	IEngineMemoryMgr* pMemoryPoolMgr = GLOBAL::Engine()->EngineMemoryMgr();
	RDX11Shader* pShader = GLOBAL::GetShaderMgr()->GetShader(EFFECT_MPASS_MESH);
	RDX11Device* pDevice = GLOBAL::GetRDX11Device();

	//////////////////////////////////////////////////////////////////////////
	// Create Geometry
	pShader->Begin();
	pShader->SetShaderContants(tm);

	CResourceGeometry* pConeX = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);
	CResourceGeometry* pConeY = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);
	CResourceGeometry* pConeZ = (CResourceGeometry*)pMemoryPoolMgr->GetNewResource(RESOURCE_GEOMETRY);

	CONE_MAKE_PARAM param;
	param.color = COLOR_RED;
	param.radius = 5;
	param.height = 15;
	param.direction = X_AXIS;
	param.offset = CVector3(scale, 0, 0);
	param.segment = 10;

	CGEOMETRY_CONSTRUCTOR::CreateConeGeometry( pConeX, param);

	param.color = COLOR_GREEN;
	param.direction = Y_AXIS;
	param.offset = CVector3(0, scale, 0);

	CGEOMETRY_CONSTRUCTOR::CreateConeGeometry( pConeY, param);

	param.color = COLOR_BLUE;
	param.direction = Z_AXIS;
	param.offset = CVector3(0, 0, scale);

	CGEOMETRY_CONSTRUCTOR::CreateConeGeometry( pConeZ, param);
	
	pDevice->PT_CreateGraphicBuffer( pConeX);
	pDevice->PT_CreateGraphicBuffer( pConeY);
	pDevice->PT_CreateGraphicBuffer( pConeZ);

	pDevice->RenderGeometry(pConeX);
	pDevice->RenderGeometry(pConeY);
	pDevice->RenderGeometry(pConeZ);

	pDevice->RemoveGraphicBuffer( pConeX);
	pDevice->RemoveGraphicBuffer( pConeY);
	pDevice->RemoveGraphicBuffer( pConeZ);

	pMemoryPoolMgr->RemoveResource(pConeX);
	pMemoryPoolMgr->RemoveResource(pConeY);
	pMemoryPoolMgr->RemoveResource(pConeZ);

	RenderAxis(tm, scale);
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderBox(XMMATRIX& mtWorld, CVector3& min, CVector3& max, DWORD color)
{
	BOX_MAKE_PARAM param;
	param.min = min;
	param.max = max;
	param.offset = CVector3(0, 0, 0);

	CVertexPC* pVertices = NULL;
	CGEOMETRY_CONSTRUCTOR::CreateBoxLine( param, &pVertices);

	for (int i =0 ; i< 24; i++)
	{
		pVertices[i].color = color;
		m_LineVertices.Add(pVertices[i]);
	}
	SAFE_DELETE_ARRAY(pVertices);

	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->Begin();
	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->SetShaderContants(mtWorld);
	DrawLine();
}

//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderWorldGrid(XMMATRIX& mtWorld, int size, int lineCount)
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

	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->Begin();
	GLOBAL::GetShaderMgr()->GetShader(EFFECT_LINE)->SetShaderContants(mtWorld);
	DrawLine();
}

//--------------------------------------------------------------------------------------------------------------------
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

	//////////////////////////////////////////////////////////////////////////
	// Draw
	UINT Stride = sizeof( CVertexPC );
	UINT Offset = 0;

	pContext->IASetVertexBuffers( 0, 1, &m_pLineBuffer, &Stride, &Offset );
	pContext->Draw( m_LineVertices.GetSize(), 0 );

	m_LineVertices.Reset();
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderText(RENDER_TEXT_BUFFER& text)
{
	m_FontRenderer.Render(text);
}

