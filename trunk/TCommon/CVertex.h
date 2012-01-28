#pragma once

#include "CDefine.h"
#include "CMathType.h"

enum eCVERTEX_TYPE
{
	FVF_3FP,
	FVF_3FP_1DC,		// 3 float position + 1 DWORD Color
	FVF_3FP_1DC_2HT,
	FVF_3FP_4BN_2HT,
	FVF_3FP_4BN_2HT_4BW,

	FVF_4HP_4BN_2HT = 1000,

	FVF_INVALID
};


struct CVertexP 
{
	CVector3		vPos;
};

struct CVertexPC 
{
	CVector3		vPos;
	uint32			color;
};

struct CVertexPCT
{
	CVector3	vPos;
	DWORD		vColor;
	XMHALF2		vTex;
};

struct CVertexPNT
{
	CVector3	vPos;
	XMBYTE4		vNormal;
	XMHALF2		vTex;
};

struct CVertexHPNT
{
	XMHALF4		vPos;
	XMBYTE4		vNormal;
	XMHALF2		vTex;
};

struct CVertexPNTW
{
	CVector3	vPos;
	XMBYTE4		vNormal;
	XMHALF2		vTex;
	XMUBYTE4	boneIDs;
	XMUBYTE4	fWeight;
};

inline void* NEW_VERTEX(eCVERTEX_TYPE type, int size)
{
	switch(type)
	{
	case FVF_3FP:				return new CVertexP[size];
	case FVF_3FP_1DC:			return new CVertexPC[size];
	case FVF_3FP_1DC_2HT:		return new CVertexPCT[size];
	case FVF_3FP_4BN_2HT:		return new CVertexPNT[size];
	case FVF_3FP_4BN_2HT_4BW:	return new CVertexPNTW[size];

	case FVF_4HP_4BN_2HT:		return new CVertexHPNT[size];
	}

	assert(0);
	return 0;
};


inline size_t VERTEX_STRIDE(eCVERTEX_TYPE type)
{
	switch(type)
	{
	case FVF_3FP:				return 12;
	case FVF_3FP_1DC:			return 16;
	case FVF_3FP_1DC_2HT:		return 20;
	case FVF_3FP_4BN_2HT:		return 20;
	case FVF_3FP_4BN_2HT_4BW:	return 28;

	case FVF_4HP_4BN_2HT:		return 16;
	}

	assert(0);
	return 0;
};

inline const char* VERTEX_TYPE_STRING(eCVERTEX_TYPE type)
{
	switch(type)
	{
	case FVF_3FP:				return ENUMSTR(FVF_3FP);
	case FVF_3FP_1DC:			return ENUMSTR(FVF_3FP_1DC);
	case FVF_3FP_1DC_2HT:		return ENUMSTR(FVF_3FP_1DC_2HT);
	case FVF_3FP_4BN_2HT:		return ENUMSTR(FVF_3FP_4BN_2HT);
	case FVF_3FP_4BN_2HT_4BW:	return ENUMSTR(FVF_3FP_4BN_2HT_4BW);

	case FVF_4HP_4BN_2HT:		return ENUMSTR(FVF_4HP_4BN_2HT);
	}

	assert(0);
	return 0;
};
