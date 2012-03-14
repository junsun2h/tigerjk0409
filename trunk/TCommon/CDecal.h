#pragma once

class CResourceTexture;

enum eDECAL_TYPE
{
	DECAL_PROJECT,
	DECAL_VOLUME
};

struct CDecalDesc
{
	XMMATRIX			matrix;
	CVector3			position;
	float				radius;
	CVector3			color;

	float				lifeTime;
	CResourceMtrl*		pMtrl;
};
