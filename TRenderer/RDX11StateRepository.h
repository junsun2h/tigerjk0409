#pragma once

#include "RDefine.h"


enum ALPHA_BLEND_TYPE
{
	BLEND_NONE,
	BLEND_ADD_BY_COLOR,
	BLEND_ADD_BY_ALPHA,

	NUM_ALPHA_BLEND_TYPE
};

enum RASTERIZER_TYPE
{
	RASTERIZER_CULL_FRONT,
	RASTERIZER_CULL_BACK,
	RASTERIZER_WIRE,
	RASTERIZER_SHADOW,

	NUM_RASTERIZER_TYPE
};

enum DEPTH_STENCIL_TYPE
{
	DEPTH_STENCIL_ON,
	DEPTH_STENCIL_OFF,
	DEPTH_STENCIL_REVERSE,
	DEPTH_STENCIL_WRITE,
	DEPTH_STENCIL_REVERSE_WRITE,

	NUM_DEPTH_STENCIL
};

enum TSAMPLER_TYPE
{
	POINT_SAMPLER,
	LNEAR_SAMPLER,
	ANSIO16_SAMPLER,

	NUM_SAMPLER_TYPE
};


class RDX11Device;

class RDX11StateRepository
{
	bool		m_bCreated;

public:
	RDX11StateRepository();
	~RDX11StateRepository();

	void		CreateStates(ID3D11Device* pD3Device);
	void		Destroy();

	ID3D11BlendState*			GetBlenderState(ALPHA_BLEND_TYPE type);
	ID3D11RasterizerState*		GetRasterizerState(RASTERIZER_TYPE type);
	ID3D11DepthStencilState*	GetDepthStencilState(DEPTH_STENCIL_TYPE type);
	ID3D11SamplerState*			GetSamplerState(TSAMPLER_TYPE type);

private:
	ID3D11BlendState*			m_pBlendType[NUM_ALPHA_BLEND_TYPE];
	ID3D11RasterizerState*		m_pRasterizerType[NUM_RASTERIZER_TYPE];
	ID3D11DepthStencilState*	m_pDepthStencilType[NUM_DEPTH_STENCIL];
	ID3D11SamplerState*			m_SamplerStates[NUM_SAMPLER_TYPE];
	
	void		CreateSampler(ID3D11Device* pD3Device);
	void		CreateBlenderStates(ID3D11Device* pD3Device);
	void		CreateRasterizerStates(ID3D11Device* pD3Device);
	void		CreateDepthStencilStates(ID3D11Device* pD3Device);
};