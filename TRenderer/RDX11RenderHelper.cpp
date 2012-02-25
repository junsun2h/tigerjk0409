#include "CGeometryConstructor.h"
#include "CGrowableArray.h"
#include "CColor.h"
#include "CQuad.h"

#include "RDefine.h"

#include "IEntity.h"
#include "IEntityProxy.h"
#include "IRDevice.h"
#include "IRenderHelper.h"
#include "IAssetMgr.h"
#include "IRenderStateMgr.h"
#include "IShader.h"

#include "RDefine.h"
#include "RDX11Global.h"
#include "RDX11RenderHelper.h"


//--------------------------------------------------------------------------------------------------------------------
RDX11RenderHelper::RDX11RenderHelper()
	: m_LineBufferBytes(0)
	, m_pLineBuffer(NULL)
	, m_pFontTexture(NULL)
	, m_pFontBuffer(NULL)
	, m_FontBufferBytes(0)
{
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::Destroy()
{
	SAFE_RELEASE( m_pLineBuffer )
	m_LineVertices.RemoveAll();
	m_LineBufferBytes = 0;

	SAFE_RELEASE( m_pFontBuffer );
	m_FontBufferBytes = 0;
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderSkeleton(IEntity* pEntity)
{
	IEntityProxyActor* pActor = (IEntityProxyActor*)pEntity->GetProxy(ENTITY_PROXY_ACTOR);
	if( pActor == NULL)
		return;

	JOINT_ENTITY_LIST* pJointEntitesList = pActor->GetJointEntities();

	CVertexPC v1;
	v1.color = COLOR_RED;

	for( UINT i=1; i < pJointEntitesList->size(); ++i)
	{
		v1.vPos = (*pJointEntitesList)[i]->GetWorldPos();
		m_LineVertices.Add(v1);
		v1.vPos = (*pJointEntitesList)[i]->GetParent()->GetWorldPos();
		m_LineVertices.Add(v1);
	}

	// Set Line Shader
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->Begin();
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->SetShaderContants( XMMatrixIdentity() );
	GLOBAL::ShaderMgr()->GetShader(MPASS_PS_COLOR)->Begin();

	GLOBAL::RenderStateMgr()->SetDepthStancil(DEPTH_STENCIL_OFF);
	
	DrawLine();
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderAxis(XMMATRIX& tm, float scale)
{
	// Set Line Shader
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->Begin();
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->SetShaderContants(tm);
	GLOBAL::ShaderMgr()->GetShader(MPASS_PS_COLOR)->Begin();

	CVertexPC v1;

	v1.vPos = CVector3(0.0f, 0.0f, 0.0f);
	v1.color = COLOR_RED;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(scale, 0.0f, 0.0f);
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0.0f, 0.0f);
	v1.color = COLOR_BLUE;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0.0f, scale);
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, 0, 0.0f);
	v1.color = COLOR_GREEN;
	m_LineVertices.Add(v1);

	v1.vPos = CVector3(0.0f, scale, 0.0f);
	m_LineVertices.Add(v1);

	GLOBAL::RenderStateMgr()->SetDepthStancil(DEPTH_STENCIL_OFF);
	DrawLine();
}

//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderScaler(XMMATRIX& tm, float scale)
{
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->Begin();
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->SetShaderContants(tm);
	GLOBAL::ShaderMgr()->GetShader(MPASS_PS_COLOR)->Begin();

	//////////////////////////////////////////////////////////////////////////
	// Create Geometry
	CObjectPool<CResourceGeometry> memPool(10);

	CResourceGeometry boxX;
	CResourceGeometry boxY;
	CResourceGeometry boxZ;
	CResourceGeometry boxCenter;

	BOX_MAKE_PARAM param;
	param.color = COLOR_RED;
	param.min = CVector3(-5.f,-5.f,-5.f);
	param.max = CVector3(5.f,5.f,5.f);
	param.offset = CVector3(scale, 0, 0);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( &boxX, param);

	param.color = COLOR_GREEN;
	param.offset = CVector3(0, scale, 0);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( &boxY, param);

	param.color = COLOR_BLUE;
	param.offset = CVector3(0, 0, scale);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( &boxZ, param);

	param.color = COLOR_GRAY;
	param.offset = CVector3(0, 0, 0);

	CGEOMETRY_CONSTRUCTOR::CreateBoxGeometry( &boxCenter, param);

	IRDevice* pDevice = GLOBAL::RDevice();
	IRenderStrategy* pRenderer = GLOBAL::RDevice()->GetRenderStrategy();

	pDevice->CreateGraphicBuffer( &boxX);
	pDevice->CreateGraphicBuffer( &boxY);
	pDevice->CreateGraphicBuffer( &boxZ);
	pDevice->CreateGraphicBuffer( &boxCenter);
	
	boxX.loadState = RESOURCE_LOAD_FINISHED;
	boxY.loadState = RESOURCE_LOAD_FINISHED;
	boxZ.loadState = RESOURCE_LOAD_FINISHED;
	boxCenter.loadState = RESOURCE_LOAD_FINISHED;

	pRenderer->SetTransform(tm);
	pRenderer->RenderGeometry(&boxX);
	pRenderer->RenderGeometry(&boxY);
	pRenderer->RenderGeometry(&boxZ);
	pRenderer->RenderGeometry(&boxCenter);

	pDevice->RemoveGraphicBuffer( &boxX);
	pDevice->RemoveGraphicBuffer( &boxY);
	pDevice->RemoveGraphicBuffer( &boxZ);
	pDevice->RemoveGraphicBuffer( &boxCenter);

	RenderAxis(tm, scale);
}

//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderRotator(XMMATRIX& tm, float scale)
{
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->Begin();
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->SetShaderContants(tm);
	GLOBAL::ShaderMgr()->GetShader(MPASS_PS_COLOR)->Begin();
	GLOBAL::RenderStateMgr()->SetDepthStancil(DEPTH_STENCIL_OFF);

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
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->Begin();
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->SetShaderContants(tm);
	GLOBAL::ShaderMgr()->GetShader(MPASS_PS_COLOR)->Begin();

	IRDevice* pDevice = GLOBAL::RDevice();
	IRenderStrategy* pRenderer = GLOBAL::RDevice()->GetRenderStrategy();

	//////////////////////////////////////////////////////////////////////////
	// Create Geometry
	CResourceGeometry coneX;
	CResourceGeometry coneY;
	CResourceGeometry coneZ;

	CONE_MAKE_PARAM param;
	param.color = COLOR_RED;
	param.radius = 5;
	param.height = 15;
	param.direction = X_AXIS;
	param.offset = CVector3(scale, 0, 0);
	param.segment = 10;

	CGEOMETRY_CONSTRUCTOR::CreateConeGeometry( &coneX, param);

	param.color = COLOR_GREEN;
	param.direction = Y_AXIS;
	param.offset = CVector3(0, scale, 0);

	CGEOMETRY_CONSTRUCTOR::CreateConeGeometry( &coneY, param);

	param.color = COLOR_BLUE;
	param.direction = Z_AXIS;
	param.offset = CVector3(0, 0, scale);

	CGEOMETRY_CONSTRUCTOR::CreateConeGeometry( &coneZ, param);
	
	pDevice->CreateGraphicBuffer( &coneX);
	pDevice->CreateGraphicBuffer( &coneY);
	pDevice->CreateGraphicBuffer( &coneZ);

	coneX.loadState = RESOURCE_LOAD_FINISHED;
	coneY.loadState = RESOURCE_LOAD_FINISHED;
	coneZ.loadState = RESOURCE_LOAD_FINISHED;

	pRenderer->SetTransform(tm);
	pRenderer->RenderGeometry( &coneX);
	pRenderer->RenderGeometry( &coneY);
	pRenderer->RenderGeometry( &coneZ);

	pDevice->RemoveGraphicBuffer( &coneX);
	pDevice->RemoveGraphicBuffer( &coneY);
	pDevice->RemoveGraphicBuffer( &coneZ);

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

	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->Begin();
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->SetShaderContants(mtWorld);
	GLOBAL::ShaderMgr()->GetShader(MPASS_PS_COLOR)->Begin();
	GLOBAL::RenderStateMgr()->SetDepthStancil(DEPTH_STENCIL_ON);
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

	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->Begin();
	GLOBAL::ShaderMgr()->GetShader(MPASS_VS_COLOR)->SetShaderContants(mtWorld);
	GLOBAL::ShaderMgr()->GetShader(MPASS_PS_COLOR)->Begin();
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

		GLOBAL::D3DDevice()->CreateBuffer( &BufferDesc, NULL, &m_pLineBuffer );
		DXUT_SetDebugName( m_pLineBuffer, "LineBuffer" );
	}

	//////////////////////////////////////////////////////////////////////////
	// refresh vertex buffer
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();
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

	GLOBAL::RenderStateMgr()->SetTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	GLOBAL::RenderStateMgr()->SetVertexInput(FVF_3FP_1DC);
	pContext->IASetVertexBuffers( 0, 1, &m_pLineBuffer, &Stride, &Offset );
	pContext->Draw( m_LineVertices.GetSize(), 0 );

	m_LineVertices.Reset();
}


//--------------------------------------------------------------------------------------------------------------------
void RDX11RenderHelper::RenderText(RENDER_TEXT_QUAD* pText)
{
	FillFontVertex( pText );

	ID3D11Device* pd3dDevice = GLOBAL::D3DDevice();
	ID3D11DeviceContext* pd3dImmediateContext = GLOBAL::D3DContext();

	// ensure our buffer size can hold our sprites
	UINT FontDataBytes = m_FontVertices.GetSize() * sizeof( CVertexPCT );
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

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if ( S_OK == pd3dImmediateContext->Map( m_pFontBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ) 
	{ 
		CopyMemory( MappedResource.pData, (void*)m_FontVertices.GetData(), FontDataBytes );
		pd3dImmediateContext->Unmap(m_pFontBuffer, 0);
	}

	IShaderMgr* pShaderMgr = GLOBAL::ShaderMgr();

	pShaderMgr->UpdateTexture( m_pFontTexture, 0);
	pShaderMgr->GetShader(MPASS_VS_FONT)->Begin();
	pShaderMgr->GetShader(MPASS_PS_FONT)->Begin();

	GLOBAL::RenderStateMgr()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GLOBAL::RenderStateMgr()->SetVertexInput(FVF_3FP_1DC_2HT);

	// Draw
	UINT Stride = sizeof( CVertexPCT );
	UINT Offset = 0;
	pd3dImmediateContext->IASetVertexBuffers( 0, 1, &m_pFontBuffer, &Stride, &Offset );
	pd3dImmediateContext->Draw( m_FontVertices.GetSize(), 0 );

	m_FontVertices.Reset();
}

void RDX11RenderHelper::FillFontVertex( RENDER_TEXT_QUAD* pText)
{
	float screenWidth = (float)GLOBAL::DeviceInfo().width;
	float screenHeight = (float)GLOBAL::DeviceInfo().height;

	float fCharTexSizeX = 0.010526315f;
	float fGlyphSizeX = 15.0f / screenWidth;
	float fGlyphSizeY = 42.0f / screenHeight;


	float fRectLeft = pText->rc.left / screenWidth;
	float fRectTop = 1.0f - pText->rc.top / screenHeight;

	fRectLeft = fRectLeft * 2.0f - 1.0f;
	fRectTop = fRectTop * 2.0f - 1.0f;

	int NumChars = (int)wcslen( pText->strMsg );
	if ( pText->bCenter ) 
	{
		float fRectRight = pText->rc.right / screenWidth;
		fRectRight = fRectRight * 2.0f - 1.0f;
		float fRectBottom = 1.0f - pText->rc.bottom / screenHeight;
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
		if( pText->strMsg[i] == '\n' )
		{
			fRectLeft = fOriginalLeft;
			fRectTop -= fGlyphSizeY;

			continue;
		}
		else if( pText->strMsg[i] < 32 || pText->strMsg[i] > 126 )
		{
			continue;
		}

		// Add 6 sprite vertices
		CVertexPCT SpriteVertex;
		float fRectRight = fRectLeft + fGlyphSizeX;
		float fRectBottom = fRectTop - fGlyphSizeY;
		float fTexLeft = ( pText->strMsg[i] - 32 ) * fCharTexSizeX;
		float fTexRight = fTexLeft + fCharTexSizeX;

		// tri1
		SpriteVertex.vPos = CVector3( fRectLeft, fRectTop, fDepth );
		SpriteVertex.vTex = XMHALF2( fTexLeft, fTexTop );
		SpriteVertex.vColor = pText->clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectRight, fRectTop, fDepth );
		SpriteVertex.vTex = XMHALF2( fTexRight, fTexTop );
		SpriteVertex.vColor =  pText->clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectLeft, fRectBottom, fDepth );
		SpriteVertex.vTex = XMHALF2( fTexLeft, fTexBottom );
		SpriteVertex.vColor =  pText->clr;
		m_FontVertices.Add( SpriteVertex );

		// tri2
		SpriteVertex.vPos = CVector3( fRectRight, fRectTop, fDepth );
		SpriteVertex.vTex = XMHALF2( fTexRight, fTexTop );
		SpriteVertex.vColor =  pText->clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectRight, fRectBottom, fDepth );
		SpriteVertex.vTex = XMHALF2( fTexRight, fTexBottom );
		SpriteVertex.vColor =  pText->clr;
		m_FontVertices.Add( SpriteVertex );

		SpriteVertex.vPos = CVector3( fRectLeft, fRectBottom, fDepth );
		SpriteVertex.vTex = XMHALF2( fTexLeft, fTexBottom );
		SpriteVertex.vColor =  pText->clr;
		m_FontVertices.Add( SpriteVertex );

		fRectLeft += fGlyphSizeX;
	}
}