#pragma once

#include "CDefine.h"
#include "CLog.h"

struct IRDevice;		
struct IAssetMgr;
struct IEntity;
struct IEntityMgr;	
struct IRenderHelper;	
struct ILoader;
struct ISpaceMgr;		
struct IEntityProxyCamera;
struct IRenderingCallback;
struct CENGINE_INIT_PARAM;
struct CCAMERA_DESC;
struct IRenderer;

class CTimer;

struct IEngine
{
	virtual bool				StartUp(const CENGINE_INIT_PARAM* pParam) = 0;
	virtual bool				ShutDown() = 0;
	virtual void				Resize(UINT width, UINT height) = 0;

	virtual IAssetMgr*			AssetMgr() = 0;
	virtual IEntityMgr*			EntityMgr() =0;
	virtual IRenderer*			Renderer() = 0;

	virtual CTimer*				GlobalTimer() = 0;
	virtual ILoader*			Loader() = 0;
	virtual ISpaceMgr*			SpaceMgr() =0;
	virtual long				GetCurrentFrame() =0;

	virtual void				UpdateAndRender(CCAMERA_DESC* pCameraDesc, IRenderingCallback* pRenderCallback) = 0;
};