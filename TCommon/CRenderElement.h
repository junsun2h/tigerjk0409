#pragma once

#include <CResource.h>

struct IShader;
struct CLightDesc;

enum eRENDER_FLAG
{
	RENDER_FLAG_TRANSPARENT	= BIT(1),
	RENDER_FLAG_SKIN		= BIT(2),
	RENDER_FLAG_BUMP		= BIT(3),
	RENDER_FLAG_LIGHT		= BIT(4),

	RENDER_FLAG_FONT		= BIT(5),
	RENDER_FLAG_COLOR		= BIT(6),
};

struct CRenderElement
{
	IShader*			pPixelShader;
	IShader*			pVertexShader;

	UINT				flag;

	CResourceMtrl		material;
	CResourceGeometry*	pGeometry;
	XMMATRIX			worldMatrix;
	
	UINT				meshSlot;	// mesh's slot number in Actor for skinned geometries
	UINT				refMatrixCount;
	XMMATRIX*			pRefMatrix;

	UINT				lightCount;
	CLightDesc*			pLights;

	void				InitFlag(bool bLight = true)
	{
		if( bLight )
			flag |= RENDER_FLAG_LIGHT;

		if( pGeometry->IsSkinMesh() )
			flag |= RENDER_FLAG_SKIN;

		if( material.pTextures[TEXTURE_BUMP] != NULL )
			flag |= RENDER_FLAG_BUMP;
	}

	CRenderElement()
		: pVertexShader(NULL)
		, pPixelShader(NULL)
		, flag(0)
		, pGeometry(NULL)
		, meshSlot(-1)
	{
	}
};


typedef	std::vector<CRenderElement>	RENDER_ELEMENT_LIST;
