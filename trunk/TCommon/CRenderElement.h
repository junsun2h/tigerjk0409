#pragma once

#include <CResource.h>

struct IShader;
struct CLightDesc;

enum eRENDER_FLAG
{
	RENDER_FLAG_SKIN					= BIT(1),
	RENDER_FLAG_BUMP_MAP				= BIT(2),
	RENDER_FLAG_SPECULAR_MAP			= BIT(3),
	RENDER_FLAG_LIGHT					= BIT(4),

	RENDER_FLAG_DEPTH_TEST_OFF			= BIT(5),
	RENDER_FLAG_DEPTH_TEST_WRITE_OFF	= BIT(6),

	RENDER_FLAG_BLEND_ALPHA				= BIT(7),
	RENDER_FLAG_BLEND_CONSTANT			= BIT(8),

// special shader, don't use uber-system
	FONT_SHADER							= BIT(30),
	COLOR_MESH_SHADER					= BIT(31),
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
			flag |= RENDER_FLAG_BUMP_MAP;

		if( material.pTextures[TEXTURE_SPECULAR] != NULL )
			flag |= RENDER_FLAG_SPECULAR_MAP;
	}

	CRenderElement()
		: pVertexShader(NULL)
		, pPixelShader(NULL)
		, flag(0)
		, pGeometry(NULL)
		, meshSlot(-1)
		, pRefMatrix(NULL)
	{
	}
};


typedef	std::vector<CRenderElement>	RENDER_ELEMENT_LIST;
