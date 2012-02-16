#include <atlcoll.h>

#include "CDefine.h"

#include "IRDX11Device.h"
#include "IRenderStateMgr.h"

#include "RDX11Global.h"
#include "RDX11RenderStateMgr.h"


RDX11RenderStateMgr::RDX11RenderStateMgr()
	: m_bCreated(false)
	, m_currentVertexType(FVF_INVALID)
	, m_currentTopology(D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
{
}

RDX11RenderStateMgr::~RDX11RenderStateMgr()
{
	if( m_bCreated )
		Destroy();
}

void RDX11RenderStateMgr::Init()
{
	if( m_bCreated == true)
		return;

	ID3D11Device* pD3Device = GLOBAL::D3DDevice();

	CreateSampler(pD3Device);
	CreateBlenderStates(pD3Device);
	CreateRasterizerStates(pD3Device);
	CreateDepthStencilStates(pD3Device);

	GLOBAL::D3DContext()->PSSetSamplers(0, 3, m_SamplerStates);

	m_bCreated = true;
}

void RDX11RenderStateMgr::Destroy()
{
	for (int i= 0; i < NUM_ALPHA_BLEND_TYPE; ++i)
	{
		SAFE_RELEASE(m_pBlendType[i]);
	}

	for (int i= 0; i < NUM_RASTERIZER_TYPE; ++i)
	{
		SAFE_RELEASE(m_pRasterizerType[i]);
	}

	for (int i= 0; i < NUM_DEPTH_STENCIL; ++i)
	{
		SAFE_RELEASE(m_pDepthStencilType[i]);
	}

	for (int i=0; i < NUM_SAMPLER_TYPE; ++i)
	{
		SAFE_RELEASE(m_SamplerStates[i]);
	}

	// clear input
	{
		POSITION pos = m_InputLayoutMap.GetStartPosition();
		INPUT_LAYOUT_MAP::CPair* itr = NULL;

		while (pos)
		{
			itr = m_InputLayoutMap.GetNext(pos);
			SAFE_RELEASE( itr->m_value );
		}

		m_InputLayoutMap.RemoveAll();
	}

	m_bCreated = false;
}

void RDX11RenderStateMgr::CreateBlenderStates(ID3D11Device* pD3Device)
{
	D3D11_BLEND_DESC blendDesc;

	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	pD3Device->CreateBlendState( &blendDesc, &m_pBlendType[BLEND_ADD_BY_COLOR] );

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	pD3Device->CreateBlendState( &blendDesc, &m_pBlendType[BLEND_ADD_BY_ALPHA] );

	m_pBlendType[BLEND_NONE] = NULL;
}

void RDX11RenderStateMgr::CreateDepthStencilStates(ID3D11Device* pD3Device)
{
	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		pD3Device->CreateDepthStencilState( &dsDesc, &m_pDepthStencilType[DEPTH_STENCIL_OFF] );
	}

	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		pD3Device->CreateDepthStencilState( &dsDesc, &m_pDepthStencilType[DEPTH_STENCIL_ON] );
	}

	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		pD3Device->CreateDepthStencilState( &dsDesc, &m_pDepthStencilType[DEPTH_STENCIL_REVERSE] );
	}

	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		pD3Device->CreateDepthStencilState( &dsDesc, &m_pDepthStencilType[DEPTH_STENCIL_WRITE] );
	}

	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		dsDesc.StencilEnable = false;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace = dsDesc.FrontFace;

		pD3Device->CreateDepthStencilState( &dsDesc, &m_pDepthStencilType[DEPTH_STENCIL_REVERSE_WRITE] );
	}
}

void RDX11RenderStateMgr::CreateRasterizerStates(ID3D11Device* pD3Device)
{
	D3D11_RASTERIZER_DESC rsDesc;
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = FALSE;
	rsDesc.DepthBias = 0;
	rsDesc.DepthBiasClamp = 0.0;
	rsDesc.SlopeScaledDepthBias =  0.0;
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.ScissorEnable = FALSE;
	rsDesc.MultisampleEnable = TRUE;
	rsDesc.AntialiasedLineEnable = FALSE;

	for(int i=0; i < NUM_RASTERIZER_TYPE; ++i)
	{
		switch(i)
		{
		case RASTERIZER_CULL_FRONT: 
			rsDesc.CullMode = D3D11_CULL_FRONT;
			rsDesc.FillMode = D3D11_FILL_SOLID;
			rsDesc.SlopeScaledDepthBias = 0.0;
			break;
		case RASTERIZER_CULL_BACK:
			rsDesc.CullMode = D3D11_CULL_BACK;
			rsDesc.FillMode = D3D11_FILL_SOLID;
			rsDesc.SlopeScaledDepthBias = 0.0;
			break;
		case RASTERIZER_WIRE: 
			rsDesc.CullMode = D3D11_CULL_BACK;
			rsDesc.FillMode = D3D11_FILL_WIREFRAME;
			rsDesc.SlopeScaledDepthBias = 0.0;
			break;
		case RASTERIZER_SHADOW:
			rsDesc.CullMode = D3D11_CULL_BACK;
			rsDesc.FillMode = D3D11_FILL_SOLID;
			rsDesc.SlopeScaledDepthBias = 1.0; // Setting the slope scale depth biase greatly decreases surface acne and incorrect self shadowing.
			break;
		}

		TDXERROR( pD3Device->CreateRasterizerState( &rsDesc, &m_pRasterizerType[i] ) );
	}
}

void RDX11RenderStateMgr::CreateSampler(ID3D11Device* pD3Device)
{
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory( &SamDesc, sizeof(SamDesc) );
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;

	TDXERROR( pD3Device->CreateSamplerState( &SamDesc, &m_SamplerStates[LNEAR_SAMPLER] ) );

	SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	SamDesc.MipLODBias = 0;
	SamDesc.MaxAnisotropy = 16;

	TDXERROR( pD3Device->CreateSamplerState( &SamDesc, &m_SamplerStates[ANSIO16_SAMPLER] ) );

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.MaxLOD = 0;

	TDXERROR( pD3Device->CreateSamplerState( &SamDesc, &m_SamplerStates[POINT_SAMPLER] ) );
}

void RDX11RenderStateMgr::ApplyRenderState(const GRAPHIC_DEVICE_DESC& desc)
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	pContext->OMSetDepthStencilState( m_pDepthStencilType[desc.DepthStencil], desc.StencilRef );
	pContext->RSSetState( m_pRasterizerType[desc.RasterizerState] );
	pContext->OMSetBlendState( m_pBlendType[desc.BlendState], desc.blendFactor, desc.sampleMask );

	m_CurrentDesc = desc;
}

//--------------------------------------------------------------------------------------
void RDX11RenderStateMgr::StoreCurrentState()
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	pContext->OMGetDepthStencilState( &m_pStoredDepthStencil, &m_StoredStencilRef );
	pContext->RSGetState( &m_pStoredRasterizer );
	pContext->OMGetBlendState( &m_pStoredBlend, m_StoredBlendFactor, &m_StoredSampleMask );
}

//--------------------------------------------------------------------------------------
void RDX11RenderStateMgr::RestoreSavedState()
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	pContext->OMSetDepthStencilState( m_pStoredDepthStencil, m_StoredStencilRef );
	pContext->RSSetState( m_pStoredRasterizer );
	pContext->OMSetBlendState( m_pStoredBlend, m_StoredBlendFactor, m_StoredSampleMask );

	SAFE_RELEASE( m_pStoredDepthStencil );
	SAFE_RELEASE( m_pStoredRasterizer );
	SAFE_RELEASE( m_pStoredBlend );
}

void RDX11RenderStateMgr::SetRasterizer(RASTERIZER_TYPE RasterizerState)
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	pContext->RSSetState( m_pRasterizerType[RasterizerState] );
	m_CurrentDesc.RasterizerState = RasterizerState;
}

void RDX11RenderStateMgr::SetBlend(ALPHA_BLEND_TYPE BlendState, float* pblendFactor, UINT sampleMask)
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	pContext->OMSetBlendState( m_pBlendType[BlendState], pblendFactor, sampleMask );
	m_CurrentDesc.BlendState = BlendState;
	m_CurrentDesc.blendFactor = pblendFactor;
	m_CurrentDesc.sampleMask = sampleMask;
}

void RDX11RenderStateMgr::SetDepthStancil(DEPTH_STENCIL_TYPE depthStencil, UINT StencilRef)
{
	ID3D11DeviceContext* pContext = GLOBAL::D3DContext();

	pContext->OMSetDepthStencilState( m_pDepthStencilType[depthStencil], StencilRef );
	m_CurrentDesc.DepthStencil = depthStencil;
	m_CurrentDesc.StencilRef = StencilRef;
}

void RDX11RenderStateMgr::CreateInputLayout( eCVERTEX_TYPE eVertexyType, ID3DBlob* pBlob)
{
	if( pBlob == NULL )
		return;

	ID3D11Device* pD3Device = GLOBAL::D3DDevice();
	ID3D11InputLayout* pD3DLayout = NULL;

	if( m_InputLayoutMap.Lookup(eVertexyType) != NULL )
		return;

	if( eVertexyType == FVF_3FP_1DC )
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		pD3Device->CreateInputLayout( layout, ARRAYSIZE( layout ), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pD3DLayout );
	}
	else if( eVertexyType == FVF_3FP_1DC_2HT )
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",  0, DXGI_FORMAT_R16G16_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		pD3Device->CreateInputLayout( layout, ARRAYSIZE( layout ), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pD3DLayout );
	}
	else if( eVertexyType == FVF_4HP_4BN_2HT)
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_SINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		pD3Device->CreateInputLayout( layout, ARRAYSIZE( layout ), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pD3DLayout );
	}
	else if( eVertexyType == FVF_4HP_4BN_2HT_4BW )
	{
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R8G8B8A8_SINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONES", 0, DXGI_FORMAT_R8G8B8A8_SINT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHTS", 0, DXGI_FORMAT_R8G8B8A8_SINT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		pD3Device->CreateInputLayout( layout, ARRAYSIZE( layout ), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pD3DLayout );
	}

	m_InputLayoutMap.SetAt( eVertexyType, pD3DLayout);
}

void RDX11RenderStateMgr::SetVertexInput(eCVERTEX_TYPE type)
{
	if( type == m_currentVertexType )
		return;

	INPUT_LAYOUT_MAP::CPair* pInputLayout = m_InputLayoutMap.Lookup( type );

	if( pInputLayout == NULL )
	{
		assert(0);
		return;
	}

	GLOBAL::D3DContext()->IASetInputLayout( pInputLayout->m_value );
}

void RDX11RenderStateMgr::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
	if( m_currentTopology == topology )
		return;

	GLOBAL::D3DContext()->IASetPrimitiveTopology( topology );
}