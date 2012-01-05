#include "RDX11RenderHelper.h"
#include "IResource.h"
#include "RDX11Device.h"


CHAR g_strLineEffectFile[] = \
	"cbuffer CB_Model : register( b0 )"\
	"{"\
	"	matrix WVP;	"\
	"};"\
	""\
	""\
	"struct VS_INPUT"\
	"{"\
	"	float4 Pos : POSITION;"\
	"	uint4 Color : COLOR;"\
	"};"\
	""\
	"struct PS_INPUT"\
	"{"\
	"	float4 Pos : SV_POSITION;"\
	"	float4 Color : COLOR;"\
	"};"\
	""\
	"PS_INPUT VS( VS_INPUT In )"\
	"{"\
	"	PS_INPUT OUT = (PS_INPUT)0;"\
	""\
	"	OUT.Pos = mul( In.Pos, WVP );"\
	"	OUT.Color.x = In.Color.x/255.f;"\
	"	OUT.Color.y = In.Color.y/255.f;"\
	"	OUT.Color.z = In.Color.z/255.f;"\
	"	OUT.Color.w = In.Color.w/255.f;"\
	""\
	"	return OUT;"\
	"}"\
	""\
	"float4 PS( PS_INPUT In) : SV_Target"\
	"{"\
	"	return In.Color;"\
	"}";


const UINT g_LineEffectFileSize = sizeof( g_strLineEffectFile );


RDX11RenderHelper::RDX11RenderHelper()
	: m_LineBufferBytes(0)
	, m_pVSRender(NULL)
	, m_pPSRender(NULL)
	, m_pInputLayout(NULL)
	, m_pLineBuffer(NULL)
{

}

bool RDX11RenderHelper::Init()
{
	HRESULT hr = S_OK;

	ID3D11Device* pD3Device = RDX11Device::GetDevice();

	// Compile Shaders
	ID3DBlob* pVSBlob = NULL;
	ID3DBlob* pPSBlob = NULL;
	V( D3DCompile( g_strLineEffectFile, g_LineEffectFileSize, "none", NULL, NULL, "VS", "vs_4_0_level_9_1", D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &pVSBlob, NULL ) );
	V( D3DCompile( g_strLineEffectFile, g_LineEffectFileSize, "none", NULL, NULL, "PS", "ps_4_0_level_9_1", D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY, 0, &pPSBlob, NULL ) );

	V( pD3Device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVSRender ) );
	DXUT_SetDebugName( m_pVSRender, "LineShaderVS" );

	V( pD3Device->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPSRender ) );
	DXUT_SetDebugName( m_pPSRender, "LineShaderPS" );

	// Create input layout
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	V( pD3Device->CreateInputLayout( layout, ARRAYSIZE( layout ), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout ) );
	DXUT_SetDebugName( m_pInputLayout, "LineShaderInputLayout" );

	return true;
}


void RDX11RenderHelper::Destroy()
{
	SAFE_RELEASE( m_pVSRender )
	SAFE_RELEASE( m_pPSRender )
	SAFE_RELEASE( m_pInputLayout )
	SAFE_RELEASE( m_pLineBuffer )
	m_LineVertices.RemoveAll();
	m_LineBufferBytes = 0;
}

void RDX11RenderHelper::RenderBox(CMatrix& mtWorld, CVector3& min, CVector3& max)
{

}

void RDX11RenderHelper::RenderGrid(CMatrix& mtWorld, int size, int segmentCount)
{
	int vertexCount = ( segmentCount + 3 ) * 2;

	m_LineVertices.SetSize(vertexCount);

	float halfWidth = size/2.f;
	float lineWidth = size/50.f;

	for(int i = 0 ; i < segmentCount ;  i += 2)
	{
		m_LineVertices[i].vPos.x = halfWidth - i/2 * lineWidth;
		m_LineVertices[i+1].vPos.x = m_LineVertices[i].vPos.x;

		m_LineVertices[i].vPos.z = 0;
		m_LineVertices[i+1].vPos.z = 0;

		if( i== 50 )
		{
			m_LineVertices[i].vPos.y = 0;
			m_LineVertices[i+1].vPos.y = -halfWidth;
		}
		else
		{
			m_LineVertices[i].vPos.y = halfWidth;
			m_LineVertices[i+1].vPos.y = -halfWidth;
		}

		m_LineVertices[i].color = COLOR_GRAY;
		m_LineVertices[i+1].color = COLOR_GRAY;
	}

	for(int i = segmentCount ; i < segmentCount *2 ; i += 2)
	{
		m_LineVertices[i].vPos.y = halfWidth - ( i - segmentCount )/2 * lineWidth;
		m_LineVertices[i+1].vPos.y = m_LineVertices[i].vPos.y;

		m_LineVertices[i].vPos.z = 0;
		m_LineVertices[i+1].vPos.z = 0;

		if( i== 150 )
		{
			m_LineVertices[i].vPos.x = 0;
			m_LineVertices[i+1].vPos.x = -halfWidth;
		}
		else
		{
			m_LineVertices[i].vPos.x = halfWidth;
			m_LineVertices[i+1].vPos.x = -halfWidth;
		}

		m_LineVertices[i].color = COLOR_GRAY;
		m_LineVertices[i+1].color = COLOR_GRAY;
	}

	m_LineVertices[segmentCount*2 +0].vPos = CVector3(0.0f, 0.0f, 0.0f);
	m_LineVertices[segmentCount*2 +0].color = COLOR_RED;

	m_LineVertices[segmentCount*2 +1].vPos = CVector3(halfWidth, 0.0f, 0.0f);
	m_LineVertices[segmentCount*2 +1].color = COLOR_RED;

	m_LineVertices[segmentCount*2 +2].vPos = CVector3(0.0f, 0.0f, 0.0f);
	m_LineVertices[segmentCount*2 +2].color = COLOR_BLUE;

	m_LineVertices[segmentCount*2 +3].vPos = CVector3(0.0f, 0.0f, halfWidth);
	m_LineVertices[segmentCount*2 +3].color = COLOR_BLUE;

	m_LineVertices[segmentCount*2 +4].vPos = CVector3(0.0f, 0, 0.0f);
	m_LineVertices[segmentCount*2 +4].color = COLOR_GREEN;

	m_LineVertices[segmentCount*2 +5].vPos = CVector3(0.0f, halfWidth, 0.0f);
	m_LineVertices[segmentCount*2 +5].color = COLOR_GREEN;

		
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

		RDX11Device::GetDevice()->CreateBuffer( &BufferDesc, NULL, &m_pLineBuffer );
		DXUT_SetDebugName( m_pLineBuffer, "LineBuffer" );
	}

	//////////////////////////////////////////////////////////////////////////
	// Draw
	ApplyRenderState();

	ID3D11DeviceContext* pContext = RDX11Device::GetContext();

	UINT Stride = sizeof( CVertexPC );
	UINT Offset = 0;
		
	pContext->IASetVertexBuffers( 0, 1, &m_pLineBuffer, &Stride, &Offset );
	pContext->IASetInputLayout( m_pInputLayout );
	pContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_LINELIST );
	pContext->Draw( m_LineVertices.GetSize(), 0 );
}

void RDX11RenderHelper::ApplyRenderState()
{
	ID3D11DeviceContext* pContext = RDX11Device::GetContext();

	// Shaders
	pContext->VSSetShader( m_pVSRender, NULL, 0 );
	pContext->HSSetShader( NULL, NULL, 0 );
	pContext->DSSetShader( NULL, NULL, 0 );
	pContext->GSSetShader( NULL, NULL, 0 );
	pContext->PSSetShader( m_pPSRender, NULL, 0 );
}
