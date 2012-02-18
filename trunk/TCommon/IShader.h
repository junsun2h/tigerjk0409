#pragma once

#include "CVertex.h"



enum eSHADER_TYPE
{
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER,

	NUM_SHADER_TYPE,
};


struct IShader
{
	virtual ~IShader(){}

	virtual void			Begin() = 0;
	virtual eSHADER_TYPE	ShaderType() = 0;
	virtual	eEFFECT_TYPE	EffectType() { return INVAILD_EFFECT; };
	virtual void			SetShaderContants(const XMMATRIX& tm) {}
};


enum SHADER_CONST_BUFFER_SLOT
{
	// 16 byte buffer slot
	SM_BUF0_16BYTE_SLOT0,		
	SM_BUF1_16BYTE_SLOT1,
	SM_BUF2_16BYTE_SLOT2,

	// 32 byte buffer slot
	SM_BUF3_32BYTE_SLOT0,		
	SM_BUF4_32BYTE_SLOT1,
	SM_BUF5_32BYTE_SLOT2,

	// 64 byte buffer slot
	SM_BUF6_64BYTE_SLOT0,		
	SM_BUF7_64BYTE_SLOT1,
	SM_BUF8_64BYTE_SLOT2,

	// 128 byte buffer slot
	SM_BUF9_128BYTE_SLOT0,		
	SM_BUF10_128BYTE_SLOT1,

	// 192 byte buffer slot
	SM_BUF11_192BYTE_SLOT0,
	SM_BUF12_192BYTE_SLOT1,

	// 256 byte buffer slot
	SM_BUF13_256BYTE_SLOT0,
};


struct IShaderMgr
{
	virtual IShader*	GetShader(eEFFECT_TYPE type) = 0;
	virtual bool		SetCurrentShader(IShader* pShader) =0;

	virtual IShader*	GetCurrentVS() =0;
	virtual IShader*	GetCurrentPS() =0;
	virtual IShader*	GetCurrentGS() =0;

	virtual void		UpdateShaderConstant(void* pScr, size_t size, SHADER_CONST_BUFFER_SLOT slot, eSHADER_TYPE type) = 0;
};