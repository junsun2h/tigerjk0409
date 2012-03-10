#pragma once

#include "CVertex.h"


struct CRenderElement;

enum eSHADER_TYPE
{
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,

	NUM_SHADER_TYPE,
};

#define MAX_SHADER_CONSTANT_SLOT	14

struct IShader
{
	virtual ~IShader(){}

	virtual void			Begin() = 0;
	virtual eSHADER_TYPE	ShaderType() = 0;
};


struct IShaderMgr
{
	virtual bool		CheckShader(CRenderElement* pRenderElement) = 0;

	virtual IShader*	GetShader(UINT flag, eSHADER_TYPE shaderType) = 0;
	virtual void		Begin(UINT flag) = 0;

	virtual bool		CheckAndSet(IShader* pShader) =0;

	virtual IShader*	GetCurrentShader(eSHADER_TYPE type) =0;

	virtual void		SetShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type) = 0;
	virtual void		SetTexture(const CResourceTexture* pTexture, UINT slot) =0;
};