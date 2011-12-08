#include "RDX11StateRepository.h"

RDX11StateRepository::RDX11StateRepository()
	: m_bCreated(false)
{
}

RDX11StateRepository::~RDX11StateRepository()
{
	if( m_bCreated )
		Destroy();
}

void RDX11StateRepository::CreateStates(ID3D11Device* pD3Device)
{
	if( m_bCreated == true)
		return;

	CreateSampler(pD3Device);
	CreateBlenderStates(pD3Device);
	CreateRasterizerStates(pD3Device);
	CreateDepthStencilStates(pD3Device);

	m_bCreated = true;
}

void RDX11StateRepository::Destroy()
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

	m_bCreated = false;
}

void RDX11StateRepository::CreateBlenderStates(ID3D11Device* pD3Device)
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

void RDX11StateRepository::CreateDepthStencilStates(ID3D11Device* pD3Device)
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

void RDX11StateRepository::CreateRasterizerStates(ID3D11Device* pD3Device)
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

void RDX11StateRepository::CreateSampler(ID3D11Device* pD3Device)
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

ID3D11BlendState*			RDX11StateRepository::GetBlenderState(ALPHA_BLEND_TYPE type)
{
	return m_pBlendType[type];
}

ID3D11RasterizerState*		RDX11StateRepository::GetRasterizerState(RASTERIZER_TYPE type)
{
	return m_pRasterizerType[type];
}

ID3D11DepthStencilState*	RDX11StateRepository::GetDepthStencilState(DEPTH_STENCIL_TYPE type)
{
	return m_pDepthStencilType[type];
}

ID3D11SamplerState*			RDX11StateRepository::GetSamplerState(TSAMPLER_TYPE type)
{
	return m_SamplerStates[type];
}