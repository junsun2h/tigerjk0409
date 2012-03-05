#pragma once

#include <CResource.h>

struct IShader;

enum eRENDER_FLAG
{
	RENDER_FLAG_OPAQUE = BIT(1),
};

struct CRenderElement
{
	IShader*			pPixelShader;
	IShader*			pVertexShader;

	DWORD				flag;

	CResourceMtrl		material;
	bool				bMaterialChanged;
	CResourceGeometry*	pGeometry;
	XMMATRIX			worldMatrix;

	int8				meshSlotInActor;	// for skinned geometries

	CRenderElement()
		: pVertexShader(NULL)
		, pPixelShader(NULL)
		, flag(0)
		, bMaterialChanged(false)
		, pGeometry(NULL)
		, meshSlotInActor(-1)
	{
	}
};


typedef	std::vector<CRenderElement>	RENDER_ELEMENT_LIST;
