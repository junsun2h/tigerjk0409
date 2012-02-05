#pragma once

#include "CDefine.h"

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


struct GRAPHIC_DEVICE_DESC
{
	GRAPHIC_DEVICE_DESC()
		: DepthStencil(DEPTH_STENCIL_INVALID)
		, RasterizerState(RASTERIZER_INVALID)
		, BlendState(BLEND_INVALID)
		, StencilRef(NULL)
		, blendFactor(NULL)
		, sampleMask(0xffffffff)
	{
	}

	DEPTH_STENCIL_TYPE	DepthStencil;
	RASTERIZER_TYPE		RasterizerState;
	ALPHA_BLEND_TYPE	BlendState;
	UINT				StencilRef;
	float*				blendFactor;
	UINT				sampleMask;
};


struct IRenderStateMgr
{
	virtual void	Init() = 0;
	virtual void	Destroy() = 0;

	virtual void	StoreCurrentState() = 0;
	virtual void	RestoreSavedState() = 0;

	virtual void	ApplyRenderState(const GRAPHIC_DEVICE_DESC& desc) = 0;
	virtual void	SetRasterizer(RASTERIZER_TYPE RasterizerState) = 0;
	virtual void	SetBlend(ALPHA_BLEND_TYPE BlendState, float* pblendFactor = NULL, UINT sampleMask = 0xffffffff) = 0;
	virtual void	SetDepthStancil(DEPTH_STENCIL_TYPE DepthStencilState, UINT StencilRef = NULL) = 0;
};