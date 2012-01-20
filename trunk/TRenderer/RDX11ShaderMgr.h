#pragma once

#include "RDefine.h"
#include <CGuid.h>
#include <atlcoll.h>
#include "RDX11Shader.h"
#include "CUnitPool.h"



enum EFFECT_TYPE
{
	EFFECT_FONT,
	EFFECT_LINE,

	NUM_EFFECT_TYPE
};

enum SHADER_TYPE
{
	VS_SHADER,
	GS_SHADER,
	PS_SHADER,

	NUM_SHADER_TYPE
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


class RDX11ShaderMgr
{
public:
	RDX11ShaderMgr();
	~RDX11ShaderMgr();

	void			init();
	void			Destroy();

	RDX11Shader*	GetShader(EFFECT_TYPE type);
	void			BeginShader(EFFECT_TYPE type);
	void			UpdateShaderConstant(void* pScr, size_t size, SHADER_CONST_BUFFER_SLOT slot, SHADER_TYPE type);

private:
	void			CreateFontShader();
	void			CreateLineShader();

	UINT			GetDXBufSize(SHADER_CONST_BUFFER_SLOT slot);

	
	typedef ATL::CAtlMap<UINT, ID3D11Buffer*>	CONST_BUFFER_MAP;
	CONST_BUFFER_MAP	m_ConstBufferMap[NUM_SHADER_TYPE];

	typedef ATL::CAtlMap<long, RDX11Shader*>	SHADER_MAP;
	SHADER_MAP			m_ShaderMap;

	CObjectPool<RDX11Shader>	m_MemPoolShader;
};
