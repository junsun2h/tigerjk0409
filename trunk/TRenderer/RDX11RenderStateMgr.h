#pragma once

#pragma once

#include "RDefine.h"


enum ALPHA_BLEND_TYPE
{
	BLEND_NONE,
	BLEND_ADD_BY_COLOR,
	BLEND_ADD_BY_ALPHA,

	NUM_ALPHA_BLEND_TYPE,

	BLEND_INVALID
};

enum RASTERIZER_TYPE
{
	RASTERIZER_CULL_FRONT,
	RASTERIZER_CULL_BACK,
	RASTERIZER_WIRE,
	RASTERIZER_SHADOW,

	NUM_RASTERIZER_TYPE,

	RASTERIZER_INVALID,
};

enum DEPTH_STENCIL_TYPE
{
	DEPTH_STENCIL_ON,
	DEPTH_STENCIL_OFF,
	DEPTH_STENCIL_REVERSE,
	DEPTH_STENCIL_WRITE,
	DEPTH_STENCIL_REVERSE_WRITE,

	NUM_DEPTH_STENCIL,

	DEPTH_STENCIL_INVALID
};

enum TSAMPLER_TYPE
{
	POINT_SAMPLER,
	LNEAR_SAMPLER,
	ANSIO16_SAMPLER,

	NUM_SAMPLER_TYPE
};


class RDX11Device;

class RDX11RenderStateMgr
{
public:
	RDX11RenderStateMgr();
	~RDX11RenderStateMgr();

	void		Init();
	void		Destroy();

	void		StoreCurrentState();
	void		RestoreSavedState();

	void		ApplyRenderState(DEPTH_STENCIL_TYPE DepthStencilState,
								RASTERIZER_TYPE RasterizerState,
								ALPHA_BLEND_TYPE BlendState,
								UINT StencilRef = NULL,
								float* blendFactor = NULL,
								UINT sampleMask = 0xffffffff);

	void		SetRasterizer(RASTERIZER_TYPE RasterizerState);
	void		SetBlend(ALPHA_BLEND_TYPE BlendState, float* pblendFactor = NULL, UINT sampleMask = 0xffffffff);
	void		SetDepthStancil(DEPTH_STENCIL_TYPE DepthStencilState, UINT StencilRef = NULL);

private:
	void		CreateSampler(ID3D11Device* pD3Device);
	void		CreateBlenderStates(ID3D11Device* pD3Device);
	void		CreateRasterizerStates(ID3D11Device* pD3Device);
	void		CreateDepthStencilStates(ID3D11Device* pD3Device);

	bool						m_bCreated;

	ID3D11BlendState*			m_pBlendType[NUM_ALPHA_BLEND_TYPE];
	ID3D11RasterizerState*		m_pRasterizerType[NUM_RASTERIZER_TYPE];
	ID3D11DepthStencilState*	m_pDepthStencilType[NUM_DEPTH_STENCIL];
	ID3D11SamplerState*			m_SamplerStates[NUM_SAMPLER_TYPE];

	// current device states
	ALPHA_BLEND_TYPE			m_CurrentBlend;
	RASTERIZER_TYPE				m_CurrentRasterizer;
	DEPTH_STENCIL_TYPE			m_CurrentDepthStencil;
	UINT						m_CurrentStencilRef;
	float*						m_CurrentblendFactor;
	UINT						m_CurrentsampleMask;


	// temporary object to save device states
	ID3D11DepthStencilState*	m_pStoredDepthStencil;
	UINT						m_StoredStencilRef;
	ID3D11RasterizerState*		m_pStoredRasterizer;
	ID3D11BlendState*			m_pStoredBlend;
	float						m_StoredBlendFactor[4];
	UINT						m_StoredSampleMask;

};