#pragma once

#include <CResource.h>

struct IShader;

enum eRENDER_FLAG
{
	RENDER_FLAG_SKIN					= BIT(1),
	RENDER_FLAG_BUMP_MAP				= BIT(2),
	RENDER_FLAG_SPECULAR_MAP			= BIT(3),

	RENDER_FLAG_DEPTH_TEST_OFF			= BIT(5),
	RENDER_FLAG_DEPTH_TEST_WRITE_OFF	= BIT(6),

	RENDER_FLAG_BLEND_ALPHA				= BIT(7),
	RENDER_FLAG_BLEND_CONSTANT			= BIT(8),

// special shader, don't use uber-system
	SHADER_FONT_VS						= BIT(25),
	SHADER_COLOR_VS						= BIT(26),
	SHADER_QUAD_VS						= BIT(27),
	SHADER_POS_VS						= BIT(28),

	SHADER_FONT_PS						= BIT(29),
	SHADER_COLOR_PS						= BIT(30),
	SHADER_TEXURE_PS					= BIT(31)
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

	void				InitFlag()
	{
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
