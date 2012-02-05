#pragma once


enum EFFECT_TYPE
{
	EFFECT_FONT,
	EFFECT_LINE,

	EFFECT_GPASS_LAMBERT,
	EFFECT_MPASS_MESH,

	NUM_EFFECT_TYPE
};

enum SHADER_TYPE
{
	VS_SHADER,
	GS_SHADER,
	PS_SHADER,

	NUM_SHADER_TYPE
};


struct IShader
{
	virtual void		Begin() = 0;
	virtual void		End() = 0;

	virtual void		SetShaderContants(IEntityProxyRender* pRenderProxy) = 0;
	virtual void		SetShaderContants(CResourceMtrl* pMaterial) = 0;
	virtual void		SetShaderContants(XMMATRIX& tm) = 0;
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
	virtual IShader*	GetShader(EFFECT_TYPE type) = 0;
	virtual void		SetCurrentShader(IShader* pShader) =0;
	virtual IShader*	GetCurrentShader() = 0;

	virtual void		UpdateShaderConstant(void* pScr, size_t size, SHADER_CONST_BUFFER_SLOT slot, SHADER_TYPE type) = 0;
};