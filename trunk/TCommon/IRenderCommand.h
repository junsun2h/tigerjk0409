#pragma once

#include <CMathType.h>


class CResourceGeometry;
class CResourceMtrl;


enum eRENDER_BUFFER
{
	RENDER_BUFFER_0,
	RENDER_BUFFER_1,
	NUM_RENDER_BUFFER,
};


struct CRenderCommandBase
{
	CResourceGeometry*	pGeometry;
	XMMATRIX			worldTM;
	CResourceMtrl*		pMaterial;
};


struct CRenderCommandSkin : public CRenderCommandBase
{
	XMMATRIX*			refSkinTM;
	UINT				refSkinTMCount;
};



struct IRenderCommandMgr
{
	virtual byte*	AddCommand(eRENDER_COMMAND cmd, CRenderCommandBase* pCommand) = 0;
};
