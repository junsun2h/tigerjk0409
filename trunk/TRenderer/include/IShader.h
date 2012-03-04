#pragma once

#include "CVertex.h"


enum eEFFECT_TYPE;
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
	virtual	eEFFECT_TYPE	EffectType() { return INVAILD_EFFECT; };

	virtual void			SetShaderContants(const XMMATRIX& tm) {}
	virtual void			SetShaderContants(XMMATRIX* tm, UINT size) { assert(0); }
};


struct IShaderMgr
{
	virtual bool		AssignShader(CRenderElement* pRenderElement) = 0;

	virtual IShader*	GetShader(eEFFECT_TYPE type) = 0;
	virtual bool		SetCurrentShader(IShader* pShader) =0;

	virtual IShader*	GetCurrentVS() =0;
	virtual IShader*	GetCurrentPS() =0;
	virtual IShader*	GetCurrentGS() =0;

	virtual void		UpdateShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type) = 0;
	virtual void		UpdateTexture(CResourceTexture* pTexture, UINT slot) =0;
};