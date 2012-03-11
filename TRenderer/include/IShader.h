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


struct SHADER_COMPILE_DESC
{
	LPCSTR		szFileName; 
	void*		pSrc; 
	SIZE_T		SrcDataSize;
	LPCSTR		szEntrypoint; 
	LPCSTR		szShaderModel; 

	UINT		compileFlag1;

	UINT		uberFlag;

	void*		shader_Macros;

	eCVERTEX_TYPE	eVertexyType;
	eSHADER_TYPE	shaderType;

	LPCSTR		debugName;

	SHADER_COMPILE_DESC()
	{
		memset( this, 0, sizeof(SHADER_COMPILE_DESC));
	}
};


#define MAX_SHADER_CONSTANT_SLOT	14

struct IShader
{
	virtual ~IShader(){}

	virtual SHADER_COMPILE_DESC& GetDesc() = 0;
};


struct IShaderMgr
{
	virtual bool		CheckShader(CRenderElement* pRenderElement) = 0;

	virtual void		Begin(UINT flag) = 0;
	virtual void		Begin(IShader* pVS, IShader* pPS) = 0;

	virtual void		SetShaderConstant(void* pScr, size_t size, UINT slot, eSHADER_TYPE type) = 0;
	virtual void		SetTexture(const CResourceTexture* pTexture, UINT slot) =0;

	virtual void		Reload(UINT flag) = 0;
};