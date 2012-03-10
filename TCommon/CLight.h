#pragma once


struct IEntity;


enum eLIGHT_TYPE
{
	LIGHT_POINT,
	LIGHT_DIRECTION,
	LIGHT_SPOT
};

struct CLightDesc
{
	// aligned data for shader (32)
	CVector3	pos;
	float		range;
	CVector3	color;
	float		intensity;

	char		name[64];
	UINT		handle;	// handle in LightMgr
};

typedef std::vector<CLightDesc*> LIGHT_LIST;
