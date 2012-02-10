#pragma once

struct IEngine;

#include "CDefine.h"
#include "CResource.h"
#include "CCamera.h"
#include "CEngineParam.h"
#include "CTimer.h"
#include "CQuad.h"
#include "CAABB.h"


#include "IRenderHelper.h"
#include "IEntityProxy.h"
#include "IRDevice.h"
#include "IEntityMgr.h"
#include "IAssetMgr.h"
#include "IRenderCallback.h"
#include "ILoader.h"
#include "IEntity.h"
#include "IResourceMemMgr.h"
#include "ISpaceMgr.h"



struct IEngine
{
	virtual bool				StartUp(const CENGINE_INIT_PARAM &param) = 0;
	virtual bool				ShutDown() = 0;
	virtual void				Resize(UINT width, UINT height) = 0;

	virtual IRDevice*			RDevice() = 0;
	virtual IAssetMgr*			AssetMgr() = 0;
	virtual IEntityMgr*			EntityMgr() =0;

	virtual IRenderHelper*		RenderHelper() =0;
	virtual CTimer*				GlobalTimer() = 0;
	virtual ILoader*			Loader() = 0;
	virtual IResourceMemMgr*	ResourceMemMgr() =0;
	virtual ISpaceMgr*			SpaceMgr() =0;
	virtual long				GetCurrentFrame() =0;

	virtual void				UpdateAndRender(IEntityProxyCamera* pCamera, IRenderingCallback* pRenderCallback) = 0;
};